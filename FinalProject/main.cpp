#include <Arduino.h>
#include <DFRobot_PH.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <HttpClient.h>
//Define Pins
#define PH_PIN 33
#define TURB_PIN 32
#define GREEN_PIN 2
#define YELLOW_PIN 15
#define RED_PIN 13
//PH Values
float pHvoltage = 25;
float phValue = 25;
float temperature = 25;
//Calibration values for PH Sensor
float neutralVoltage = 1430;
float acidVoltage = 1960;
//Turbidity Values
float turbSensor;
float turbVoltage;

int calcTurbidity();


char ssid[] = "Dubu";    // your network SSID (name) 
char pass[] = "20252025"; // your network password (use for WPA, or use as key for WEP)


const char kHostname[] = "18.116.49.74";
// ✅ Updated port from 80 to 5000
const int kPort = 5000;

const int kNetworkTimeout = 30*1000;
const int kNetworkDelay = 1000;

void setup()
{
  Serial.begin(115200);  

  WiFi.begin(ssid,pass);
  Serial.println("Starting wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Initialization
  pinMode(TURB_PIN, INPUT);
  pinMode(PH_PIN, INPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(YELLOW_PIN,OUTPUT);
  pinMode(RED_PIN,OUTPUT);
  Serial.println("SETUP DONE");

}

void loop()
{

    WiFiClient c;
    HttpClient http(c);

    //Get PH Voltage
    pHvoltage = analogRead(PH_PIN)/4095.0*3300;
    //Grab values for PH Equation
    float slope = (7.0-4.0) / ((neutralVoltage - 1500) / 3.0 - (acidVoltage - 1500) / 3.0);
    float intercept = 7.0 - slope * (neutralVoltage - 1500) / 3.0;
    //calculate PH Value
    phValue = slope * (pHvoltage - 1500) / 3.0 + intercept;  /*UPLOAD THIS VALUE TO CLOUD FOR VISUALIZATION */

    //Get Turbidity value.
    int ntu = calcTurbidity();  /*UPLOAD THIS VALUE TO CLOUD FOR VISUALIZATION */ 
    //adding LED  warning logic
    if (ntu < 5.0 && (phValue > 6.75 && phValue < 8.25)) {
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(RED_PIN, LOW);
    }
    else if ((ntu >= 5.0 && ntu < 10.0) && (phValue >= 6.5 && phValue <= 8.5)){
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(RED_PIN, LOW);     
    }
    else {
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(RED_PIN, HIGH);
    }

    String path = "/?phValue=" + String(phValue) + "&ntu=" + String(ntu);
    delay(100);
    // ✅ Updated port number here:
    http.get(kHostname, kPort, path.c_str());

    //PRINT Values to Serial Monitor.
    Serial.print("pH:");
    Serial.println(phValue,2);
    Serial.print("Voltage: ");
    Serial.println(pHvoltage/1000,2);
    Serial.print("Turbidity: ");
    Serial.println(ntu);
    Serial.println("--------------");
    delay(500);
}
//Function to calculate Turbidity value.
int calcTurbidity() {
  int ntu;
  //Read Value
  for (int i = 0; i < 800; i ++) { //gathering 800 values and averaging.
    turbVoltage += (analogRead(TURB_PIN)) * (4.2/ 4095.0);
  }
  //Convert to voltage
  turbVoltage = turbVoltage / 800.0;
  if(turbVoltage < 2.5) { //end of equation
    ntu = 3000;
  }
  else{
    //Insert Voltage to NTU equation
    ntu = (-1120.4 * turbVoltage * turbVoltage)  + (5742.3 * turbVoltage) - 4352.9;
  }
  //Print values of sensor data and voltage calculated.
  Serial.print("Turb Voltage: ");
  Serial.println(turbVoltage);
  Serial.print("Turb Sensor: ");
  Serial.println(turbSensor);
  return ntu+18;
}