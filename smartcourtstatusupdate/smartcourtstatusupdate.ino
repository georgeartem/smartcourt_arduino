
#include <ESP8266WiFi.h>

const int led = 0;
const int sonarTrig = 14;  
const int sonarEcho = 13;
const int obstacle = 8;
long duration, inches, cm;
char server[] ="www.previousk.com";

#define WLAN_SSID       "strychnine"
#define WLAN_PASS       "TokyoPopSoda"

WiFiClient client;

void setup() {

pinMode(sonarTrig, OUTPUT);
pinMode(sonarEcho, INPUT); 

Serial.begin(115200);

Serial.print(F("Connecting to "));
Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void loop() {
String current = "first";
String last = "last";
String courtstatus = "";

// read if something is blocking the sonar sensor with a delay
digitalWrite(sonarTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(sonarTrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(sonarTrig, LOW);
  duration = pulseIn(sonarEcho, HIGH);

          //  inches = microsecondsToInches(duration);
          cm = microsecondsToCentimeters(duration);

      if ( cm < obstacle )
      {digitalWrite(led, HIGH);
        courtstatus = "occupied";
        }
      else
      {digitalWrite(led, LOW);
        courtstatus = "available";
        }
        
      current = courtstatus;

         if (current == last)
            return;
 
  String value = (current == LOW ? "available" : "occupied");

  // Now we can publish stuff!
  Serial.print(F("\nSending sensor value: "));
  Serial.print(value);
  Serial.print("... ");

  last = current;
   
      //concatenates the string to be sent in the HTTP request
      String stringOne = "GET /smartcourt/api/smartcourtdata.php?courtid=";
      const int courtid =1;
      String stringTwo = "&key=smartcourtdata";
      String stringThree = "&locationid=";
      const int locationid =1;
      String stringFour = "&status=";
      String stringFive = " HTTP/1.1";
      String stringFinal = stringOne + courtid + stringTwo + stringThree + locationid + stringFour + value + stringFive;
      
      //send http request and test if success
      if (client.connect( server , 80)){
      Serial.println("Connected");
      client.println(stringFinal);
      client.println("Host: www.previousk.com");
      client.println("Connection: close");
      client.println();}
        else{
          Serial.println("Connection Failed");
        }
}

//long microsecondsToInches(long microseconds)
//{return microseconds / 74 / 2;}
 
long microsecondsToCentimeters(long microseconds)
{return microseconds / 29 / 2;}
