#include <Arduino.h>
#include <wifi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Adafruit_sensor.h>
#include <Adafruit_BME280.h>

// my wifi name and password
const char *ssid = "HASEEB";
const char *password = "65464816";

// Set web server port number to 80
AsyncWebServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output18State = "off";

// these are the GPIO pins of esp32
const int trigPin = 27; // setting the trig pin
const int echoPin = 25; // setting the echo pin
const int ledPin = 18;  // setting the ledpin
String ledState;

// Replaces placeholder with LED state value

// define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

float getDistance()
{

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}

String processor(const String &var)
{
  Serial.println(var);
  if (var == "DISTANCE")
  {
    return String(getDistance());
  }
  return String();
}

void setup()
{
  Serial.begin(9600);       // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);  // sets the ledPin as another output

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File file = SPIFFS.open("/index.html");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {if(getDistance()<20){
                digitalWrite(ledPin, HIGH); 
            }
            else{
              digitalWrite(ledPin,LOW);
            } 
            request->send(SPIFFS, "/index.html", String(), false, processor);
            });

  // // Route for root / web page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/on.html", String(), false, processor); });

  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/off.html", String(), false, processor); });

  // Start server
  server.begin();
}

void loop()
{

  //     // Clears the trigPin
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);
  // // Sets the trigPin on HIGH state for 10 micro seconds
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);

  // // Reads the echoPin, returns the sound wave travel time in microseconds
  // duration = pulseIn(echoPin, HIGH);

  // // Calculate the distance
  // distanceCm = duration * SOUND_SPEED / 2; // s=vt

  // // Convert to inches
  // distanceInch = distanceCm * CM_TO_INCH;

  // // Prints the distance in the Serial Monitor
  // Serial.print("Distance (cm): ");
  // Serial.println(distanceCm);
  // Serial.print("Distance (inch): ");
  // Serial.println(distanceInch);
  // if (distanceCm <= 20)
  // { // open the door if distance less than 20cm
  //   digitalWrite(ledPin, HIGH);
  //   Serial.print("Opening Door!");
  // }
  // else
  // { // close the door otherwise
  //   digitalWrite(ledPin, LOW);
  //   Serial.print("Door Closed!");
  // }

  // delay(5000); // 5 second delay before loop resarts
}