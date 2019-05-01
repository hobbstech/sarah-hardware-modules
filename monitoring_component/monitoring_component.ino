#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>

#define MOTION_PIN 5
#define FLAME_PIN  A1
#define DHT11_PIN 4
#define GAS_PIN A0
#define LIGHT_RELAY_PIN 6
#define MOISTURE_SENSOR_PIN A2

#define DHT_TYPE DHT11

#define BT_SERIAL_RX 2
#define BT_SERIAL_TX 3 

#define INTERVAL_PERIOD 60000

DHT_Unified dht(DHT11_PIN, DHT_TYPE);
SoftwareSerial btSerial(BT_SERIAL_RX, BT_SERIAL_TX);

uint32_t delayMS;
float temp;
float humidity;
int moisture;
int flame;
int gas;
int motion;

void setup() {

  pinMode(MOTION_PIN, INPUT);
  pinMode(LIGHT_RELAY_PIN, OUTPUT);

  Serial.begin(9600);
  btSerial.begin(9600);

  dht.begin();
  sensor_t sensor;
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

}

void loop() {
  // put your main code here, to run repeatedly:
  temp = readDHT11Temprature(dht);
  humidity = readDHT11Humidity(dht);
  moisture = readMoisture();
  flame = readFlame();
  gas = readGas();
  motion = readMotion();

sendInfoToBlueTooth();

delay(INTERVAL_PERIOD);
  
}

int readMoisture() {
  return analogRead(MOISTURE_SENSOR_PIN);
}

int readGas() {
  return analogRead(GAS_PIN);
}

int readFlame() {
  return analogRead(FLAME_PIN);
}

int readMotion() {
  return digitalRead(MOTION_PIN);
}

float readDHT11Humidity(DHT_Unified dht) {

  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    return 0;
  }
  return event.relative_humidity;

}

float readDHT11Temprature(DHT_Unified dht) {

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    return 0;
  }
  return event.temperature;

}

void turnOnLights() {
  digitalWrite(LIGHT_RELAY_PIN, HIGH);
}

void turnOffLights() {
  digitalWrite(LIGHT_RELAY_PIN, LOW);
}

void sendInfoToBlueTooth() {

  Serial.print("--> Moisture : ");
  Serial.println(moisture);
  Serial.print("--> Flame : ");
  Serial.println(flame);
  Serial.print("--> Gas : ");
  Serial.println(gas);
  Serial.print("--> Temperature : ");
  Serial.println(temp);
  Serial.print("--> Humidity : ");
  Serial.println(humidity);
  Serial.print("--> Motion : ");
  Serial.println(motion);

  String dataToTransmit = String("");
  dataToTransmit.concat("M");
  dataToTransmit.concat(moisture);
  dataToTransmit.concat("F");
  dataToTransmit.concat(flame);
  dataToTransmit.concat("G");
  dataToTransmit.concat(gas);
  dataToTransmit.concat("T");
  dataToTransmit.concat(temp);
  dataToTransmit.concat("H");
  dataToTransmit.concat(humidity);
  dataToTransmit.concat("O");
  dataToTransmit.concat(motion);
  dataToTransmit.concat("\n");
//  char data[dataToTransmit.length()];
//  dataToTransmit.toCharArray(data, 32);
  Serial.println(dataToTransmit);

  btSerial.print("M");
  btSerial.print(moisture);
  btSerial.print("F");
  btSerial.print(flame);
  btSerial.print("G");
  btSerial.print(gas);
  btSerial.print("T");
  btSerial.print(temp);
  btSerial.print("H");
  btSerial.print(humidity);
  btSerial.print("O");
  btSerial.print(motion);
  btSerial.print("\n");
  //btSerial.print(dataToTransmit);

}

//void receiveEventHandler(int bytesReceived) {
//  while (Wire.available()) {
//    char ch = Wire.read();
//    Serial.print("--> Wire recieved :");
//    Serial.println(ch);
//    if (ch == '1') {
//      turnOnLights();
//    } else if (ch == '0') {
//      turnOffLights();
//    }
//  }
//}
