#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
int button = 5;
const int a1= 18;
typedef struct struct_message {
    String a;
    int b;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);

}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(button,INPUT);
  pinMode(18, OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
//  Serial.print(myData.b);
//digitalWrite(a1, 0);
//delay(1000);
//digitalWrite(a1, 1);
//delay(1000);
int s= digitalRead(button);
if(myData.b >=0 && myData.b<=127){
  digitalWrite(a1, 0);
  Serial.println("a1 = 0");
  
  }
  if(s == 0){
    digitalWrite(a1, 1);
    Serial.println("a1 = 1");
      myData.b = 128;
    }
  
}
