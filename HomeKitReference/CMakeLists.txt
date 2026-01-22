# HomeKit Reference Application
# Hardware-free test implementation for BWHomeKit component

cmake_minimum_required(VERSION 3.16)

# BW Framework components
set(EXTRA_COMPONENT_DIRS
    "C:/BradSoftware/ESP32/BW Framework/BWCore"
    "C:/BradSoftware/ESP32/BW Framework/BWWifi"
    "C:/BradSoftware/ESP32/BW Framework/BWHomeKit"
    "C:/BradSoftware/ESP32/esp-homekit-sdk/components/homekit"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(HomeKitReference)
