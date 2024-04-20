//Setup library yang digunakan
#include <Wire.h>
#include <SPI.h>

#include "EEPROM.h"
#include "SD.h"
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <PWMServo.h>

#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);

#include <TinyGPS++.h>

//Setup variabel global
float gps_stat;
bool apogee = false;
unsigned long timer = 0;
int TEAM_ID = 376737, State, y, alt, spin, waktu_;
int depan, kanan, kiri, belakang;
float altitudeDiff,
      batt_volt,
      pressure,
      altitudeBMP,
      temp,
      volt,
      velocity = 0,
      pitch,
      roll,
      yaw,
      alt0,
      readAlt;
float lat1, long1, altGPS1, alt_now, alt_prev;
String altitudeBMP2, pressureBMP2, latGPS2, longGPS2, alt_GPS2, separate;
float altitude2, lat2, long2, altGPS2, pressure2;
int videoTransfer = 0, gpps, ones, timer_state, timer_state2, separate_int = 0;
String dat, paketKirim1, paketKirim, paketKirim2;
String mission_time, datagps;

Adafruit_BMP3XX bmp;
unsigned long missionTime = 0,
              packetCount = 1,
              waktu = 0;
uint32_t startKirim = 0;
bool mulaiKirim = false, stopKirim = true, leave = false, vel = false, putusin = false;
const int CSpin = 4;
File sensorData;

PWMServo ESC1; //kanan
PWMServo ESC2; //belakang
PWMServo ESC3; //depan
PWMServo ESC4; //kiri


//PID SETUP VARIABLE//////////////////////////////////////////////////////
float elapsedTimePID, timePID, timePrev;
float pwm_Front, pwm_Back, pwm_Left, pwm_Right;

//////////////////////////////PID FOR ALTITUDE///////////////////////////
float altitude_PID, altitude_error, altitude_previous_error;
float altitude_pid_p = 0;
float altitude_pid_i = 0;
float altitude_pid_d = 0;

///////////////////////////////ALTITUDE PID CONSTANTS////////////////////
double altitude_kp = 10; //3.55
double altitude_ki = 0.003; //0.003
double altitude_kd = 2.05; //2.05
float desired_altitude = 0;

//////////////////////////////PID FOR VELOCITY///////////////////////////
float velocity_PID, velocity_error, velocity_previous_error;
float velocity_pid_p = 0;
float velocity_pid_i = 0;
float velocity_pid_d = 0;

///////////////////////////////VELOCITY PID CONSTANTS////////////////////
double velocity_kp = 20; //3.55
double velocity_ki = 0.003; //0.003
double velocity_kd = 1.05; //2.05
float desired_velocity = 0;
int turun_1 = 7,
    turun_2 = 5,
    recover_1 = 1, //20m - 10m
    recover_2 = 0.2;//10m - 0m

//////////////////////////////PID FOR ROLL///////////////////////////
float roll_PID, roll_error, roll_previous_error;
float roll_pid_p = 0;
float roll_pid_i = 0;
float roll_pid_d = 0;

///////////////////////////////ROLL PID CONSTANTS////////////////////
double roll_kp = 4.15; //3.55
double roll_ki = 0.003; //0.003
double roll_kd = 2.05; //2.05
float roll_desired_angle = 0;

//////////////////////////////PID FOR PITCH//////////////////////////
float pitch_PID, pitch_error, pitch_previous_error;
float pitch_pid_p = 0;
float pitch_pid_i = 0;
float pitch_pid_d = 0;
///////////////////////////////PITCH PID CONSTANTS///////////////////
double pitch_kp = 4.15; //3.55
double pitch_ki = 0.003; //0.003
double pitch_kd = 2.05; //2.05
float pitch_desired_angle = 0;
//////////////////////////////////////////////////////////////////////////////////////


void inisialisasi(),
     hitungData(),
     GPS(),
     stateKontol(),
     inisialisasiGYRO(),
     initBuzzer(),
     inisialisasiGPS(),
     descentRate(),
     hitungGYRO(),
     kalmanBarometer(),
     printData(),
     saveData(),
     inisialisasiBMP(),
     hitungBMP(),
     Temp(),
     hitungSPIN();
void  Tekanan();
void  descentRate(); //velocity
void  hitungGYRO(); //roll,pitch,yaw
void hitungSPIN(); //spin
void hitungBaterai(); //volt
void diffAltitude();
void pisah();
void paket1();
void paket2();
void hotwire();
void manual_command();
void manual_receive();
void manual_separation();

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial3.begin(9600);
  Serial2.begin(9600);
  Serial5.begin(9600);

  ESC1.attach(3, 1040, 1960);
  ESC2.attach(4, 1040, 1960);
  ESC3.attach(5, 1040, 1960);
  ESC4.attach(6, 1040, 1960);

  //ESC Calibration
  ESC1.write(30);                  // sets the Servo position according to the scaled value
  ESC2.write(30);
  ESC3.write(30);
  ESC4.write(30);
  delay(500);
  ESC1.write(0);                  // sets the Servo position according to the scaled value
  ESC2.write(0);
  ESC3.write(0);
  ESC4.write(0);
  pinMode(9, OUTPUT);

  inisialisasi();
  pinMode(BUILTIN_SDCARD, OUTPUT);
  SD.begin(BUILTIN_SDCARD);
}

void loop() {
  writeEEPROM();
  uint32_t startTime = millis();
  missionTime = millis() / 1000;

  //TIME PID/////////////////////////////////////////////////////////
  timePrev = timePID;  // the previous time is stored before the actual time read
  timePID = millis();  // actual time read
  elapsedTimePID = (timePID - timePrev) / 1000;
  ///////////////////////////////////////////////////////////////////
  //Ambil data

  //Update status
  //Separasi dll

  ////////////////////Transfer Video Status////////////////////////
  if (Serial2.read() == -1) {
    videoTransfer = 1;
  }
  else {
    videoTransfer = 0;
  }
  ///////////////////////////////
  String cok = Serial1.read();
  int hott = cok.toInt();
  /////////////////////////////

  if (hott > 10) {
    putusin = true;
  }
  if (putusin == true) {
    //pakein timer
    digitalWrite(9, HIGH);
  }

  uint32_t elapsedTime = millis() - startTime;
  mpu.update();
  hitungData();

  if (State == 6) {
    desired_velocity = turun_1;
  } else if (State == 8 && altitudeBMP >= 20) {
    desired_velocity = turun_2;
  } else if (apogee == true && altitudeBMP < 20) {
    desired_velocity = recover_2;
  }

  if (State == 6) {
    pid_call_descend();
    pid_call_orientation();
  } else if (State == 7) {
    velocity_PID = 0;
    pid_call_hovering();
    pid_call_orientation();
  } else if (State == 8) {
    pid_call_descend();
    pid_call_orientation();
  }
  //pid_call_hovering();
  //pid_call_descend();
  //pid_call_orientation();
  //////////////////////////
  //PWMServo ESC1; //kanan
  //PWMServo ESC2; //belakang
  //PWMServo ESC3; //depan
  //PWMServo ESC4; //kiri
  ESC1.write(kanan);                  // sets the Servo position according to the scaled value
  ESC2.write(belakang);
  ESC3.write(depan);
  ESC4.write(kiri);
  //////////////////////////
  stateKontol();
  hotwire();
  /*
  Serial.print("Front: ");
  Serial.print(depan);
  Serial.print("   |   ");
  Serial.print("Back: ");
  Serial.print(belakang);
  Serial.print("   |   ");
  Serial.print("Left: ");
  Serial.print(kiri);
  Serial.print("   |   ");
  Serial.print("Right: ");
  Serial.print(kanan);
  Serial.print("   |   ");
  Serial.print("velocity: ");
  Serial.print(velocity);
  Serial.print("   |   ");
  Serial.print("sumbu: ");
  Serial.println(pwm_Right);*/
  if (State == 9){
    digitalWrite(2, HIGH);
  }
  //manual_separation();
  if (millis() - startKirim > 1000) { // &&  mulaiKirim==true) {       //Send data every 1 sec
    //Send Data (gabisa?)
    //printData();
    int asu = 10;
    asu = 10 + asu;
    if (State < 6 && State > 1)  {
      Serial1.println(paketKirim1);
      Serial.println(paketKirim2);
      saveData();
      packetCount++;
    } else if (State >= 6) {
      Serial1.println(paketKirim2);
      saveData();
      packetCount++;
    }
    //Serial1.println(paketKirim2);
    startKirim = millis();
  }
}

void inisialisasi() {
  inisialisasiGYRO(); // Set range and calibration
  initBuzzer();
  inisialisasiBMP(); // Set sampling configuration
  inisialisasiGPS();
  inisialisasiVoltageSensor(); // Set pin mode
  assignEEPROM();
}

void hitungData() {
  hitungBMP(); //altitudeBMP, apogee, turun(status 0/1)
  Temp();
  Tekanan();
  descentRate(); //velocity
  hitungGYRO(); //roll,pitch,yaw
  hitungSPIN(); //spin
  hitungBaterai(); //volt
  GPS();
  diffAltitude();
  pisah();
  paket1();
  paket2();
}

void paket1() {
  paketKirim1 = (String(TEAM_ID) + "," + String(packetCount) + "," + String(mission_time) + "," + String(pressure) + "," + String("0") + "," + String(altitudeBMP) + "," + String("0") + "," + String("0") + "," + String(velocity) + "," + String(temp) + "," + String(batt_volt) + "," + String(datagps) + "," + String("0") + "," + String("0") + "," + String("0") + "," + String(State) + "," + String(pitch) + "," + String(roll) + "," + String(yaw) + "," + String(spin) + "," + String(videoTransfer));
}
void paket2() {
  paketKirim2 = (String(TEAM_ID) + "," + String(packetCount) + "," + String(mission_time) + "," + String(pressure) + "," + String(pressure2) + "," + String(altitudeBMP) + "," + String(altitude2) + "," + String(altitudeDiff) + "," + String(velocity) + "," + String(temp) + "," + String(batt_volt) + "," + String(datagps) + "," + String(lat2, 6) + "," + String(long2, 6) + "," + String(altGPS2) + "," + "1" + "," + String(pitch) + "," + String(roll) + "," + String(yaw) + "," + String(spin) + "," + String(videoTransfer));
}
