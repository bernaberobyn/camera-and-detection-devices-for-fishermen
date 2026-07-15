# camera-and-detection-devices-for-fishermen

## Device 1: ESP32 based camera to capture images

### Hardware needed
1. [Controller with camera](https://amzn.to/3w7MiV1)
2. [Micro USB - USB cable](https://amzn.to/4aSkZNp)
3. [Power bank](https://amzn.to/3UhFdcn)
4. [Micro SD card](https://www.amazon.com/SanDisk-microSDHC-Memory-SDSDQ-004G-Packaging/dp/B00FM1JGHK)

### Environment setup
We have used Arduino IDE to program the hardware. Make sure to install the latest Arduino IDE with ESP32 board support packages, before getting to the next step.

### Arduino code for capturing images and saving them to SD card
There are 2 different .ino files that can achieve this:
1. [image_capture_on_reset.ino](https://github.com/bernaberobyn/camera-and-detection-devices-for-fishermen/blob/main/image_capture_on_reset.ino): This code helps in capturing an image on reset button press.
2. [image_capture_in_loop_with_timestamps](https://github.com/bernaberobyn/camera-and-detection-devices-for-fishermen/blob/main/image_capture_in_loop_with_timestamps.ino): This code can be used for running image capture periodically with timestamps to remove the hassle of pressing button while under water.

In Arduino IDE, create a new sketch and paste any one of the above codes. Select the correct board, port, and settings to upload the code to ESP32 hardware.
- Board: AI-Thinker ESP32-CAM
- Port: Check in device manager (for windows)
- PSRAM: Enabled
- FLASH Size: 4MB
- Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)

### Mechanical casing
Created a 3D printed case to enclose the device with camera exposed. Added a power supply in the case for remote operation.

## Device 2: 

