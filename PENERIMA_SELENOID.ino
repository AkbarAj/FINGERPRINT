#include <esp_now.h>
#include <WiFi.h>
#include<HTTPClient.h>

const char* ssid     = "mamank";
const char* password = "mamank69";
const char* serverName = "http://192.168.43.86/teslogin/api/finger";
int finger = 0;
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
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  finger =myData.b;
}
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
   Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
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
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
   
    // Specify content-type header
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
   // Prepare your HTTP POST request data
     Serial.print("finger: ");
     Serial.println(finger);
    String httpRequestData = "id=" + String(finger);
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    Serial.print("check data: ");
    Serial.print(finger);
     Serial.print(" end data: ");
     // Send HTTP POST request
    int httpResponseCode = 1;
    http.POST(httpRequestData);
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
   delay(5000);  

}
