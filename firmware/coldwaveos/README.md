# Coldwave eOS - ARM Cortex-M Operating System 

Coldwave eOS is a realtime operating system library, designed specifically for ARM Cortex-M series microcontrollers.

Its purpose is to speed up development of specific device firmwares and ease the porting of code to a different Microcontroller ("CPU").

It was initially created to incorporate libflake on a very low level for microcontrollers and also to abstract os-level functions as well as networking functions, so the firmware is independent from the underlaying IP-Stack and/or RTOS.

In its current form and as of version 1.6.1 Coldwave eOS depends on the following external software packages:
- [FreeRTOS](https://www.freertos.org) as the preemptive realtime scheduler codebase and
- [lwip](https://savannah.nongnu.org/projects/lwip) as the TCP/IP stack. (The lwIP Implementation was forked and altered because of some bugs, so be careful when updating here).
- [mbedTLS](https://tls.mbed.org/) as the TLS and crypto implementation
- [MCUBoot](https://www.trustedfirmware.org/projects/mcuboot/index.html) as the bootloader
- [WPA supplicant](https://w1.fi/wpa_supplicant/) for WPA3 WLAN authentication

The source code has two main build targets: `coldwaveos-bundle` and `bootloader-bundle` where the latter produces a libary that contains all the bootloader code and only needs to be linked to a file containing the signing-pubkey in the application. the separate library is necessary, because the mbedTLS configuration for the OS would make the bootloader binary unnecessary big (since it contains more tables and curves).
 
## Memory usage 

The memory usage refers to release builds, debug builds will be up to 50% larger

* Bare OS C/C++ (with no drivers) *29kB*
* TCP/IP Stack *32kB*
* TLS Stack *230kB*
* MCUboot bootloader *37kB*

### Flake

* Flake Client Only *86kB*
* Flake Device (Router+Service) *142kB*

### Typical Applications

* Typical Driver Stack: *~50kB*

### Recommended Flash/RAM 

The flash layout of an application is

| area | description | size |
|------+-------------+------|
| BL | Bootloader | 40 kB |
| IMG0 | Active Firmware | (Total-Flash - BL - FS) / 2 |
| IMG1 | OTA Download area |  (Total-Flash - BL - FS) / 2 |
| FS | KV Filesystem | 32 kB | 

with a bootloader metadat overhead of ca 1kB in each flash image,
which leaves us with maximum firmware image sizes  (Total-Flash - BL - FS) / 2  of

| Flash Size | Max Image Size |
|---+---|
| 512kB |  219 kB |
| 1024kB |  475 kB | 
| 1536kB |  731 kB |
| 2048kB |  987 kB |

The recommended flash for typical applications would be:

* Flake-Device Firmware with Bootloader, LTE-M: *1024kB Flash* / *128kB RAM* 
* Flake-Device Firmware with Bootloader, WIFI & BLE: *1536kB Flash* / *256kB RAM* 

Smaller flash is only viable for applications the don't use IP networking functionality and therefore don't need lwIP and the TLS Stack, which reduces the code by about 260kB. 
A bare-minimum application only using the kernel, some drivers and updates via UART can be done with a flash size of 128kB, which is considered the absolute minimum.

### Prerequisites

For building the libraries (OS and bootloader) you need:

- CMake, minimum v3.26 [https://cmake.org/download/] 
- a GNU `make` binary (that usually comes bundled with e.g. XCode command line tools or the CLion IDE)
- ARM-GCC-Toolchain [https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads]
- As well as the following helper functions for some stages of the build process:
	- Perl
	- Python3 
	- Jinja2
	- Jsonschema
	- WordCount (wc) 

### Building

1. create a build folder in the project's root folder.
2. name this folder `build`.
3. cd into the build directory and execute `cmake ..` 
5. run `make` to compile and link the code
6. run `cmake --build . --target install` to create the libraries and includes for the selected SoC in the `/dist/` path

## Code Structure 

* `/arch`
	* arm-math CMSIS libraries that get exported during dist-build
	* core-startup code (`startup.c`)
	* the linker script template 
* `/cmake`
* `/dist`
	* everything that needs to be copied to a firmware-application project including libaries, includes, scripts. the dist folder gets populated on a dist-build with `CMAKE_BUILD_TYPE` set to `Release`
* `/doc`
	* when running doxygen the generated doc will be placed here
* `/driver`
	* driver stubs and sources. Details can be found in the [src/driver/Readme.md](/src/driver/Readme.md) 
* `/hal`
	* helper build script to download the SoC vendor's HAL code and add it to the build. The main confiuration variable affecting this is `CONFIG_HAL_NAME`
* `/include`
 	* the exported headers of the OS including kernel and driver headers. This directory gets exported in the `/dist` folder when an install is performed. add driver headers here for external hw or new OS-drivers. Details can be found in the [src/driver/Readme.md](/src/driver/Readme.md) 
* `/kernel`
	* everything related to the RTOS (FreeRTOS only as of now). **no need to add or edit things here**
* `/libc`
	* small replacement code for the usually too bloated libc
* `/modules`
	* utility modules that aren't part of the core OS functionality, like a small dhcp-server.
* `/platform`
	* everything that is not part of the kernel or a driver but provides important OS functionality. Here are the build-scripts, configuration files and porting code for mbedTLS, lwIP, FreeRTOS, libflake, libcoldwave and wpa_supplicant
* `/subsys`
	* core OS functionality like logging, over-the-air updates and a key-value filesystem.