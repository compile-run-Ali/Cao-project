#include <Arduino.h>
#include <wifi.h>

//my wifi name and password
const char* wifiName="HASEEB";
const char* wifiPassword="65464816";
WiFiServer server(80);

//these are the GPIO pins of esp32
const int trigPin = 27;//setting the trig pin
const int echoPin = 25;//setting the echo pin 
const int ledPin=18;//setting the ledpin

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode (ledPin, OUTPUT);//sets the ledPin as another output

  Serial.print("Connecting to ");
  Serial.println(wifiName);
  WiFi.begin(wifiName, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print("Trying to connect to Wifi Network");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;//s=vt
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  if(distanceCm<=20){//open the door if distance less than 20cm
    digitalWrite(ledPin, HIGH);
      Serial.print("Opening Door!");
  }
  else{//close the door otherwise
    digitalWrite(ledPin, LOW);
    Serial.print("Door Closed!");
  }
  
  delay(5000);//5 second delay before loop resarts
}