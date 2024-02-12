# usb2serial
USB keyboard/mouse to serial interface converter using a Raspberry Pi Pico, based on ps2x2pico by No0ne
Additions have been made to the original design. Arrow keys are working. 


# Usage
* Copy `usb2serial.uf2` from build folder to your Pi Pico by pressing BOOTSEL before pluggging in. Serial BAUD RATE 115200
 UART TX PIN 28.


# Build
```
export PICO_SDK_PATH=/path/to/pico-sdk
mkdir build
cd build
cmake ..
make
```

# Resources
* https://github.com/No0ne/ps2x2pico
* https://github.com/raspberrypi/pico-sdk/tree/master
* 
