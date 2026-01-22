/// @file main.cpp
/// @brief HomeKit Reference Application
///
/// Hardware-free test implementation for validating BWHomeKit component.
/// Exposes virtual sensors that can be controlled via button or timer
/// for testing HomeKit pairing and state synchronization.

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "BWCore/AppIdentity.h"
#include "BWWifi/WifiManager.h"
#include "BWHomeKit/HomeKitManager.h"

static const char* TAG = "HomeKitRef";

// GPIO for boot button (used to toggle contact sensor)
static constexpr gpio_num_t BUTTON_GPIO = GPIO_NUM_9;

// Service pointers for main loop updates
static BW::HomeKit::ContactSensor* s_contactSensor = nullptr;
static BW::HomeKit::OccupancySensor* s_occupancySensor = nullptr;
static BW::HomeKit::Switch* s_virtualSwitch = nullptr;
static BW::HomeKit::TemperatureSensor* s_tempSensor = nullptr;

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

/// @brief WiFi state change callback
static void OnWifiStateChanged(bw::WifiState state, const bw::WifiEventInfo& info)
{
    ESP_LOGI(TAG, "WiFi state: %s", bw::WifiStateToString(state));

    if (state == bw::WifiState::Connected)
    {
        ESP_LOGI(TAG, "WiFi connected, IP: %s", info.IPAddress.c_str());

        // Start HomeKit when WiFi connects
        esp_err_t ret = BW::HomeKit::HomeKit().Start();
        if (ret == ESP_OK)
        {
            ESP_LOGI(TAG, "HomeKit started successfully");
            ESP_LOGI(TAG, "Setup code: %s", BW::HomeKit::HomeKit().GetSetupCode());
        }
        else
        {
            ESP_LOGE(TAG, "Failed to start HomeKit: %s", esp_err_to_name(ret));
        }
    }
    else if (state == bw::WifiState::Disconnected)
    {
        // Stop HomeKit when WiFi disconnects
        BW::HomeKit::HomeKit().Stop();
    }
}

/// @brief HomeKit pairing state change callback
static void OnPairingStateChanged(BW::HomeKit::PairingState state)
{
    switch (state)
    {
        case BW::HomeKit::PairingState::NotPaired:
            ESP_LOGI(TAG, "HomeKit: Not paired");
            break;
        case BW::HomeKit::PairingState::PairingInProgress:
            ESP_LOGI(TAG, "HomeKit: Pairing in progress...");
            break;
        case BW::HomeKit::PairingState::Paired:
            ESP_LOGI(TAG, "HomeKit: Paired successfully!");
            break;
    }
}

/// @brief Switch write callback (from HomeKit)
///
/// Called when HomeKit requests the switch state change.
/// Note: The HAP layer has already updated the switch's internal state
/// before this callback is invoked. Use this callback for application
/// logic like controlling hardware (GPIO, relays, etc).
static void OnSwitchWrite(bool on)
{
    ESP_LOGI(TAG, "HomeKit requested switch: %s", on ? "ON" : "OFF");
    // In a real application, you would control hardware here:
    // gpio_set_level(RELAY_GPIO, on ? 1 : 0);
}

// Simulated temperature state (used by read callback)
static float s_simulatedTemp = 22.0f;
static bool s_tempRising = true;

/// @brief Temperature read callback (from HomeKit)
///
/// Called when HomeKit requests the current temperature.
/// This allows on-demand sensor reading instead of polling.
/// In a real application, you would read from actual hardware here.
static float OnTemperatureRead()
{
    // Simulate temperature drift
    if (s_tempRising)
    {
        s_simulatedTemp += 0.5f;
        if (s_simulatedTemp >= 28.0f)
        {
            s_tempRising = false;
        }
    }
    else
    {
        s_simulatedTemp -= 0.5f;
        if (s_simulatedTemp <= 18.0f)
        {
            s_tempRising = true;
        }
    }

    ESP_LOGI(TAG, "HomeKit requested temperature: %.1f C", s_simulatedTemp);
    return s_simulatedTemp;
}

/// @brief Initialize all components
static esp_err_t InitializeComponents()
{
    esp_err_t ret;

    // Initialize NVS (required for WiFi and HomeKit)
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGW(TAG, "NVS partition needs erase");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize BWCore (must be first BW component)
    ret = bw::App.Initialize("HomeKitRef", nullptr, "1.0.0");
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize BWCore: %s", esp_err_to_name(ret));
        return ret;
    }
    ESP_LOGI(TAG, "BWCore initialized: %s/%s",
             bw::App.GetAppName().c_str(),
             bw::App.GetAppId().c_str());

    // Initialize HomeKit (before adding services)
    ret = BW::HomeKit::HomeKit().Initialize(BW::HomeKit::AccessoryCategory::Sensor);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize HomeKit: %s", esp_err_to_name(ret));
        return ret;
    }

    // Set pairing callback
    BW::HomeKit::HomeKit().SetPairingStateCallback(OnPairingStateChanged);

    // Add HomeKit services
    auto& accessory = BW::HomeKit::HomeKit().GetAccessory();

    // Contact sensor (simulates door/window sensor)
    s_contactSensor = accessory.AddContactSensor("Virtual Door");
    if (!s_contactSensor)
    {
        ESP_LOGE(TAG, "Failed to add ContactSensor");
        return ESP_FAIL;
    }

    // Occupancy sensor (simulates motion detector)
    s_occupancySensor = accessory.AddOccupancySensor("Virtual Motion");
    if (!s_occupancySensor)
    {
        ESP_LOGE(TAG, "Failed to add OccupancySensor");
        return ESP_FAIL;
    }

    // Temperature sensor (simulates thermometer)
    // Uses read callback for on-demand reading instead of polling
    s_tempSensor = accessory.AddTemperatureSensor("Virtual Temp");
    if (!s_tempSensor)
    {
        ESP_LOGE(TAG, "Failed to add TemperatureSensor");
        return ESP_FAIL;
    }
    s_tempSensor->SetReadCallback(OnTemperatureRead);

    // Virtual switch (can be toggled from Home app)
    s_virtualSwitch = accessory.AddSwitch("Virtual Switch");
    if (!s_virtualSwitch)
    {
        ESP_LOGE(TAG, "Failed to add Switch");
        return ESP_FAIL;
    }
    s_virtualSwitch->SetWriteCallback(OnSwitchWrite);

    ESP_LOGI(TAG, "Added %d HomeKit services", (int)accessory.GetServiceCount());

    // Initialize WiFi
    ret = bw::WifiManager::Instance().Initialize();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize WiFi: %s", esp_err_to_name(ret));
        return ret;
    }

    // Set WiFi callback
    bw::WifiManager::Instance().SetStateCallback(OnWifiStateChanged);

    return ESP_OK;
}

/// @brief Main application entry point
extern "C" void app_main()
{
    ESP_LOGI(TAG, "=================================");
    ESP_LOGI(TAG, "HomeKit Reference Application");
    ESP_LOGI(TAG, "=================================");

    // Configure button GPIO
    ConfigureButton();

    // Initialize all components
    esp_err_t ret = InitializeComponents();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Initialization failed, halting");
        return;
    }

    // Try to connect with stored credentials
    if (bw::WifiManager::Instance().HasStoredCredentials())
    {
        ESP_LOGI(TAG, "Connecting to stored WiFi...");
        bw::WifiManager::Instance().Connect();
    }
    else
    {
        ESP_LOGW(TAG, "No stored WiFi credentials");
        ESP_LOGI(TAG, "Starting AP mode for provisioning...");
        bw::WifiManager::Instance().StartAP();
    }

    // Main loop variables
    bool lastButtonState = false;
    uint32_t occupancyToggleTime = 0;

    ESP_LOGI(TAG, "Entering main loop");
    ESP_LOGI(TAG, "Press BOOT button to toggle contact sensor");
    ESP_LOGI(TAG, "Temperature uses read callback (on-demand from HomeKit)");

    // Main loop
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
        }
        lastButtonState = buttonPressed;

        // Toggle occupancy sensor every 30 seconds
        if (now - occupancyToggleTime > 30000)
        {
            occupancyToggleTime = now;
            bool newState = !s_occupancySensor->IsOccupied();
            s_occupancySensor->SetOccupied(newState);
            ESP_LOGI(TAG, "Occupancy sensor: %s", newState ? "OCCUPIED" : "NOT OCCUPIED");
        }

        // Note: Temperature sensor uses read callback - no polling needed.
        // HomeKit will call OnTemperatureRead() when it needs the current value.

        // Small delay to prevent tight loop
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
