#include <WiFi.h>
#include <HttpClient.h>
#include <Adafruit_AHTX0.h>

// This example downloads the URL "http://arduino.cc/"

char ssid[] = "Andrew";    // your network SSID (name) 
char pass[] = "12345678"; // your network password (use for WPA, or use as key for WEP)

// Name of the server we want to connect to
const char kHostname[] = "3.17.76.34";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
//const char kPath[] = "/?var=25.3";

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

Adafruit_AHTX0 aht;

void setup() {

  
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());

  if (! aht.begin()) {
    Serial.println("Check wiring!");
    while (1) delay(10);
  }
  Serial.println("Temperature and humidity componenets Successfully attached!");
}

void loop() {
  WiFiClient c;
  HttpClient http(c);

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  Serial.print(" Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  String pathTemp = "/?var=" + String(temp.temperature);
  http.get(kHostname, 80, pathTemp.c_str());

  delay(3000); 

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");
  String pathHumid = "/?var=" + String(humidity.relative_humidity);
  http.get(kHostname, 80, pathHumid.c_str());
  delay(3000); 
}