#include "HX711.h"

#include <WiFi.h>
#include <WebServer.h>
#include <esp_now.h>

//ESP: Cliente
// Dirección MAC del cliente (ESP32 del cliente)
//{0x84, 0xCC, 0xA8, 0x5B, 0xA0, 0x38}
//{0xFC, 0xF5, 0xC4, 0x2A, 0x14, 0x0C}
//{0x80, 0x64, 0x6F, 0xC4, 0xFC, 0xC0}
//uint8_t slaveAddress[] = {0x84, 0xCC, 0xA8, 0x5B, 0xA0, 0x38}; // Reemplaza con la dirección MAC del cliente

// Define a data structure
typedef struct struct_message {
  char a[46];
  int b;
  float c;
  bool d;
} struct_message;
 
// Create a structured object
struct_message myData;
 
 
// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Num. cliente: ");
  Serial.println(myData.b);
  Serial.print("Float Value: ");
  Serial.println(myData.c);
  Serial.print("Boolean Value: ");
  Serial.println(myData.d);
  Serial.println();
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
 
}
