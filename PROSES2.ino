#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_Fingerprint.h>
uint8_t broadcastAddress[] = {0x78,0x21,0x84,0x7D,0x7E,0xA0};
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
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
  pinMode(26,OUTPUT);
  digitalWrite(26,HIGH);
  Serial.begin(9600);
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
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  WiFi.mode(WIFI_STA);
   if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()                     // run over and over again
{
  finger.fingerID = 0;
  getFingerprintID();
  Serial.println(finger.fingerID);
  int aa =finger.fingerID;
  if(finger.fingerID != 0){
    myData.a = "OKE";
    myData.b = finger.fingerID;
    finger.fingerID = 0;
     esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

            //don't ned to run this at full speed.
  }
  delay(2000);
}
