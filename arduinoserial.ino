
//////////////////////////////////////////////
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define RX_PIN 3  // Arduino Pin connected to the TX of the GPS module
#define TX_PIN 2
static const int RXPin = 3, TXPin =2 ;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

SoftwareSerial mySerial(7, 8 ); // RX, TX
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);



  // Double-check the "pulseSensor" object was created and began seeing a signal
 
   
}

void loop(){
  ss.begin(GPSBaud);
 if (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
       
      Serial.print(gps.location.lat(), 6);
      Serial.print(","); 
      Serial.println(gps.location.lng(), 6);
      
      float latt = gps.location.lat();
      
      float longg = gps.location.lng();
      ss.end() ;
      
      mySerial.begin(9600);
      mySerial.print(latt, 6);
      mySerial.print(","); 
      mySerial.print(longg, 6);
      mySerial.print("/"); 
      mySerial.end() ;

     
      
    
    }
  }
 
}
