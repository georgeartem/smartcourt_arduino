#include <ESP8266WiFi.h>

String value;

const int vibeInput = A0;
int LEDpin = 0;

char server[] ="www.previousk.com";

//#define WLAN_SSID "CherryStreetCoffee"
//#define WLAN_PASS "cherrylove"

#define WLAN_SSID       "Alexander's iPhone"
#define WLAN_PASS       "turoidular"

WiFiClient client;

void setup() {

pinMode('A', INPUT);
pinMode(LEDpin, OUTPUT);


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
String value;
int state;
int i = 0;

  while( i < 6000){
 
    if ( analogRead(vibeInput) > 700) {
        Serial.println('.');
        state ++;
        i++;
        delay(10);
        } 
        
        if (analogRead(vibeInput) <= 700){
        Serial.println('/');
        delay(10);
        i++;}
  }


if (state > 700){ // if the sensor has been tripped more than 4 times in a minute, send data
      // Only publish to the website if the courtstatus is occupied!
  // Now we can publish stuff!
value = "testoccup"; 
  
  Serial.print(F("\nSending sensor value: "));
  Serial.print(value);
  Serial.println("... ");

 
      //concatenates the string to be sent in the HTTP request
      String stringOne = "GET /smartcourt/api/smartcourtdata.php?courtid=";
      const int courtid =3;
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

if (state <= 100){ // if the sensor has been tripped more than 4 times in a minute, send data
      // Only publish to the website if the courtstatus is occupied!
  // Now we can publish stuff!
value = "testavail"; 
  
  Serial.print(F("\nSending sensor value: "));
  Serial.print(value);
  Serial.println("... ");

 
      //concatenates the string to be sent in the HTTP request
      String stringOne = "GET /smartcourt/api/smartcourtdata.php?courtid=";
      const int courtid =3;
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

}



