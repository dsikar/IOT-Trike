/**
 * IOT-Trike
 *
 *  Created on: 25.01.2017
 *
 */
 
 // Changes required
 // 1. SSID and Password
 // 2. IP Address of AWS IOT-Trike server


#include <Arduino.h>

// ESP8266 includes
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;

// Servo includes
#include <Servo.h> 
Servo myservo;
Servo myservo2; 

int pos = 90; // initial pos
int pos2 = 180; // initial pos
int iStop = 1516; // 1512 value - 1.5V AA x4 
int iReverse = 1505; // 1505 value - 1.5 AA x4
int iGo = 1539; // 1521 value - 1.5 AA x4

void setup() {
  
    // ESP8266 init routines
    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    // Commented out for debugging - COMMENT IN ONCE WORKING
    //WiFiMulti.addAP("zuzunet", "t3rr1v3l!");
    // WiFiMulti.addAP("BTHub6-Z2R7", "HYp7rXgi4Hxm");
    WiFiMulti.addAP("TP-LINK_C8F400", "80552109");
    // WiFiMulti.addAP("Makerversity_2G", "mak3rv3rs1ty");
    // WiFiMulti.addAP("South London Makerspace", "portugal");
    // WiFiMulti.addAP("SKYFF71E", "ARUPSXSY");
    
    // Servo init routines
    myservo.attach(5); 
    myservo2.attach(4);
    myservo.write(pos); // set 0 pos
    // "Even" point 1512
    // up till 1500 - clockwise
    // p
    myservo2.writeMicroseconds(iStop);
    // e.g. 1510 - clockwise
    // 1550 - anticlockwise
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        
        // String httpReq = "http://52.213.86.82/date.php?analogRead=";
        String httpReq = "http://34.248.3.208/drive.php";
        // httpReq += analogRead(iMQ_4_pin);
        
        //USE_SERIAL.println(httpReq);
                
        // http.begin("http://52.209.97.70/date.php");
        http.begin(httpReq);
        
        /*
          // or
          http.begin("http://192.168.1.12/test.html");
          http.setAuthorization("user", "password");

          // or
          http.begin("http://192.168.1.12/test.html");
          http.setAuthorization("dXNlcjpwYXN3b3Jk");
         */


        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        USE_SERIAL.println("httpCode = ");
        USE_SERIAL.println(httpCode);
        USE_SERIAL.println("HTTP_CODE_OK = ");
        USE_SERIAL.println(HTTP_CODE_OK);
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
                parse(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    //USE_SERIAL.println(analogRead(0));
    delay(1000);

}

void parse(String strCmd) {
 if(strCmd == "LT") {
   USE_SERIAL.println("Left Turn");
   int iNextPos = pos - 10;
  //USE_SERIAL.println("pos before call =");
  //USE_SERIAL.println(pos);
   // for(iCurrPos; iCurrPos <= iNextPos; iCurrPos -= 1) // goes from 0 degrees to 180 degrees 
  for(pos; pos>=iNextPos; pos-=1)
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(40);                       // waits 15ms for the servo to reach the position 
  } 
  USE_SERIAL.println("pos after call =");
  //USE_SERIAL.println(pos);
 } 
 if(strCmd == "RT") {
   //USE_SERIAL.println("Right Turn");
   int iNextPos = pos + 10;
   // for(iCurrPos; iCurrPos <= iNextPos; iCurrPos -= 1) // goes from 0 degrees to 180 degrees 
  for(pos; pos<=iNextPos; pos+=1)
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(40);                       // waits 15ms for the servo to reach the position 
  }  
  //USE_SERIAL.println("pos =");
  //USE_SERIAL.println(pos);  
 }
 if(strCmd == "FW") {
   //USE_SERIAL.println("Forward");
   myservo2.writeMicroseconds(iGo);
   delay(15); 
   
 }
 if(strCmd == "RV") {
   USE_SERIAL.println("Reverse");
   myservo2.writeMicroseconds(iReverse);
   delay(15); 
   
 } 
 if(strCmd == "ST") {
   USE_SERIAL.println("Stop");
   myservo2.writeMicroseconds(iStop);
   delay(15); 
 } 
}

