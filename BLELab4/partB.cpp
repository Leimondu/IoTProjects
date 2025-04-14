
#include <Arduino.h>
#include <BLEDevice.h>
#include <SparkFunLSM6DSO.h>
#include <Wire.h>

#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
 

LSM6DSO myIMU; //Default constructor is I2C, addr 0x6B
int counter = 0; //counter of steps
float threshold = -1.05; //setting to a known working value

class MyCallbacks: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    pCharacteristic->setValue(counter);
  }
};

//calibrates the threshold value
void calibrate() {
  float reading;
  for (int i = 0; i <= 10; i++) {
    reading = myIMU.readFloatAccelX();
    if(reading < threshold) {
      threshold = reading;
    }
    delay(100);
  }

}

void setup() {

  //Initializations
  Serial.begin(115200);
  Wire.begin();
  if( myIMU.begin() )
    Serial.println("IMU Ready.");
  else { 
    Serial.println("Can not connect to IMU.");
  }
  if( myIMU.initialize(BASIC_SETTINGS) )
    Serial.println("Loaded Settings.");
  //Calibrating threshold value
  calibrate();
  //BLE Initialization
  BLEDevice::init("SDSUCS");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                        );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0); 
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
  delay (1000);
}

void loop()
{
  //Get accel parameter reading and print
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatAccelX(), 3);
  //Get gyroscope value along x axis
  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatGyroX(), 3);
  //Step logic
  if (myIMU.readFloatAccelX() < threshold  ) {
    counter ++;
  }
  //prints counter
  Serial.println("Counter: ");
  Serial.println(counter);

  delay(100);
}