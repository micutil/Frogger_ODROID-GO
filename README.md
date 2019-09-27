# Frogger for ODROID-GO

### Modified by Micutil

- M5Stack support, added the bin file.(2019/9/3)
- Delay time support (2019/9/3)  
- Added launcher code for SD-Update / LovyanLauncher.
- Modified sketch for Odroid-GO ([using ESP32-Chimera-Core](https://github.com/tobozo/ESP32-Chimera-Core)).
- Added binary for Odroid-GO

### Binary files for Launcher
- in M5_Gamebin ... 4M minimal SPIFFS partition scheme.
- in MD_Gamebin ... 4M default partition scheme.
- in MF_Gamebin ... Fire defaut partition scheme.
- in OG_Gamebin ... bin file for Odroid-GO.
- in odroid/firmwares ... fw file for Odroid-GO Menu Control.

##### ODROID-GO Device Firmware Menu Controls

1. Power Switch Off
2. Hold B button, Power Switch On
3. Release B button when device firmware Menu appears.

##### Launch an app

- A button: Select Application F/W
- B button: Cancel
- Start : Write Application F/W

## How to play

#### Odroid-GO

- Start: A button
- Delay time: Up/down key (Odroid-GO)
- Forwad: Up / down key
- Lateral: Left / right key 

#### M5Stack

- Start: A button
- Delay time: B or C button
- Forwad: A button
- Lateral: B or C button 


-------------------------
### Original READ ME

Frogger Clone for ODROID-GO

Copy the .fw file in /odroid/firmware and run it from Go-Play.

![alt text](https://raw.githubusercontent.com/pappani/Frogger_ODROID-GO/master/Screenshot.jpg)
