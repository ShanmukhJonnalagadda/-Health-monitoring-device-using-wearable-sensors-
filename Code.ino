
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

// ==== DS18B20 Temperature Sensor ====
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ==== Pulse Sensor ====
#define USE_ARDUINO_INTERRUPTS true
const int PulseWire = A0;       
const int LED13 = 13;
int Threshold = 550;
PulseSensorPlayground pulseSensor;

// ==== MMA7361 Accelerometer ====
int x = 0;
int y = 0;
int z = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Start Temperature Sensor
  sensors.begin();

  // Start Pulse Sensor
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  pulseSensor.blinkOnPulse(LED13);

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor initialized successfully!");
  } else {
    Serial.println("PulseSensor initialization failed.");
  }

  Serial.println("Setup complete.\n");
}

void loop() {
  Serial.println("=== SENSOR READINGS ===");

  // === Temperature Reading ===
  sensors.requestTemperatures();
  float celsius = sensors.getTempCByIndex(0);
  float fahrenheit = sensors.getTempFByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(celsius); Serial.print(" °C  |  ");
  Serial.print(fahrenheit); Serial.println(" °F");

  // === Pulse Reading ===
  int myBPM = pulseSensor.getBeatsPerMinute();

  if (pulseSensor.sawStartOfBeat()) {
    if (myBPM >= 60 && myBPM <= 70) {
      Serial.print("♥  Heartbeat detected! BPM: ");
      Serial.println(myBPM);
    } else {
      Serial.print("Heartbeat detected but BPM out of range (60–70): ");
      Serial.println(myBPM);
    }
  }

  // === Accelerometer Reading ===
  x = analogRead(A1);
  y = analogRead(A2);
  z = analogRead(A3);

  float xVoltage = x * 5.0 / 1024.0;
  float yVoltage = y * 5.0 / 1024.0;
  float zVoltage = z * 5.0 / 1024.0;

  Serial.print("X: "); Serial.print(x); Serial.print(" ("); Serial.print(xVoltage); Serial.println(" V)");
  Serial.print("Y: "); Serial.print(y); Serial.print(" ("); Serial.print(yVoltage); Serial.println(" V)");
  Serial.print("Z: "); Serial.print(z); Serial.print(" ("); Serial.print(zVoltage); Serial.println(" V)");

  Serial.println("-----------------------------\n");
  delay(1000);
}
