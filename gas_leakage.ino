#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "iotgasleak-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "2as8yvzXXCNksphHOZEwpU0DCC64Gyu2TgpxXRzC"

#define BLYNK_TEMPLATE_ID "TMPLRKeXSJC9"
#define BLYNK_DEVICE_NAME "IOT Course Work"
#define BLYNK_AUTH_TOKEN "8WyamwHHQ14kOFrdZ_Rmf31FQmJPVa9B"

// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "Sachintha";
char pass[] = "sachintha";

// define the GPIO connected with Sensors & LEDs
#define MQ2_SENSOR    A0 //A0
#define GREEN_LED     14 //D5
#define RED_LED       13 //D7
#define WIFI_LED      16 //D0

int MQ2_SENSOR_Value = 0;
bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;

#define VPIN_BUTTON_1    V1
#define VPIN_BUTTON_2    V2

BlynkTimer timer;

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  getSensorData();
  isconnected = Blynk.connected();
  if (isconnected == true) {
    digitalWrite(WIFI_LED, LOW);
    sendSensorData();
    //Serial.println("Blynk Connected");
  }
  else{
    digitalWrite(WIFI_LED, HIGH);
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
  MQ2_SENSOR_Value = map(analogRead(MQ2_SENSOR), 0, 1024, 0, 100);
  //  Serial.println(analogRead(MQ2_SENSOR));
  if (MQ2_SENSOR_Value > 3 ) {
    //    Serial.println(MQ2_SENSOR_Value);
    Firebase.setFloat("number", 4);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  else {
    Firebase.setFloat("number", 3);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
}

void sendSensorData()
{
  Blynk.virtualWrite(VPIN_BUTTON_1, MQ2_SENSOR_Value);
  if (MQ2_SENSOR_Value > 50 )
  {
    Blynk.logEvent("gas", "Gas Detected!");
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(MQ2_SENSOR, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  //  digitalWrite(GREEN_LED, LOW);
  //  digitalWrite(RED_LED, LOW);
  //  digitalWrite(WIFI_LED, HIGH);
  //
  WiFi.begin(ssid, pass);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
    Blynk.config(auth);
  delay(1000);
}

void loop()
{
    Blynk.run();
    timer.run();
}
