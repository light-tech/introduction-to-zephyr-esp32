# Introduction to Zephyr - An Adaptation to ESP32

This fork is my personal learning journey to learn Zephyr RTOS following [this series](https://www.youtube.com/watch?v=mTJ_vKlMS_4&list=PLEBQazB0HUyTmK2zdwhaf8bLwuEaDH-52&index=1).

Here I adapted the course to the older [ESP32 DevKitC](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/index.html) featuring an ESP-WROOM-32 instead of the S3. I am also using the latest Zephyr (4.1.0 as of this writing).

## Recommended Environment on Windows

On Windows, I found Docker and the VSCode inside it to be veryly slow. Building the container image also take a lot more time than working directly with WSL2. So my setup includes only

 * [WSL 2](https://learn.microsoft.com/en-us/windows/wsl/install) to install Zephyr and build
 * [Python](https://www.python.org/downloads/) to flash firmware

Just follow [the official instruction](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) for Windows.

I also keep this repo outside of WSL file system to easily edit the code using *VSCode on my host Windows machine*
```
cd E:\Projects\
git clone git@github.com:light-tech/introduction-to-zephyr-esp32.git
```

Then inside this clone repo, create a Python virtual environment to install flashing tools
```bat
# Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted -Force

cd E:\Projects\introduction-to-zephyr-esp32
python -m venv venv
venv\Scripts\activate
python -m pip install pyserial==3.5 esptool==4.8.1
```

Some useful VSCode extensions:

 1. [WSL]() that allows you to open WSL folder with your native VSCode so that you can browse the files in the Zephyr installation inside WSL
 2. [nRF DeviceTree](https://marketplace.visualstudio.com/items?itemName=nordic-semiconductor.nrf-devicetree) for syntax highlight of device tree files

## Build Application

Open WSL terminal and first do
```
source $HOME/zephyrproject/.venv/bin/activate
source $HOME/zephyrproject/zephyr/zephyr-env.sh
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=$HOME/zephyr-sdk-0.17.0
echo "Zephyr base: $ZEPHYR_BASE"
```
to set up the Zephyr build environment. (You can put this in your WSL `.bashrc` to have this always run on startup. Of course, adapt the paths depending on how you install Zephyr. The two `export` commands are just to speed up SDKs discovery; otherwise it will take longer to build.)

Now go find your repository on the host machine:
```
cd /mnt/e/Projects/introduction-to-zephyr-esp32/workspace/apps/01_demo_blink
west build -p always -b esp32_devkitc_wroom/esp32/procpu -- -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay
```

## Flash Application

You may need to install USB drivers for your development boards which could be [CP210x](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads) or [WCH340G](). Without this you cannot upload code or debug.

Open Powershell on the **host computer**, activate the Python virtual environment on which we installed the flashing tools earlier by running
```
cd E:\Projects\introduction-to-zephyr-esp32
.\venv\Scripts\activate
```

Now we can do
```sh
python -m esptool --port "<PORT>" --chip auto --baud 921600 --before default_reset --after hard_reset write_flash -u --flash_mode keep --flash_freq 40m --flash_size detect 0x1000 workspace/apps/01_demo_blink/build/zephyr/zephyr.bin
```
where `<PORT>` should be the COM port of your ESP32 DevKit assigned by Windows (check it in Device Manager).

It there is flashing error, you can try reset the device into bootloader mode by holding the *BOOT* button and pressing and releasing the *RESET* button and finally release the *BOOT* button.

## Serial Monitor

Run this

```sh
python -m serial.tools.miniterm "<PORT>" 115200
```

## License

All software in this repository, unless otherwise noted, is licensed under the [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0) license.
