#include <Adafruit_SH110X.h>
#include <splash.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_SH1106.h>
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ_PIN A0
#define RL_VALUE 10.0
#define RO_CLEAN_AIR_FACTOR 3.08

float readMQ();
float getResistance(float sensor_volt);

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  dht.begin();
//display.setRotation(2); (in case you want to rotate display.)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Initializing");
  display.println("Breathalyzer...");
  display.display();
  delay(2000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float sensor_volt = analogRead(MQ_PIN) / 1024.0 * 5.0;
  float rs = getResistance(sensor_volt);
  float ratio = rs / RO_CLEAN_AIR_FACTOR;
  float ppm = pow(ratio, -1.179) * 4.385;
  float mgL = ppm * 1.995;
    display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Human Breathalyzer");
  display.drawLine(0, 12, 127, 12, WHITE);
  display.setCursor(0, 15);
  display.print("Acetone (ppm): ");
  display.print(ppm, 2);
  display.setCursor(0, 30);
  display.print("Acetone (mg/l): ");
  display.print(mgL, 2);
  display.setCursor(0, 45);
  display.print("Temp: ");
  display.print(t, 1);
  display.print(" C");
  display.setCursor(0, 55);
  display.print("Humidity: ");
  display.print(h, 1);
  display.print(" %");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Blow into sensor");
  display.display();
  delay(2000);
}

float readMQ() {
  int i;
  float rs=0;
  for (i=0;i<10;i++) {
    rs += getResistance(analogRead(MQ_PIN) / 1024.0 * 5.0 );
    delay(100);
  }
  rs = rs/10.0;
  return rs;
}

float getResistance(float sensor_volt) {
  return RL_VALUE * (5.0 - sensor_volt) / sensor_volt;
}
