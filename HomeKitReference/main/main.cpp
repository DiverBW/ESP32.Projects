/// @file main.cpp
/// @brief HomeKit Reference Application
///
/// Test implementation for validating BWHomeKit component with display feedback.
/// Exposes virtual sensors that can be controlled via button or timer
/// for testing HomeKit pairing and state synchronization.

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "BWBoot/Boot.h"
#include "BWDisplay/BWDisplay.h"
#include "BWDisplay/ILI9341Driver.h"
#include "BWHomeKit/HomeKitManager.h"

using namespace BW;

static const char* TAG = "HomeKitRef";

// GPIO for boot button (used to toggle contact sensor)
static constexpr gpio_num_t BUTTON_GPIO = GPIO_NUM_9;

// Display pin configuration for ILI9341
static const ILI9341Pins DISPLAY_PINS = {
    .Mosi = GPIO_NUM_7,
    .Clk = GPIO_NUM_6,
    .Cs = GPIO_NUM_10,
    .Dc = GPIO_NUM_4,
    .Reset = GPIO_NUM_3,
    .Backlight = GPIO_NUM_5
};

// Service pointers for main loop updates
static ContactSensor* s_contactSensor = nullptr;
static OccupancySensor* s_occupancySensor = nullptr;
static Switch* s_virtualSwitch = nullptr;
static TemperatureSensor* s_tempSensor = nullptr;

// Simulated temperature state
static float s_simulatedTemp = 22.0f;
static bool s_tempRising = true;

/// @brief Configure the boot button GPIO
static void ConfigureButton()
{
    gpio_config_t config = {};
    config.pin_bit_mask = (1ULL << BUTTON_GPIO);
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&config);
}

/// @brief Check if button is pressed (active low)
static bool IsButtonPressed()
{
    return gpio_get_level(BUTTON_GPIO) == 0;
}

/// @brief Update the main status display
static void UpdateStatusDisplay()
{
    if (!Scr::HasDisplay())
        return;

    Scr::Clear();

    auto* drv = Scr::GetDriver();
    if (!drv)
        return;

    // Title (centered)
    drv->DrawText(80, 5, "HomeKit Reference", Colors::White, FontSize::Medium);

    // Sensor states - use direct pixel positioning for clean layout
    char line[40];

    snprintf(line, sizeof(line), "Door: %s", s_contactSensor->IsOpen() ? "OPEN" : "Closed");
    drv->DrawText(10, 40, line, s_contactSensor->IsOpen() ? Colors::Orange : Colors::Green, FontSize::Medium);

    snprintf(line, sizeof(line), "Motion: %s", s_occupancySensor->IsOccupied() ? "DETECTED" : "Clear");
    drv->DrawText(10, 65, line, s_occupancySensor->IsOccupied() ? Colors::Orange : Colors::Green, FontSize::Medium);

    snprintf(line, sizeof(line), "Switch: %s", s_virtualSwitch->IsOn() ? "ON" : "OFF");
    drv->DrawText(10, 90, line, s_virtualSwitch->IsOn() ? Colors::Yellow : Colors::Gray, FontSize::Medium);

    float tempF = s_simulatedTemp * 9.0f / 5.0f + 32.0f;
    snprintf(line, sizeof(line), "Temp: %.1f F", tempF);
    drv->DrawText(10, 115, line, Colors::Cyan, FontSize::Medium);

    // Instructions at bottom (small font)
    drv->DrawText(10, 200, "BOOT btn: toggle door", Colors::Cyan, FontSize::Small);
    drv->DrawText(10, 215, "Motion & temp change every 30s", Colors::Cyan, FontSize::Small);
}

/// @brief HomeKit pairing state change callback
static void OnPairingStateChanged(PairingState state)
{
    switch (state)
    {
        case PairingState::NotPaired:
            ESP_LOGI(TAG, "HomeKit: Not paired");
            Scr::ShowStatusBar("Waiting for pairing...", StatusIcon::Info);
            break;
        case PairingState::PairingInProgress:
            ESP_LOGI(TAG, "HomeKit: Pairing in progress...");
            Scr::ShowStatusBar("Pairing in progress...", StatusIcon::Sync);
            break;
        case PairingState::Paired:
            ESP_LOGI(TAG, "HomeKit: Paired successfully!");
            Scr::ShowToast("Paired with HomeKit!", 3000);
            break;
    }
}

/// @brief Switch write callback (from HomeKit)
static void OnSwitchWrite(bool on)
{
    ESP_LOGI(TAG, "HomeKit requested switch: %s", on ? "ON" : "OFF");

    char msg[32];
    snprintf(msg, sizeof(msg), "Switch: %s", on ? "ON" : "OFF");
    Scr::ShowToast(msg, 2000);

    // Update display after toast
    vTaskDelay(pdMS_TO_TICKS(2100));
    UpdateStatusDisplay();
}

/// @brief Temperature read callback (from HomeKit)
static float OnTemperatureRead()
{
    // Simulate temperature drift
    if (s_tempRising)
    {
        s_simulatedTemp += 0.5f;
        if (s_simulatedTemp >= 28.0f)
            s_tempRising = false;
    }
    else
    {
        s_simulatedTemp -= 0.5f;
        if (s_simulatedTemp <= 18.0f)
            s_tempRising = true;
    }

    float tempF = s_simulatedTemp * 9.0f / 5.0f + 32.0f;
    ESP_LOGI(TAG, "HomeKit requested temperature: %.1f F (%.1f C)", tempF, s_simulatedTemp);
    return s_simulatedTemp;
}

/// @brief Initialize display (must be called before Boot)
static void InitializeDisplay()
{
    ESP_LOGI(TAG, "Initializing display...");

    auto driver = std::make_unique<ILI9341Driver>(DISPLAY_PINS, Orientation::Landscape);
    Scr::RegisterDriver(std::move(driver));
    Scr::SetBacklight(255);

    if (Scr::HasDisplay())
    {
        ESP_LOGI(TAG, "Display initialized");
    }
    else
    {
        ESP_LOGW(TAG, "Display initialization failed");
    }
}

/// @brief Initialize HomeKit services (called from BeforeProvisioning hook)
static void InitializeHomeKit(BootContext& ctx)
{
    ctx.SetStatus("Initializing HomeKit...");

    // Initialize HomeKit
    esp_err_t ret = HomeKit().Initialize(AccessoryCategory::Sensor);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize HomeKit: %s", esp_err_to_name(ret));
        ctx.RequestAbort("HomeKit init failed");
        return;
    }

    // Set pairing callback
    HomeKit().SetPairingStateCallback(OnPairingStateChanged);

    // Add HomeKit services
    auto& accessory = HomeKit().GetAccessory();

    // Contact sensor (simulates door/window sensor)
    s_contactSensor = accessory.AddContactSensor("Virtual Door");
    if (!s_contactSensor)
    {
        ESP_LOGE(TAG, "Failed to add ContactSensor");
        ctx.RequestAbort("Failed to add ContactSensor");
        return;
    }

    // Occupancy sensor (simulates motion detector)
    s_occupancySensor = accessory.AddOccupancySensor("Virtual Motion");
    if (!s_occupancySensor)
    {
        ESP_LOGE(TAG, "Failed to add OccupancySensor");
        ctx.RequestAbort("Failed to add OccupancySensor");
        return;
    }

    // Temperature sensor (simulates thermometer)
    s_tempSensor = accessory.AddTemperatureSensor("Virtual Temp");
    if (!s_tempSensor)
    {
        ESP_LOGE(TAG, "Failed to add TemperatureSensor");
        ctx.RequestAbort("Failed to add TemperatureSensor");
        return;
    }
    s_tempSensor->SetReadCallback(OnTemperatureRead);

    // Virtual switch (can be toggled from Home app)
    s_virtualSwitch = accessory.AddSwitch("Virtual Switch");
    if (!s_virtualSwitch)
    {
        ESP_LOGE(TAG, "Failed to add Switch");
        ctx.RequestAbort("Failed to add Switch");
        return;
    }
    s_virtualSwitch->SetWriteCallback(OnSwitchWrite);

    ESP_LOGI(TAG, "Added %d HomeKit services", (int)accessory.GetServiceCount());
}

/// @brief Start HomeKit server (called from AfterWifiConnected hook)
static void StartHomeKit(BootContext& ctx)
{
    ctx.SetStatus("Starting HomeKit...");

    // Note: Provisioning web server is now automatically stopped when provisioning
    // completes (in BWProvisioning), so no manual Stop() call is needed here.

    esp_err_t ret = HomeKit().Start();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start HomeKit: %s", esp_err_to_name(ret));
        Scr::ShowError("HomeKit start failed");
        return;
    }

    ESP_LOGI(TAG, "HomeKit started successfully");
    ESP_LOGI(TAG, "Setup code: %s", HomeKit().GetSetupCode());

    // Show QR code for easy pairing if not already paired
    if (!HomeKit().IsPaired())
    {
        // Wait for any active overlay (toast) to clear before showing QR code
        while (Scr::HasActiveOverlay())
        {
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        const char* setupPayload = HomeKit().GetSetupPayload();
        if (setupPayload && Scr::ShowQRCode(setupPayload, "Scan to Pair"))
        {
            ESP_LOGI(TAG, "QR code displayed for pairing");
            ESP_LOGI(TAG, "Waiting for pairing (press BOOT button to skip)...");

            // Wait until paired, button pressed, or 60 second timeout
            uint32_t startTime = xTaskGetTickCount() * portTICK_PERIOD_MS;
            const uint32_t PAIRING_TIMEOUT_MS = 60000;

            while (!HomeKit().IsPaired())
            {
                // Check for button press to skip
                if (IsButtonPressed())
                {
                    ESP_LOGI(TAG, "Button pressed - skipping QR code display");
                    vTaskDelay(pdMS_TO_TICKS(300));  // Debounce
                    break;
                }

                // Check timeout
                uint32_t elapsed = (xTaskGetTickCount() * portTICK_PERIOD_MS) - startTime;
                if (elapsed >= PAIRING_TIMEOUT_MS)
                {
                    ESP_LOGI(TAG, "QR code timeout - proceeding to main display");
                    break;
                }

                vTaskDelay(pdMS_TO_TICKS(100));
            }

            if (HomeKit().IsPaired())
            {
                ESP_LOGI(TAG, "Device paired successfully!");
            }
        }
        else
        {
            // Fall back to showing setup code as toast
            char msg[48];
            snprintf(msg, sizeof(msg), "Pair: %s", HomeKit().GetSetupCode());
            Scr::ShowToast(msg, 5000);
            vTaskDelay(pdMS_TO_TICKS(5500));
        }
    }

    // Show main status display
    UpdateStatusDisplay();
}

/// @brief Main application entry point
extern "C" void app_main()
{
    ESP_LOGI(TAG, "=================================");
    ESP_LOGI(TAG, "HomeKit Reference Application");
    ESP_LOGI(TAG, "=================================");

    // Configure button GPIO
    ConfigureButton();

    // Initialize display FIRST (before Boot, so provisioning screens work)
    InitializeDisplay();

    // Use BWBoot for proper boot sequence
    // This BLOCKS until WiFi is connected (handling provisioning if needed)
    BootResult result = Boot()
        .WithAppIdentity("HomeKitRef", nullptr, "1.0.0")
        .WithSplash("HomeKit Reference", "v1.0.0", 2000)
        .WithProvisioning("HomeKit Reference", false)  // No discovery (HomeWeb)
        .BeforeProvisioning(InitializeHomeKit)
        .AfterWifiConnected(StartHomeKit)
        .Start();

    // Check boot result
    if (!result.Success)
    {
        ESP_LOGE(TAG, "Boot failed at phase %s: %s",
                 BootPhaseToString(result.FailedPhase),
                 result.ErrorMessage.c_str());
        Scr::ShowError("Boot failed");
        return;  // Don't enter main loop if boot failed
    }

    ESP_LOGI(TAG, "Boot complete, WiFi connected");

    // Main loop variables
    bool lastButtonState = false;
    uint32_t occupancyToggleTime = 0;
    bool displayNeedsUpdate = false;

    ESP_LOGI(TAG, "Entering main loop");
    ESP_LOGI(TAG, "Press BOOT button to toggle contact sensor");

    // Main loop - ONLY runs after WiFi is connected
    while (true)
    {
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

        // Check button for contact sensor toggle
        bool buttonPressed = IsButtonPressed();
        if (buttonPressed && !lastButtonState)
        {
            // Button just pressed - toggle contact sensor
            bool newState = !s_contactSensor->IsOpen();
            s_contactSensor->SetOpen(newState);
            ESP_LOGI(TAG, "Contact sensor: %s", newState ? "OPEN" : "CLOSED");

            char msg[32];
            snprintf(msg, sizeof(msg), "Door: %s", newState ? "OPEN" : "CLOSED");
            Scr::ShowToast(msg, 2000);
            displayNeedsUpdate = true;
        }
        lastButtonState = buttonPressed;

        // Toggle occupancy sensor every 30 seconds
        if (now - occupancyToggleTime > 30000)
        {
            occupancyToggleTime = now;
            bool newState = !s_occupancySensor->IsOccupied();
            s_occupancySensor->SetOccupied(newState);
            ESP_LOGI(TAG, "Occupancy sensor: %s", newState ? "OCCUPIED" : "NOT OCCUPIED");

            // Also update temperature (ramp up/down)
            if (s_tempRising)
            {
                s_simulatedTemp += 0.5f;
                if (s_simulatedTemp >= 28.0f)
                    s_tempRising = false;
            }
            else
            {
                s_simulatedTemp -= 0.5f;
                if (s_simulatedTemp <= 18.0f)
                    s_tempRising = true;
            }
            s_tempSensor->SetTemperature(s_simulatedTemp);
            float tempF = s_simulatedTemp * 9.0f / 5.0f + 32.0f;
            ESP_LOGI(TAG, "Temperature updated: %.1f F (%.1f C)", tempF, s_simulatedTemp);

            char msg[32];
            snprintf(msg, sizeof(msg), "Motion: %s", newState ? "DETECTED" : "Clear");
            Scr::ShowToast(msg, 2000);
            displayNeedsUpdate = true;
        }

        // Update main display after toast dismisses
        if (displayNeedsUpdate && !Scr::HasActiveOverlay())
        {
            UpdateStatusDisplay();
            displayNeedsUpdate = false;
        }

        // Small delay to prevent tight loop
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
