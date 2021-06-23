#include <ESP8266WiFi.h>

#define STASSID "M&P-2.4" 
#define STAPSK  "Budapest2020"



const int trigPin = 12;
const int echoPin = 13;
const int relayPin = 14;
const int initialDelay = 10000;
const int pulseWidth = 6000;

const int connectionTimer = 100;
const int loopDelay = 10000;
const int alertLevel = 10;
const int fullWaterDepth = 2;
const int emptyWaterDepth = 14;

bool pump;
long counter=0;
int connectionCounter=0;
long tripTime;
int waterDepth;
float tankLevel;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
unsigned long previousTime2 = 0;
unsigned long lastPumpOn = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
const long period = 86400000;
// Variable to store the HTTP request
String header;

// Set web server port number to 80
WiFiServer server(80);

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  pinMode(echoPin, INPUT);
  delay(initialDelay);
  
  Serial.print("Initialized");

  WiFi.mode(WIFI_STA); 
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED and connectionCounter <connectionTimer) { 
    ++connectionCounter;
    Serial.print(".");
    delay(500);
  }
  if (connectionCounter <connectionTimer) {
      //para usar con ip fija
      //IPAddress Ip(192,168,0,243); 
      //IPAddress Gateway(192,168,0,1); 
      //IPAddress Subnet(255,255,255,0); 
      //WiFi.config(Ip, Gateway, Subnet); 
      
      Serial.println("");
      Serial.println("WiFi Connected");
      Serial.println(WiFi.localIP());
               
  }
  server.begin();
}

void(* resetFunc) (void) = 0;

// the loop function runs over and over again forever
void loop() {
  currentTime = millis();
  
//  if(currentTime%2000 == 0){
//  Serial.println("currentTime");
//  Serial.println(currentTime);
//  Serial.println("previousTime");
//  Serial.println(previousTime);
//  Serial.println(previousTime2);
//  Serial.println("LastPumpOn");
//  Serial.println(lastPumpOn);
//  }
  

  
  if(currentTime - previousTime2 > loopDelay){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    tripTime = pulseIn(echoPin, HIGH);
    waterDepth = (tripTime * 0.034) / 2;
    Serial.println("Water Measurement:");
    //Serial.print(waterDepth);
    tankLevel=map(waterDepth,emptyWaterDepth,fullWaterDepth,0,100);
    Serial.println(tankLevel);
    previousTime2=currentTime;
  }


  if(currentTime - lastPumpOn>=period and tankLevel>alertLevel){
    Serial.println("Relay Activated");
    digitalWrite(relayPin, HIGH);   
    lastPumpOn=currentTime;
    pump=true;
  }

  if(currentTime - lastPumpOn>pulseWidth and pump){
    Serial.println("Relay Deactivated");
    pump=false;
    digitalWrite(relayPin, LOW);   
  }

  if(currentTime>10e9){
    resetFunc(); 
    }
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {
    Serial.println("New Incoming Connection");
   
    // Escribir las cabeceras HTML
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
  
    //Escribir el contenido que vamos a visualizar
    client.println("<html>");
    client.println("<body>");
    client.print(String(tankLevel));
    client.println("</body>");
    client.println("</html>");
  }
   



}