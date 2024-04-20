#define SEALEVELPRESSURE_HPA (1013.25)

bool satu = false;

double baroKalmanP = 2.2f,
       baroKalmanQ = 0.05f,
       baroKalmanR = 20.0f,
       baroKalmanG,
       baroKalmanX = 100.0f,
       // alternalive parameter baroKalman[5] = {1.3f, 0.06f, 10.0f, 0.0f, 150.0f};
       baroKalman[5] = {2.2f, 0.06f, 10.0f, 0.0f, 100.0f};

float initAlt,
      press,
      altPrev,
      time_prev = 0,
      time_now,
      turun = 0;

void inisialisasiBMP() {
  bmp.begin_I2C();
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_32X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_32X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
  bmp.performReading();
}
void Temp() {
  temp = bmp.temperature;
}
void Tekanan() {
  pressure = bmp.pressure;
}
void hitungBMP() {
  bmp.performReading();

  // sampling
  press = bmp.pressure;
  readAlt = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  if (readAlt > 1500) {
    readAlt = 0;
  }

  // filter
  kalmanBarometer();
  //Serial.println(millis());

  // kalibrasi (biar alt awal = 0
  if (readAlt != 0 && millis() < 10000) {
    alt0 = readAlt;
  }
  if (readAlt < alt0) {
    alt0 = readAlt;
  }

  // set actual altitude
  altitudeBMP = readAlt - alt0;


  if (altitudeBMP < 0) {
    altitudeBMP = 0;
  }

  if (altitudeBMP > 50.0 && altPrev > altitudeBMP) {
    apogee = true;
  }
  altPrev = altitudeBMP;
}


void descentRate() {
  if ((millis() - waktu) > 1000) {
    alt_now = altitudeBMP - alt_prev; //A1-A0
    velocity = alt_now / (1000 / 1000) * (-1); //(INI MASIH KECEPATAN NAIK BUKAN TURUN)
    alt_prev = altitudeBMP;
    waktu = millis();
  }
}
void diffAltitude() {
  altitudeDiff = altitude2 - altitudeBMP;
}

void kalmanBarometer() {
  //Serial.print(readAlt - alt_0); Serial.print(",");
  //prediction update
  baroKalman[0] = baroKalman[0] + baroKalman[1];

  //measurement update
  baroKalman[3] = baroKalman[0] / (baroKalman[0] + baroKalman[2]);
  baroKalman[4] = baroKalman[4] + baroKalman[3] * (readAlt - baroKalman[4]);
  baroKalman[0] = (1 - baroKalman[3]) * baroKalman[0];
  readAlt = baroKalman[4];
  //Serial.println(readAlt - alt_0);
}
