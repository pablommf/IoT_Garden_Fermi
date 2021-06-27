//Code Developed to be used with Blynk app and 8266 arduino
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
char auth[] = "";//Enter your Auth token
char ssid[] = "";//Enter your WIFI name
char pass[] = "";//Enter your WIFI password
const int fullWaterDepth = 2;
const int emptyWaterDepth = 14;

BlynkTimer timer;
bool pinValue = 0;
 
#define trig 12
#define echo 13
#define relay 14
 
void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(10L, Wlevel);
  digitalWrite(relay, LOW);
}
 
BLYNK_WRITE(V0) {
  pinValue = param.asInt();
}
 
void loop() {
  Blynk.run();
  timer.run();
}
 
void Wlevel() {
  if (pinValue == 0) {
    digitalWrite(relay, LOW);
  } else if (pinValue == 1) {
    digitalWrite(relay, HIGH);
  }
 
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long waterDepth = t / 29 / 2;
  long tankLevel=map(waterDepth,emptyWaterDepth,fullWaterDepth,0,100);
  Blynk.virtualWrite(V1, tankLevel);
  Serial.println(tankLevel);

}