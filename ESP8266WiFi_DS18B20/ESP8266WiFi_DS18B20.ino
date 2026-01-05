#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS1 D1
#define ONE_WIRE_BUS2 D2
#define ONE_WIRE_BUS3 D3

const char* server = "api.thingspeak.com"; // Your domain

String ApiKey1 = "O4C1EPUZQ87AOSV4";
String path1 = "/update?key=" + ApiKey1 + "&field1=";

String ApiKey2 = " ";
String path2 = "/update?key=" + ApiKey2 + "&field1=";

String ApiKey3 = " ";
String path3 = "/update?key=" + ApiKey3 + "&field3=";

OneWire oneWire1(ONE_WIRE_BUS1);
OneWire oneWire2(ONE_WIRE_BUS2);
OneWire oneWire3(ONE_WIRE_BUS3);

DallasTemperature DS18B20_1(&oneWire1);
DallasTemperature DS18B20_2(&oneWire2);
DallasTemperature DS18B20_3(&oneWire3);

const char* ssid = "HUAWEI-2.4G-7nGU";
const char* pass = "qhWAgt7b";

char temperatureString1[6];
char temperatureString2[6];
char temperatureString3[6];

int counter = 0;

void setup(void) {

  Serial.begin(115200);
  Serial.println("Start");

  WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting.....");
  }

  Serial.println("");
  Serial.print("Connected to "); 
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  DS18B20_1.begin();
  DS18B20_2.begin();
  DS18B20_3.begin();
}

float getTemperature1() {
  float temp1;
  do {
    DS18B20_1.requestTemperatures();
    temp1 = DS18B20_1.getTempCByIndex(0);
    delay(100);
  } while (temp1 == 85.0 || temp1 == (-127.0));
  return temp1;
}

float getTemperature2() {
  float temp2;
  do {
    DS18B20_2.requestTemperatures();
    temp2 = DS18B20_2.getTempCByIndex(0);
    delay(100);
  } while (temp2 == 85.0 || temp2 == (-127.0));
  return temp2;
}

float getTemperature3() {
  float temp3;
  do {
    DS18B20_3.requestTemperatures();
    temp3 = DS18B20_3.getTempCByIndex(0);
    delay(100);
  } while (temp3 == 85.0 || temp3 == (-127.0));
  return temp3;
}

void loop() {

  counter++;

  if (counter == 1000) {

    float temperature1 = getTemperature1();
    dtostrf(temperature1, 2, 2, temperatureString1);
    Serial.print("Temp1=  ");
    Serial.println(temperatureString1);
    Serial.print("\n\r");

    float temperature2 = getTemperature2();
    dtostrf(temperature2, 2, 2, temperatureString2);
    Serial.print("Temp2=  ");
    Serial.println(temperatureString2);
    Serial.print("\n\r");

    float temperature3 = getTemperature3();
    dtostrf(temperature3, 2, 2, temperatureString3);
    Serial.print("Temp3=  ");
    Serial.println(temperatureString3);
    Serial.print("\n\r");

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(server, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    String postStr = "api_key="+ApiKey1+"&field1="+temperatureString1+"&field2="+temperatureString2+"&field3="+temperatureString3;
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postStr.length());
    client.println();
    client.print(postStr);
    client.stop();

    Serial.print("Counter: ");
    Serial.println(counter);  
    Serial.println(postStr);                         
   }

   if (counter == 25000000) {
    Serial.print("Counter: ");
    Serial.println(counter);
    counter = 0;
  }
}

