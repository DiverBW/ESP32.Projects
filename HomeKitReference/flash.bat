@echo off
set MSYSTEM=
set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.5.2
set IDF_TOOLS_PATH=C:\Espressif
set IDF_PYTHON_ENV_PATH=C:\Espressif\python_env\idf5.5_py3.14_env
set PATH=C:\Espressif\python_env\idf5.5_py3.14_env\Scripts;C:\Espressif\tools\cmake\3.30.2\bin;C:\Espressif\tools\ninja\1.12.1;C:\Espressif\tools\riscv32-esp-elf\esp-14.2.0_20241119\riscv32-esp-elf\bin;%IDF_PATH%\tools;%PATH%
cd /d C:\BradSoftware\ESP32\Projects\HomeKitReference
python.exe "%IDF_PATH%\tools\idf.py" -p COM5 flash
