#include <FastEPD.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <bb_captouch.h>

FASTEPD epaper;
BBCapTouch bbct;
const char* ssid = "demellocoffee";
const char* password = "dancinggoats";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);

  int rc;

  rc = bbct.init(12);
    if (rc == CT_SUCCESS) {
      Serial.printf("bb_captouch init success, type = %d\n", bbct.sensorType());
  } else {
    Serial.println("Error initializing bb_captouch");
    while (1) {};
  }

 epaper.initPanel(BB_PANEL_M5PAPERS3);
 epaper.setMode(BB_MODE_NONE);
 epaper.fillScreen(BBEP_WHITE);
 epaper.setFont(FONT_16x16);
 epaper.setTextColor(BBEP_BLACK);
 epaper.setRotation(90);

  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    // Serial.println("no networks found");
    epaper.drawString("no networks found", 5, 50);
  } else {
    // Serial.print(n);
    // Serial.println(" networks found");
    // Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
    // for (int i = 0; i < n; ++i) {
    //   // Print SSID and RSSI for each network found
    //   Serial.printf("%2d", i + 1);
    //   Serial.print(" | ");
    //   Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
    //   Serial.print(" | ");
    //   Serial.printf("%4ld", WiFi.RSSI(i));
    //   Serial.print(" | ");
    //   Serial.printf("%2ld", WiFi.channel(i));
    //   Serial.print(" | ");
    //   switch (WiFi.encryptionType(i)) {
    //     case WIFI_AUTH_OPEN:            Serial.print("open"); break;
    //     case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
    //     case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
    //     case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
    //     case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
    //     case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
    //     case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
    //     case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
    //     case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
    //     default:                        Serial.print("unknown");
    //   }
    //   Serial.println();
      delay(10);
    }
  



  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

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
          const char * payloadstring = payload.c_str();
          // epaper.drawString(payloadstring, 5, 50);
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


}

void loop()
{

//  epaper.setTextWrap(true);

  TOUCHINFO ti;
  while (1) {
    bbct.getSamples(&ti);
    if (ti.count > 0) {
      Serial.printf("x,y = %d,%d\n", ti.x[0], ti.y[0]);
    }
  }
 
}