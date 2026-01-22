@echo off
call C:\Espressif\idf_cmd_init.bat esp-idf-v5.5.2
cd /d C:\BradSoftware\ESP32\Projects\HomeKitReference
idf.py set-target esp32c6
idf.py build
