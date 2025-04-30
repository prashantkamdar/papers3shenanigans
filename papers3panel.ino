#include <FastEPD.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

FASTEPD epaper;
const char* ssid = "demellocoffee";
const char* password = "dancinggoats";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void loop()
{
 epaper.initPanel(BB_PANEL_M5PAPERS3);
 epaper.setMode(BB_MODE_NONE);
 epaper.fillScreen(BBEP_WHITE);
 epaper.setFont(FONT_16x16);
 epaper.setTextColor(BBEP_BLACK);

 WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    // set secure client without certificate
    client->setInsecure();
    //create an HTTPClient instance
    HTTPClient https;

    //Initializing an HTTPS communication using the secure client
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://www.howsmyssl.com/a/check")) {  // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
       Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // print server response payload
          String payload = https.getString();
          Serial.println(payload);
          const char * c = payload.c_str();
          epaper.drawString(c, 100, 100);
        }
      }
      else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
  else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  Serial.println();



//  epaper.drawLine(0, 0, 700, 100, 0);
//  epaper.drawString("Partial Update Demoooo", 100, 100);
//  epaper.fillRoundRect(50, 50, 80, 80, 5, 0);
 Serial.print(78);
 epaper.fullUpdate();
 while (1) {};
}