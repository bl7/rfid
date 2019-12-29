#include <SPI.h>
#include <MFRC522.h>

#include <ESP8266WiFi.h> 
#include <FirebaseArduino.h>   

#define FIREBASE_HOST "iot-for-dash.firebaseio.com"
#define FIREBASE_AUTH "1gHjCiG1dY3huW2daf9gGbSjBk2OO7txK7YpRBfO"    

#define WIFI_SSID "iot"                                         
#define WIFI_PASSWORD "password"    


// RST = D3 / PIN20

// GND = GND

// IRQ = Not Used

// MISO = D6 / PIN40

// MOSI = D7 / PIN44

// SCK = D5 / PIN36

// SDA = D4 / PIN24
 
#define SS_PIN D4
#define RST_PIN D3
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();

}


void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Firebase.setString("/RFID", content);
  delay(5000);
 }
