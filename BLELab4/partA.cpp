#include <Arduino.h>
#include <BLEDevice.h>

#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define LED_PIN 2 
 
class MyCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic) {
     std::string value = pCharacteristic->getValue();
     if (value == "On") {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("**********");
        Serial.println("Turning LED on.");
        Serial.println("**********");
     }
     if (value == "Off"){
        digitalWrite(LED_PIN, LOW);
        Serial.println("**********");
        Serial.println("Turning LED off.");
        Serial.println("**********");
     }
   }
};
void setup() {
   //Initialization
   Serial.begin(115200);
   pinMode(LED_PIN, OUTPUT);
   //BLE Init
   BLEDevice::init("SDSUCS");
   BLEServer *pServer = BLEDevice::createServer();
   BLEService *pService = pServer->createService(SERVICE_UUID);
   BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                       );
   pCharacteristic->setCallbacks(new MyCallbacks());
   pCharacteristic->setValue("Hello World");
   pService->start();
   BLEAdvertising *pAdvertising = pServer->getAdvertising();
   pAdvertising->start();
}
void loop() {
   delay(2000);
}