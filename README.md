# TrolaESP32
Home bus arrivals on 4*8x8 LED MATRIX display.<br>
[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/quEmyZB2cb8/0.jpg)](http://www.youtube.com/watch?v=quEmyZB2cb8)

## ESP32 for Arduino IDE  (C++) to get started
- [Arduino core for the ESP32](https://github.com/espressif/arduino-esp32)
- [ESP32 other help](http://esp32.net/)
- [video](https://www.youtube.com/watch?v=ZAqNKaX3LQ0&t=121s) that helped me to get going

## Arduino Libraries used
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

## Adapt for your station 
```
client.println("GET https://www.trola.si/[yourStationNumber]/[yourBusNumberOptional]/ HTTP/1.0");
```
Use [trola.si](https://www.trola.si/) or use [documentation](http://trolasi.readthedocs.io/en/latest/) to figure your station number and your http request. Try it out in [postman](https://chrome.google.com/webstore/detail/postman/fhbjgbiflinjbdggehcddcbncdddomop) also add **params** so you get json formatted response. 

key     | Value
------- | -----------------
Accept  | application/json

Paste json result in [ArduinoJson Assistant](https://arduinojson.org/v5/assistant/) and you will get all the data you need to customize the code for your needs. Then you can use this simple [example](https://arduinojson.org/v5/example/http-client/) to understand how it works. 

## Problems with connecting to the ESP32 in Windows 10
In my case the device was connected. I had the latest windows 10 drivers for the esp32 but I could not upload to the esp32. So I downgraded the [driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) for windows 7/8/8.1 it has the (default) tag, and that fixed the problem for me.
