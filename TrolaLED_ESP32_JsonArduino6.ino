////LIBRARIES
#include <stdio.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

////LED MATRIX 
////LED_MATRIX HARDWARE TYPE---------↓
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES  4


////Pins esp32 DEVKITV1
#define CLK_PIN   14  // or SCK
#define DATA_PIN  12  // or MOSI
#define CS_PIN    15  // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
#define  DELAYTIME  100  // in milliseconds
////WI-FI
const char* ssid     = "Your SSID";     // your network SSID (name of wifi network)
const char* password = "YourPassword";         // your network password


const char*  server = "www.trola.si";         // Server URL



WiFiClientSecure client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


////Text display function
void scrollText(char *p)
{
  uint8_t charWidth;
  uint8_t cBuf[8];  // this should be ok for all built-in fonts

  //PRINTS("\nScrolling text");
  mx.clear();

  while (*p != '\0')
  {
    charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);

    for (uint8_t i=0; i<=charWidth; i++)  // allow space between characters
    {
      mx.transform(MD_MAX72XX::TSL);
      if (i < charWidth)
        mx.setColumn(0, cBuf[i]);
      delay(DELAYTIME);
    }
  }
}

////Request data and display it on LED_Matrix
void getTrolaDataAndDisplay()
{
  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 443)){
    Serial.println("Connection failed!");
    return;
  }
  else {
    Serial.println("Connected to server!");
    //Bus station number "603031"=Krimska direction Vižmarje https://www.trola.si/ 
    //Documentation http://trolasi.readthedocs.io/en/latest/
    client.println("GET https://www.trola.si/603031/ HTTP/1.0"); 
    client.println("Accept: application/json");   
    client.println("Connection: close");
    if (client.println() == 0) {
      Serial.println(F("Failed to send request"));
      return;
    }

    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }
    
    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      return;
    }

    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity. https://arduinojson.org/v5/assistant/
    const size_t bufferSize = JSON_ARRAY_SIZE(1) + 4*JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + 4*JSON_OBJECT_SIZE(3) + 220;
    DynamicJsonDocument jsonDocument(bufferSize);

    //Test json 
    //const char* client= "{\"stations\":[{\"number\":\"603031\",\"name\":\"Krimska\",\"buses\":[{\"direction\":\"Bavarski dvor - gameljne\",\"number\":\"1N\",\"arrivals\":[4,35,65]},{\"direction\":\"Vižmarje\",\"number\":\"1\",\"arrivals\":[4,35,65]},{\"direction\":\"Vižmarje\",\"number\":\"1D\",\"arrivals\":[20,50,80]}]}]}";
    //Test no arrivals
    //const char* client = "{\"stations\":[{\"number\":\"603031\",\"name\":\"Krimska\",\"buses\":[{\"direction\":\"Bavarski dvor - gameljne\",\"number\":\"1N\",\"arrivals\":[]},{\"direction\":\"Vižmarje\",\"number\":\"1\",\"arrivals\":[]},{\"direction\":\"Vižmarje\",\"number\":\"1D\",\"arrivals\":[]}]}]}";

     // Parse JSON object
    DeserializationError error = deserializeJson(jsonDocument, client);
     //JsonObject root = jsonBuffer.parseObject(client);
    
    if (error) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
    return;
    }
    
    // Extract values
    const char* stations0_number = jsonDocument["number"]; // "603031"
    const char* stations0_name = jsonDocument["name"]; // "Krimska"

    JsonArray stations0_buses = jsonDocument["buses"].as<JsonArray>();
   
    auto stations0_buses0 = stations0_buses[0].as<JsonObject>();
    const char* stations0_buses0_direction = stations0_buses0["direction"]; // "Bavarski dvor - gameljne"
    const char* stations0_buses0_number = stations0_buses0["number"]; // "1N"
    int stations0_buses0_arrivals0 = stations0_buses0["arrivals"][0]; // 4
    int stations0_buses0_arrivals1 = stations0_buses0["arrivals"][1]; // 35
    int stations0_buses0_arrivals2 = stations0_buses0["arrivals"][2]; // 65
    
    auto stations0_buses1 = stations0_buses[1].as<JsonObject>();
    const char* stations0_buses1_direction = stations0_buses1["direction"]; // "Bavarski dvor - gameljne"
    const char* stations0_buses1_number = stations0_buses1["number"]; // "1N"
    int stations0_buses1_arrivals0 = stations0_buses1["arrivals"][0]; // 4
    int stations0_buses1_arrivals1 = stations0_buses1["arrivals"][1]; // 35
    int stations0_buses1_arrivals2 = stations0_buses1["arrivals"][2]; // 65

    auto stations0_buses2 = stations0_buses[2].as<JsonObject>();
    const char* stations0_buses2_direction = stations0_buses2["direction"]; // "Bavarski dvor - gameljne"
    const char* stations0_buses2_number = stations0_buses2["number"]; // "1N"
    int stations0_buses2_arrivals0 = stations0_buses2["arrivals"][0]; // 4
    int stations0_buses2_arrivals1 = stations0_buses2["arrivals"][1]; // 35
    int stations0_buses2_arrivals2 = stations0_buses2["arrivals"][2]; // 65

    if(stations0_buses0_arrivals0 + stations0_buses0_arrivals1 + stations0_buses0_arrivals2 !=0){
      Serial.printf("[%s] %d, %d, %d \n",stations0_buses0_number,stations0_buses0_arrivals0,stations0_buses0_arrivals1,stations0_buses0_arrivals2);
      char buff[100];
      snprintf(buff, sizeof(buff), "%s-> %02d'%02d'",stations0_buses0_number,stations0_buses0_arrivals0,stations0_buses0_arrivals1);
      scrollText(buff);
      delay(2000);
    }
    else{
      Serial.printf("[%s] Ni prihodov\n",stations0_buses0_number);
    }
    
    if(stations0_buses1_arrivals0 + stations0_buses1_arrivals1 + stations0_buses1_arrivals2 !=0){
      Serial.printf("[%s] %d, %d, %d \n",stations0_buses1_number,stations0_buses1_arrivals0,stations0_buses1_arrivals1,stations0_buses1_arrivals2);
      char buff1[100];
      snprintf(buff1, sizeof(buff1), "%s-> %02d'%02d'",stations0_buses1_number,stations0_buses1_arrivals0,stations0_buses1_arrivals1);
      scrollText(buff1);
      delay(2000);
    }
    else{
      Serial.printf("[%s] Ni prihodov\n",stations0_buses1_number);
    }
    
    if(stations0_buses2_arrivals0 + stations0_buses2_arrivals1 + stations0_buses2_arrivals2 !=0){
      Serial.printf("[%s] %d, %d, %d \n",stations0_buses2_number,stations0_buses2_arrivals0,stations0_buses2_arrivals1,stations0_buses2_arrivals2);
      char buff2[100];
      snprintf(buff2, sizeof(buff2), "%s-> %02d'%02d'",stations0_buses2_number,stations0_buses2_arrivals0,stations0_buses2_arrivals1);
      scrollText(buff2);
      delay(2000);
    }
    else{
     Serial.printf("[%s] Ni prihodov\n",stations0_buses2_number);
    }
    /// No arrivals response
    if(stations0_buses0_arrivals0 + stations0_buses0_arrivals1 + stations0_buses0_arrivals2 + stations0_buses1_arrivals0 + stations0_buses1_arrivals1 + stations0_buses1_arrivals2 + stations0_buses2_arrivals0 + stations0_buses2_arrivals1 + stations0_buses2_arrivals2 ==0){
      scrollText("Ni prihodov             ");
    }
    
    mx.clear();
 
    // Disconnect
    client.stop();
  }
}


void setup() {
  // put your setup code here, to run once:
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 0); //Brightness LED_Matrix 0-15

  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  timeClient.begin(); 
  timeClient.setTimeOffset(7200); ///CET time
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //call for Trola Data
  getTrolaDataAndDisplay();
  //call for time update
  timeClient.update();
  Serial.println();
  Serial.println(timeClient.getFormattedTime());
  String str = timeClient.getFormattedTime(); //Magic to cut seconds HH:MM:SS
  char *cstr = &str[0u];
  char subbuff[8];
  memcpy( subbuff, &cstr[0], 5 );
  subbuff[5] = ' ';
  subbuff[6] = '\0';
  subbuff[7] = '\0';
  scrollText(subbuff); //display time on screen for 5 seconds then update all 
  delay(5000);
}
