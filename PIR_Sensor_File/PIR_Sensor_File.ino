/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 * @author: George Artem / georgeartem (_) gmail (_) com
 * @date: 5. October 2015
 *
 * kr1 (cleft) 2006 
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * The sensor will remain on HIGH for 4 seconds while motion is present.
 * Code delays just over 4 seconds 4050 milliseconds before it takes a second reading
 */

#include <ESP8266WiFi.h>

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;         

int pirPin = 5;    //the digital pin connected to the PIR sensor's output
int ledPin = 0;

String courtstatus; //status of the tennis court to be sent via wifi

    char server[] ="www.previousk.com"; // enter the name of the server

#define WLAN_SSID       "SoDo MakerSpace" // enter the Wifi SSID
#define WLAN_PASS       "MakeS0D0" // enter the Wifi Password

WiFiClient client;

/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);


//  //connect to WiFi network
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

////////////////////////////
//LOOP
void loop(){
  int state;
  int i = 0;
  while( i < 12 ){
     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state           
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         digitalWrite(pirPin, LOW);
         i++;
         state ++;
         delay(5000);}
     if(digitalRead(pirPin) == LOW){
      courtstatus = "available";
      Serial.println("inside while loop");
      Serial.println(courtstatus);
      i++;
      delay(5000);}  
    }
    
if (state > 2){ // if the sensor has been tripped more than 4 times in a minute, send data
      // Only publish to the website if the courtstatus is occupied!
      courtstatus = "pirtest";
      Serial.print(F("\nSending sensor value: "));
      Serial.print(courtstatus);
      Serial.print("... ");
      // reset the 'state' variable to zero
   
      //concatenates the string to be sent in the HTTP request
      String stringOne = "GET /smartcourt/api/smartcourtdata.php?courtid=";
      const int courtid =2;
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
    } // end if courtstatus condition

}
       
  
