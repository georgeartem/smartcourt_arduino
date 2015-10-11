#include <ESP8266WiFi.h>

String value;

int vibeInput = 5;

int vibeState = 0;

char server[] ="www.previousk.com";

#define WLAN_SSID       "SoDo MakerLab"
#define WLAN_PASS       "MakeSomething"

WiFiClient client;

void setup() {

pinMode(vibeInput, INPUT);

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
int i;
String value = "";
  while( i < 600){
 
  vibeState = digitalRead(vibeInput);
    if (vibeState == HIGH) {
        value = "occupied";
        Serial.println(value);
        digitalWrite(vibeInput, LOW);
        delay(100);
        i++;
    } if (vibeState == LOW){
        value = "available";
        Serial.println(value);
        delay(100);
        i++;
    }
  }

  // Now we can publish stuff!
  Serial.print(F("\nSending sensor value: "));
  Serial.print(value);
  Serial.println("... ");

 
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



