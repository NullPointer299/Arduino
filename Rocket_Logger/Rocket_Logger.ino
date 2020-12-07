//GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial mySerial(8, 1); // RX, TX　8,9,10,11対応
double DESTINATION_LAT;
double DESTINATION_LNG;
unsigned long runTime;
String hour;
String minute;
String second;

//acceleration
#include <Wire.h>                 // Must include Wire library for I2C
#include "SparkFun_MMA8452Q.h"    // Click here to get the library: http://librarymanager/All#SparkFun_MMA8452Q
MMA8452Q accel;                   // create instance of the MMA8452 class

//SDcard
#include <SPI.h>
#include <SD.h>
File geoLog;
File accLog;
File myFile;

int led = 13;

void setup() {
  // put your setup code here, to run once:
  digitalWrite(led, HIGH);
  boolean flag = true;

  //acceleration init
  Wire.begin();
  accel.begin();
  accel.init(SCALE_8G, ODR_12);

  //SD init
  while (flag) {
    if (SD.begin(10)) {
      flag = false;
    }
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(2000);
  }

  //GPS init
  mySerial.begin(9600);
  while (!mySerial.available()) {
    ; // wait for SoftwareSerial port to connect.
  }

  flag = true;
  while (flag) { //現在地取得
    if (mySerial.available() > 0) {
      char c = mySerial.read();
      gps.encode(c);

      if (gps.location.isUpdated()) {
        DESTINATION_LAT = gps.location.lat();
        DESTINATION_LNG = gps.location.lng();
        if (gps.time.hour() + 9 >= 24) {
          hour = hour = gps.time.hour() - 15;
        } else {
          hour = gps.time.hour() + 9;
        }
        minute = gps.time.minute();
        second = gps.time.second();
        flag = false;
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  runTime = millis();

  while (mySerial.available() > 0) {
    char c = mySerial.read();
    //Serial.print(c);
    gps.encode(c);
    if (gps.location.isUpdated()) {
      geoLog = SD.open("GeoLog.txt", FILE_WRITE);
      digitalWrite(led, HIGH);
      if (gps.time.hour() + 9 >= 24) {
        hour = hour = gps.time.hour() - 15;
      } else {
        hour = gps.time.hour() + 9;
      }
      minute = gps.time.minute();
      second = gps.time.second();

      double distanceKm = gps.distanceBetween(
                            gps.location.lat(),
                            gps.location.lng(),
                            DESTINATION_LAT,
                            DESTINATION_LNG
                          ) / 1000.0;
      double courseTo = gps.courseTo(
                          gps.location.lat(),
                          gps.location.lng(),
                          DESTINATION_LAT,
                          DESTINATION_LNG
                        );
      geoLog.print(runTime); geoLog.print("\t");
      geoLog.print(hour + ":" + minute + ":" + second); geoLog.print("\t");
      geoLog.print(gps.location.lat(), 6); geoLog.print("\t");
      geoLog.print(gps.location.lng(), 6); geoLog.print("\t");
      geoLog.print(gps.altitude.meters()); geoLog.print("\t");
      geoLog.print(gps.course.value()); geoLog.print("\t");
      geoLog.print(gps.speed.mps()); geoLog.print("\t");
      geoLog.print(distanceKm); geoLog.print("\t");
      geoLog.print(courseTo); geoLog.print(gps.cardinal(courseTo));
      geoLog.println();

      geoLog.close();
      digitalWrite(led, LOW);
    }//if end
  }//while end

  if (accel.available()) {      // Wait for new data from accelerometer
    // Acceleration of x, y, and z directions in g units
    accLog = SD.open("accLog.txt", FILE_WRITE);
    digitalWrite(led, HIGH);

    accLog.print(runTime);
    accLog.print("\t");
    accLog.print(hour + ":" + minute + ":" + second); accLog.print("\t");
    accLog.print("\t");
    accLog.print(accel.getCalculatedX(), 3);
    accLog.print("\t");
    accLog.print(accel.getCalculatedY(), 3);
    accLog.print("\t");
    accLog.print(accel.getCalculatedZ(), 3);
    accLog.println();

    accLog.close();
    digitalWrite(led, LOW);
  }//if end

}//loop end
