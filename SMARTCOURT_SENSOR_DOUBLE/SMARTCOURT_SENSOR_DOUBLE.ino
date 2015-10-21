/////////////////////////////////////////////////////////////////////////////
// @author: SMART COURT LLC 
// @author: George Artem / artem (_) previousk (_) com
// @date: 13. October 2015
//  
// released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" 
// license http://creativecommons.org/licenses/by-nc-sa/2.0/de/
/////////////////////////////////////////////////////////////////////////////

// Include the ESP8266 WiFi library. (Works a lot like the
// Arduino WiFi library.)
#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
#define WLAN_SSID       "SoDo MakerSpace" // enter the Wifi SSID
#define WLAN_PASS       "MakeS0D0" // enter the Wifi Password

WiFiClient client;

char server[] ="www.previousk.com"; // enter the name of the server to which
//you will be posting a GET request


///////////////////////////////////////////////////////////////////////
//VARS

String courtstatus; //status of the tennis court to be sent via wifi

int sleepTimeS = 3600;

int calibrationTime = 30;         

int pirPin = 5;    //the digital pin connected to the PIR sensor's output
int vibeInput = A0; //the analog pin for the vibration sensors in parallel
int ledpin = 0;

////////////////////////////////////////////////////////////////////////
/// SETUP
void setup() {
pinMode(vibeInput, INPUT);
pinMode(ledpin, OUTPUT);
pinMode(pirPin, INPUT);
digitalWrite(pirPin, LOW);

Serial.begin(115200);

//give the sensors some time to calibrate
  Serial.print("calibrating sensors ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);

//connect to WiFi network
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
////////////////////////////////////////////////////////////////
//From PHANT Deep Sleep Function to test later
//  ESP.deepSleep(sleepTimeS * 1E6);
}


////////////////////////////////////////////////////////////////
//LOOP
void loop() {
  int pirstate;
  int vibstate;
  int i= 0;
  int j= 0;
   while( i < 12 ){
     //read the PIR motion sensor 
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledpin, HIGH);   //the led visualizes the sensors output pin state           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         digitalWrite(pirPin, LOW);
         i++;
         pirstate ++;
         delay(5000);}
     
     if(digitalRead(pirPin) == LOW){
      courtstatus = "available";
      Serial.println("inside while loop");
      Serial.println(courtstatus);
      i++;
      delay(5000);}  
   }

  while( j < 6000){
        // read the vibration sensor
    if ( analogRead(vibeInput) >= 500) {
        Serial.println('.');
        vibstate ++;
        j++;
        delay(10);} 
        
        if (analogRead(vibeInput) <= 500){
        Serial.println('/');
        j++;
        delay(10);}
    }

////////////////////////////////////////////////////////////////////
// if threshold conditions for both sensors are met
// send data to the server

  if(vibstate > 80 && pirstate > 2)
    {
      courtstatus = "doubletest";
      
      Serial.print(F("\nSending sensor value: "));
      Serial.print(courtstatus);
      Serial.println("... ");

      //concatenates the string to be sent in the HTTP request
      String stringOne = "GET /smartcourt/api/smartcourtdata.php?courtid=";
      const int courtid =3;
      String stringTwo = "&key=smartcourtdata";
      String stringThree = "&locationid=";
      const int locationid =1;
      String stringFour = "&status=";
      String stringFive = " HTTP/1.1";
      String stringFinal = stringOne + courtid + stringTwo + stringThree + locationid + stringFour + courtstatus + stringFive;
      
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
////////////////////////////////////////////////////////////////////
// if threshold conditions for either sensors are not met
// send data to the server with available reading

  if(vibstate <= 80 || pirstate <= 2)
    {
      courtstatus = "availtest";
      
      Serial.print(F("\nSending sensor value: "));
      Serial.print(courtstatus);
      Serial.println("... ");

      //concatenates the string to be sent in the HTTP request
      String stringOne = "GET /smartcourt/api/smartcourtdata.php?courtid=";
      const int courtid =3;
      String stringTwo = "&key=smartcourtdata";
      String stringThree = "&locationid=";
      const int locationid =1;
      String stringFour = "&status=";
      String stringFive = " HTTP/1.1";
      String stringFinal = stringOne + courtid + stringTwo + stringThree + locationid + stringFour + courtstatus + stringFive;
      
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
