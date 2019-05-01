#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char *ssid = "STUDENT_RES";   //"big-brother"; //"HobbsTech";
const char *password = "Kalulu2007";  //"K&g9#36%r"; //"qwertyuiop";

#define ROOM_ID 27
#define ROOM_NAME Lounge

#define BT_SERIAL_RX D3
#define BT_SERIAL_TX D4

String HOST = "192.168.1.13";
String PORT = "8504";

String buff;
String baseUrl = "http://" + HOST + ":" + PORT;


SoftwareSerial btSerial(BT_SERIAL_RX, BT_SERIAL_TX);

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  btSerial.begin(9600);

}

void loop() {
  Serial.println("\n\n----------------------------------");
  buff = String("");
  delay(10000);

  while (btSerial.available()) {
    int ch = btSerial.read();
    buff.concat((char) ch);
  }
  Serial.print("--> Requested from Monitoring : ");
  Serial.println(buff);

  if (buff.length() > 0) {
    int flame = decodeFlame(buff);
    int gas = decodeGas(buff);
    int moisture = decodeMoisture(buff);
    float temp = decodeTemp(buff);
    float humidity = decodeHumidity(buff);
    int motion = decodeMotion(buff);

    postFlame(flame);
    postGas(gas);
    postMoisture(moisture);
    postTemp(temp);
    postHumidity(humidity);
    postMotion(motion);
  }
}

void postFlame(int flame) {
  String json = String("{");
  json.concat("\"flameValue\" : ");
  json.concat(flame);
  json.concat(",\"roomId\" : ");
  json.concat(ROOM_ID);
  json.concat("}");
  String url = baseUrl + "/v1/flames";
  doHttpCall(url, json);
}

void postGas(int gas) {
  String json = String("{");
  json.concat("\"gasValue\" : ");
  json.concat(gas);
  json.concat(",\"roomId\" : ");
  json.concat(ROOM_ID);
  json.concat("}");
  String url = baseUrl + "/v1/gasses";
  doHttpCall(url, json);
}

void postTemp(int temp) {
  String json = String("{");
  json.concat("\"temperature\" : ");
  json.concat(temp);
  json.concat(",\"roomId\" : ");
  json.concat(ROOM_ID);
  json.concat("}");
  String url = baseUrl + "/v1/room-temperature";
  doHttpCall(url, json);
}

void postMoisture(int moisture) {
  String json = String("{");
  json.concat("\"moistureValue\" : ");
  json.concat(moisture);
  json.concat(",\"roomId\" : ");
  json.concat(ROOM_ID);
  json.concat("}");
  String url = baseUrl + "/v1/moisture";
  doHttpCall(url, json);
}

void postHumidity(int humidity) {
  String json = String("{");
  json.concat("\"humidity\" : ");
  json.concat(humidity);
  json.concat(",\"roomId\" : ");
  json.concat(ROOM_ID);
  json.concat("}");
  String url = baseUrl + "/v1/humidity";
  doHttpCall(url, json);
}

void postMotion(int motion) {
  String json = String("{");
  json.concat("\"motion\" : ");
  json.concat(motion);
  json.concat(",\"roomId\" : ");
  json.concat(ROOM_ID);
  json.concat("}");
  String url = baseUrl + "/v1/room-motion-sensor";
  doHttpCall(url, json);
}

void doHttpCall(String url, String payload) {
  Serial.print("-----> ");
  Serial.println(url);
  Serial.print("-----> ");
  Serial.println(payload);
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(payload);
    String responsePayload = http.getString();
    Serial.print("---> Response : ");
    Serial.print(httpCode);
    Serial.print(" : ");
    Serial.println(responsePayload);
    http.end();
  } else {
    Serial.println("-----> WiFi is not connected, could not send data");
  }
}

int decodeFlame(String data) {
  return data.substring(data.indexOf('F') + 1, data.indexOf('G')).toInt();
}

int decodeGas(String data) {
  return data.substring(data.indexOf('G') + 1, data.indexOf('T')).toInt();
}

int decodeMoisture(String data) {
  return data.substring(data.indexOf('M') + 1, data.indexOf('F')).toInt();
}

float decodeTemp(String data) {
  return data.substring(data.indexOf('T') + 1, data.indexOf('H')).toFloat();
}

float decodeHumidity(String data) {
  return data.substring(data.indexOf('H') + 1, data.indexOf('O')).toFloat();
}

int decodeMotion(String data) {
  return data.substring(data.indexOf('0') + 1, data.indexOf('\n')).toInt();
}
