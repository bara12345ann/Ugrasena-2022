
//bmp code for pressure2 and altitude2
//use this as main program, separate the gps and bmp

#include <Wire.h>
#include "SerialTransfer.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <TinyGPS++.h>
#include <HardwareSerial.h>

Adafruit_BME280 bme;

//Setup variabel global
unsigned long missionTime = 0;

uint32_t startKirim = 0;
int buzzerPin = 4;
bool mulaiKirim = false, stopKirim = true;
float gps2_lat, gps2_long, gps2_alt;
float altPrev,
      altitudeBMP2,
      initAlt,
      temp,
      readAlt,
      alt0,
      alt_now,
      alt_prev = 0,
      time_prev = 0,
      time_now,
      turun = 0,
      bah = 1;
float altitude_mdpl, altitude1,pressure2;
String datacontainer;

void inisialisasi(),
     hitungData(),
     initBuzzer(),
     GPS(),
     inisialisasiBME(), // Set sampling configuration
     inisialisasiGPS();

SerialTransfer myTransfer;
struct STRUCT{
  String altitude2;
  String gps2;
  String pressure3;
}data;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  inisialisasi();
  pinMode(4, OUTPUT);
}

void loop() {

  uint32_t startTime = millis();
  missionTime = millis();

  uint32_t elapsedTime = millis() - startTime;
  hitungData();
  kirimcontainer();
  if (millis() - startKirim > 1000) { // &&  mulaiKirim==true) {       //Send data every 1 sec
    //Send Data (gabisa?)
    Serial.println(datacontainer);
    Serial1.println(datacontainer);
    
    startKirim = millis();
  }

}

void inisialisasi() {
  inisialisasiBME(); // Set sampling configuration
  inisialisasiGPS();
  initBuzzer();
}

void hitungData() {
  hitungBMP(); //altitudeBMP2, apogee, turun(status 0/1)
  Tekanan(); //pressure2
  GPS(); //latlongalt2
}
void kirimcontainer() {
  datacontainer = ("*" + String(pressure2) + "," + String(altitudeBMP2) + "," + String(gps2_lat,6) + "," + String(gps2_long,6) + "," + String(gps2_alt) + "#");
}
