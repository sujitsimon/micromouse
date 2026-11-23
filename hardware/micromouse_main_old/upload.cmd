@echo off
echo "Running Upload"
set DFU_PATH=C:\D_Drive\Events\MicroMouse\dfu-util-0.9-win64\dfu-util
echo %DFU_PATH% -a 0 -s 0x8000000 -RD .\build\micromouse_main_old.ino.bin
%DFU_PATH% -a 0 -s 0x8000000 -RD .\build\micromouse_main_old.ino.bin