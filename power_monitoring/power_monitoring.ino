#include <SoftwareSerial.h>

#define SOFT_SERIAL_RX 2
#define SOFT_SERIAL_TX 3

#define CURRENT_SENSOR_PIN A4
#define VOLTAGE_SENSOR_PIN A5

SoftwareSerial btSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX);

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 40.0 / testFrequency;   // how long to average the signal, for statistist
unsigned long printPeriod = 1000; //Refresh rate
unsigned long previousMillis = 0;

float amplitude_current;
float effective_value;
float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float current_Volts; // Voltage
int voltageSensorReadValue;

void setup() {
  Serial.begin(115200);
}

void loop() {

  double voltage = measureVoltage();
  double current = measureCurrent();
  double power = voltage * current;
  btSerial.print("P");
  btSerial.print(power);
  Serial.print("---> Power : ");
  Serial.println(power);

}

double measureCurrent() {
  int readValue = analogRead(CURRENT_SENSOR_PIN);
  int sensor_max;
  sensor_max = getCurrentMaxValue();
  Serial.print("sensor_max = ");
  Serial.println(sensor_max);
  double voltageInput = ((sensor_max) * 5.0) / 1024.0;
  Serial.print("---> Current analog input : ");
  Serial.println(voltageInput);
  amplitude_current = (15 * voltageInput) - 37.5;
  effective_value = amplitude_current / 1.414;
  Serial.print("---> Amplitude current : ");
  Serial.println(amplitude_current);
  Serial.print("---> Effective current : ");
  Serial.println(effective_value);
  return effective_value;
}

double measureVoltage() {
  voltageSensorReadValue = analogRead(VOLTAGE_SENSOR_PIN);
  Serial.print("-----> Voltage analog value : ");
  Serial.println(voltageSensorReadValue);

  double voltageInput = voltageSensorReadValue * 5.0 / 1024;
  Serial.print("---> Voltage Arduino Input : ");
  Serial.println(voltageInput);
  
  Serial.print("---> Voltage : ");
  Serial.println(current_Volts);
  return current_Volts;

}

int getCurrentMaxValue() {
  int sensorValue;
  int sensorMax = 0;
  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000) {
    sensorValue = analogRead(CURRENT_SENSOR_PIN);
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }
  }
  return sensorMax;
}
