#include <esp_now.h>
#include <WiFi.h>
#include<HTTPClient.h>
const int s = 15;
const int r = 18;
const int btn = 5;
const char* ssid     = "mamank";
const char* password = "mamank69";
const char* serverName = "http://192.168.43.86/teslogin/api/finger";
const char* finger_masuk = "http://192.168.43.86/teslogin/api/finger_masuk";
const char* finger_keluar = "http://192.168.43.86/teslogin/api/finger_keluar";
int finger = 128;
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  String a;
  int b;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  finger = myData.b;
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  pinMode(s, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  digitalWrite(r, LOW);
  // Set device as a Wi-Fi Station

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print("No connected..");
  }
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  if (finger >= 0 && finger <= 127) {
    Serial.println("TESSSSSSS");
    digitalWrite(s, HIGH);
    delay(2000);
    digitalWrite(s, LOW);
    digitalWrite(r, 0);
    Serial.println("button = 0");
    delay(2000);
  }
  int s1 = digitalRead(btn);
  if (s1 == 0) {
    digitalWrite(s, HIGH);
    delay(2000);
    digitalWrite(s, LOW);
    digitalWrite(r, 1);
    Serial.println("button = 1");
    delay(2000);
    finger = 128;
  }
  
  finger = 128;
}
