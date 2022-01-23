## Thermo application
This is the Thermo application firmware source files.

## Prerequisites
#### Softwares

* [SEGGER Embedded Studio IDE](https://www.segger.com/downloads/embedded-studio/).
* [Git Bash](https://git-scm.com/downloads).
* [nRF connect desktop app](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Connect-for-desktop).
* nRF connect mobile app [Play Store](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp), [App Store](https://apps.apple.com/us/app/nrf-connect/id1054362403).

#### Hardware
Boards based on the [nRF52832](https://www.nordicsemi.com/products/nrf52832) or the [nRF52840](https://www.nordicsemi.com/products/nrf52840).

## How to run using SEGGER mbedded Studio

1. Clone this repository into your preferred location.
```
git clone 
```
Project strcuture :
```
application
-----------|components
-----------|main
-----------|pca10040 (nRF52832)
-----------|pca10056 (nRF52840)
assets
nRF5_SDK_17.0.2_d674dde
.gitignore
README.md
```
2. Execute the **.emProject** project file, which can be found here:
```
application/pca10056/s140/ses
```
3.  Using the **Build** menu, **Build and Run** the application.
4.  To access the application LOGs, use the **Debug** menu, then **Go**, then the **top green right poiting triangle** (continue execution).

## Pin configuartions
Inputs are configured in ``components/board/custom_board.h``

|    Device    |  Pin  |
| ------------ | ----- |
| WS2812B      | P0,16 |
