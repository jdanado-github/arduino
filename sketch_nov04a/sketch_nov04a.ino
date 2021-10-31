/*
 * Relay Shield - Blink
 * Turns on the relay for two seconds, then off for two seconds, repeatedly.
 *
 * Relay Shield transistor closes relay when D1 is HIGH
 */

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "<token>";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "<My SSID>";
char pass[] = "<My Password>";

BlynkTimer timer;
 
const int relayPin = D1;
const int trigPin = D2;
const int echoPin = D3;

// defines variables
long duration;
int distance;
int vStatus;
int timerID = 0;

void readSensor()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, distance);

  if (distance > 20) {
    vStatus = 1;
  } else {
    vStatus = 2;
  }
  Blynk.virtualWrite(V1, vStatus);    
}

void click() {
    digitalWrite(relayPin, HIGH); // turn on relay with voltage HIGH
    delay(1000L);
    digitalWrite(relayPin, LOW);  // turn off relay with voltage LOW  
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  timer.deleteTimer(timerID);
  
  if (pinValue != vStatus ) {
    Serial.println("click");
    click();
    vStatus = pinValue;
  } 

  delay(5000L);
  timerID = timer.setInterval(5000L, readSensor);
}

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Blynk.begin(auth, ssid, pass);
  timerID = timer.setInterval(5000L, readSensor);
}
 
void loop() {
  Blynk.run();
  timer.run();
}
