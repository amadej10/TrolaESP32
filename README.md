# TrolaESP32
Home bus arrivals on 4*8x8 LED MATRIX display.

## ESP32 for Arduino IDE  (C++) to get started
- [Arduino core for the ESP32](https://github.com/espressif/arduino-esp32)
- [ESP32 other help](http://esp32.net/)
- [video](https://www.youtube.com/watch?v=ZAqNKaX3LQ0&t=121s) that helped me to get going

## Arduino Libraries used:
- [MD_MAX72XX](https://github.com/MajicDesigns/MD_MAX72XX) for LED_MATRIX
- [NTPClient](https://github.com/arduino-libraries/NTPClient) to get the time
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) to get data from http GET request

## API used to get trola arrivals
- trola.si [documentation](http://trolasi.readthedocs.io/en/latest/) 

## Hardware used
- [MAX7219 4*8x8 LED Matrix](https://www.ebay.com/itm/MAX7219-Dot-led-matrix-MCU-control-LED-Display-module-for-Arduino-Raspberry-Pi/272583013958?_trkparms=aid%3D111001%26algo%3DREC.SEED%26ao%3D1%26asc%3D20160727114228%26meid%3D9713bea1c9bb4702beb4853cd5f4eec7%26pid%3D100290%26rk%3D1%26rkt%3D1%26mehot%3Dpp%26sd%3D272583013958%26itm%3D272583013958&_trksid=p2060778.c100290.m3507) FC-16 Hardware [library documentation](https://majicdesigns.github.io/MD_MAX72XX/page_f_c16.html)
- [EPS32](https://www.ebay.com/itm/352009863808?rmvSB=true) board

## Connections

ESP32   | MAX7219 8x8 LED Matrix
------- | ----------------------
VIN     | VCC
GND     | GND
GPIO12  | DIN
GPIO15  | CS
GPIO14  | CLK
