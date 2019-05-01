#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char *ssid = "HobbsTech";
const char *password = "qwertyuiop";

#define ROOM_ID 27
#define ROOM_NAME Lounge

#define SOFT_SERIAL_RX D3
#define SOFT_SERIAL_TX D4

String HOST = "192.168.1.13";
String PORT = "8504";

String buff;
String baseUrl = "http://" + HOST + ":" + PORT;


SoftwareSerial btSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);

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
  while (btSerial.available()) {
    int ch = btSerial.read();
    buff.concat((char) ch);
  }
  Serial.print("--> Received Data : ");
  Serial.println(buff);

  if (buff.length() > 0) {
    String power = decodePower(buff);
    postPower(power);
  }
}

String decodePower(String data){
  return data.substring(data.indexOf('P') + 1, data.indexOf('\n'));
}

void postPower(String power) {
  String json = String("{");
  json.concat("\"power\" : ");
  json.concat(power);
  json.concat(",\"roomId\" : ");
  json.concat(ROOM_ID);
  json.concat("}");
  String url = baseUrl + "/v1/power";
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
