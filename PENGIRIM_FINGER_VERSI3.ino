#include <esp_now.h>
#include <WiFi.h>
#include<HTTPClient.h>
#include <Adafruit_Fingerprint.h>
uint8_t broadcastAddress[] = {0x94, 0xb9, 0x7e, 0xd9, 0xcd, 0x4c};
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
const char* ssid     = "mamank";
const char* password = "mamank69";
const char* serverName = "http://192.168.43.86/teslogin/api/finger";
const char* finger_masuk = "http://192.168.43.86/teslogin/api/finger_masuk";
const char* finger_keluar = "http://192.168.43.86/teslogin/api/finger_keluar";

typedef struct struct_message {
  String a;
  int b;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
}

void loop()                     // run over and over again
{
  finger.fingerID = 0;
  getFingerprintID();
  Serial.println(finger.fingerID);
  int aa = finger.fingerID;
  if (finger.fingerID != 0) {
    myData.a = "OKE";
    myData.b = finger.fingerID;
    
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      http.begin(client, finger_masuk);
      http.begin(client, finger_keluar);
      //      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Prepare your HTTP POST request data
      Serial.print("finger: ");
      Serial.println(finger.fingerID);
      String httpRequestData = "id=" + String(finger.fingerID);
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);
      Serial.print("check data: ");
      Serial.print(finger.fingerID);
      Serial.print(" end data: ");
      // Send HTTP POST request
      int httpResponseCode = 1;
      http.POST(httpRequestData);

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
        if (result == ESP_OK) {
          Serial.println("Sent with success");
        }
        else {
          Serial.println("Error sending the data");
        }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }



    //don't ned to run this at full speed.
  }
  delay(2000);
  finger.fingerID = 0;
}
