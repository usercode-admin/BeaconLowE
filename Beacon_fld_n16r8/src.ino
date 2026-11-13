/* Optimization above all, performance first—taking everything to the extreme by making it light, fast, and powerful. */
/* src by: KenDrick | tiktok: @kne_rdick | github: https://github.com/usercode-admin/BeaconLowE | license: MIT */
/* mode: beacon */
/* version: 1.0.0 */
/* date: 2026-13-11 */


#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <esp_wifi.h>

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

const char* AP_SSID = "beacon_s3";
const char* AP_PASS = "12345678";

AsyncWebServer server(80);

volatile bool isRunning = false;
volatile uint8_t targetChannel = 1;
char prefixSSID[24] = "Your_SSID";
uint8_t prefixLen = 10;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

TaskHandle_t beaconTaskHandle = NULL;

uint8_t beaconPacket[38] = {
  0x80, 0x00, 0x00, 0x00,                 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,     
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,     
  0x00, 0x00,                             
  0x83, 0x51, 0xF7, 0x8F, 0x0F, 0x00, 0x00, 0x00, 
  0xE8, 0x03,                             
  0x31, 0x00,                             
  0x00, 0x20,                             
};

const uint8_t postTemplate[] = {
  0x01, 0x08, 0x82, 0x84, 0x8B, 0x96, 0x24, 0x30, 0x48, 0x6C,
  0x03, 0x01, 1
};

inline void fastRandomMAC(uint8_t* mac) {
  uint32_t r = esp_random();
  mac[0] = 0x02;
  mac[1] = (uint8_t)(r >> 24);
  mac[2] = (uint8_t)(r >> 16);
  mac[3] = (uint8_t)(r >> 8);
  mac[4] = (uint8_t)(r);
  mac[5] = (uint8_t)(esp_random() & 0xFF); 
}

inline int fastAppendNumber(char* buf, uint32_t n) {
  if (n == 0) {
    buf[0] = '0';
    return 1;
  }
  char temp[10];
  int i = 0;
  while (n > 0) {
    temp[i++] = '0' + (n % 10);
    n /= 10;
  }
  int len = i;
  for (int j = 0; j < len; j++) {
    buf[j] = temp[--i];
  }
  return len;
}

void beaconTask(void * pvParameters) {
  uint8_t buf[128];
  memcpy(buf, beaconPacket, 38);
  
  uint8_t localPostTemplate[sizeof(postTemplate)];
  memcpy(localPostTemplate, postTemplate, sizeof(postTemplate));

  uint32_t counter = 0;
  uint16_t seqNum = 0;
  char currentSSID[33]; 
  char localPrefix[24]; 
  uint8_t localPrefixLen = 0;

  while (true) {
    if (isRunning) {
      portENTER_CRITICAL(&mux);
      memcpy(localPrefix, prefixSSID, sizeof(localPrefix));
      localPrefixLen = prefixLen;
      uint8_t currentTargetCh = targetChannel;
      portEXIT_CRITICAL(&mux);

      static uint8_t lastChannel = 0;
      if (currentTargetCh != lastChannel) {
        lastChannel = currentTargetCh;
        esp_wifi_set_channel(lastChannel, WIFI_SECOND_CHAN_NONE);
        localPostTemplate[12] = lastChannel; 
      }

      if (localPrefixLen > 20) localPrefixLen = 20;
      memcpy(currentSSID, localPrefix, localPrefixLen);
      
      int numLen = fastAppendNumber(currentSSID + localPrefixLen, counter++);
      int written = localPrefixLen + numLen;
      if (written > 32) written = 32;

      if (counter > 999999) counter = 0;

      seqNum = (seqNum + 1) & 0x0FFF;
      buf[22] = (seqNum & 0x0F) << 4;
      buf[23] = (seqNum >> 4) & 0xFF;

      fastRandomMAC(&buf[10]);
      fastRandomMAC(&buf[16]);

      buf[37] = (uint8_t)written;
      memcpy(&buf[38], currentSSID, written);
      int pos = 38 + written;

      memcpy(&buf[pos], localPostTemplate, sizeof(localPostTemplate));
      pos += sizeof(localPostTemplate);

      esp_wifi_80211_tx(WIFI_IF_AP, buf, pos, false);

      if ((counter & 0x1F) == 0) {
        vTaskDelay(1); 
      }
      
    } else {
      vTaskDelay(pdMS_TO_TICKS(50)); 
    }
  }
}

void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>BeaconLowE</title></head><body style='font-family:sans-serif;padding:10px'>";
    html += "<h3>status: " + String(isRunning ? "<span style='color:red'>run</span>" : "<span style='color:green'>waiting...</span>") + "</h3>";
    html += "<form action='/set' method='GET'>";
    html += "SSID prefix:<br><input name='ssid' value='" + String(prefixSSID) + "' maxlength='20' style='width:100%;margin:5px 0'><br>";
    html += "Kênh (1-11):<br><input name='ch' type='number' value='" + String(targetChannel) + "' min='1' max='11' style='width:100%;margin:5px 0'><br>";
    html += "<button type='submit' style='width:100%;padding:10px;background:red;color:white;border:none;margin-top:10px'>start</button>";
    html += "</form>";
    html += "<form action='/stop' method='GET'><button type='submit' style='width:100%;padding:10px;background:gray;color:white;border:none;margin-top:5px'>stop</button></form>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request){
    portENTER_CRITICAL(&mux);
    if (request->hasParam("ssid")) {
      String s = request->getParam("ssid")->value();
      s.toCharArray(prefixSSID, sizeof(prefixSSID));
      prefixLen = s.length();
    }
    if (request->hasParam("ch")) {
      targetChannel = constrain(request->getParam("ch")->value().toInt(), 1, 11);
    }
    portEXIT_CRITICAL(&mux);
    
    isRunning = true;
    request->redirect("/");
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    isRunning = false;
    request->redirect("/");
  });

  server.begin();
}

void setup() {
  setCpuFrequencyMhz(160);

  Serial.begin(115200);
  delay(500);
  
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(AP_SSID, AP_PASS);
  
  esp_wifi_start();
  esp_wifi_set_ps(WIFI_PS_NONE);

  setupServer();

  xTaskCreatePinnedToCore(
    beaconTask,
    "BeaconTask",
    4096,
    NULL,
    1,
    &beaconTaskHandle,
    0
  );
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
