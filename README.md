![banner](.images/banner-dark-theme.png#gh-dark-mode-only)
![banner](.images/banner-light-theme.png#gh-light-mode-only)

# micro-ROS module for Raspberry Pi Pico SDK

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Getting Started

Here is a quick way to compile the example given in this repository.

### Dependencies

micro-ROS precompiled library is compiled using `arm-none-eabi-gcc` [9.3.1](https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2), a compatible version is expected when building the micro-ROS project.
You can specify a compiler path with the following command:

```bash
# Configure environment
echo "export PICO_TOOLCHAIN_PATH=..." >> ~/.bashrc
source ~/.bashrc
```

### 1. Install Pico SDK
First, make sure the Pico SDK is properly installed and configured:

```bash
# Install dependencies
sudo apt install cmake g++ gcc-arm-none-eabi doxygen libnewlib-arm-none-eabi git python3
git clone --recurse-submodules https://github.com/raspberrypi/pico-sdk.git $HOME/pico-sdk

# Configure environment
echo "export PICO_SDK_PATH=$HOME/pico-sdk" >> ~/.bashrc
source ~/.bashrc
```

### 2. Compile Example

Once the Pico SDK is ready, compile the example:

```bash
cd micro_ros_raspberrypi_pico_sdk
mkdir build
cd build
cmake ..
make
```

To flash, hold the boot button, plug the USB and run:
```
cp pico_micro_ros_example.uf2 /media/$USER/RPI-RP2
```

### 3. Start Micro-ROS Agent
Micro-ROS follows the client-server architecture, so you need to start the Micro-ROS Agent.
You can do so using the [micro-ros-agent Snap](https://snapcraft.io/micro-ros-agent) (follow the link for installation details):
For serial comunication:
```bash
micro_ros_agent serial --dev /dev/ttyACM0 -b 115200
```
For wireless comunication:
```bash
micro_ros_agent udp4 --port 4444
```

or using the [micro-ros-agent Docker](https://hub.docker.com/r/microros/micro-ros-agent):
For serial comunication:
```bash
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:iron serial --dev /dev/ttyACM0 -b 115200
```
For wireless comunication:
```bash
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:iron udp4 --port 4444
```

## What files are relevant?
- `pico_uart_transport.c`: Contains the board specific implementation of the serial transport (no change needed).
- `pico_wifi_transport.c`: Contains the board specific implementation of the wifi transport (no change needed).
- `CMakeLists.txt`: CMake file.
- `pico_micro_ros_example.c`: The actual ROS 2 publisher.

## How to build the precompiled library

Micro-ROS is precompiled for Raspberry Pi Pico in [`libmicroros`](libmicroros).
If you want to compile it by yourself:

```bash
docker pull microros/micro_ros_static_library_builder:iron
docker run -it --rm -v $(pwd):/project microros/micro_ros_static_library_builder:iron
```

Note that folders added to `microros_static_library/library_generation/extra_packages` and entries added to `microros_static_library/library_generation/extra_packages/extra_packages.repos` will be taken into account by this build system.
## How to use Pico SDK?

Here is a Raspberry Pi Pico C/C++ SDK documentation:
https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf
## Purpose of the Project

This software is not ready for production use. It has neither been developed nor
tested for a specific use case. However, the license conditions of the
applicable Open Source licenses allow you to adapt the software to your needs.
Before using it in a safety relevant setting, make sure that the software
fulfills your requirements and adjust it according to any applicable safety
standards, e.g., ISO 26262.

## License

This repository is open-sourced under the Apache-2.0 license. See the [LICENSE](LICENSE) file for details.

For a list of other open-source components included in this repository,
see the file [3rd-party-licenses.txt](3rd-party-licenses.txt).

## Known Issues/Limitations

There are no known limitations.
