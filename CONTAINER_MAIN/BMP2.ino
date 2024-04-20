//code untuk sensor bmp
#define SEALEVELPRESSURE_HPA (1013.25)

int       apogee = 0, status_sep = 0;


void inisialisasiBME() {
  unsigned bme_status;
  bme_status = bme.begin();
  if (!bme_status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
}

void Tekanan() {
  pressure2 = bme.readPressure();
}

void hitungBMP() {
  altitude_mdpl = bme.readAltitude(SEALEVELPRESSURE_HPA);

  // kalibrasi (biar alt awal = 0
  if (altitude_mdpl != 0 && millis() < 10000) {
    alt0 = altitude_mdpl;
  }
  if (altitude_mdpl < alt0) {
    alt0 = altitude_mdpl;
  }

  // set actual altitude
  altitude1 = altitude_mdpl - alt0;

  if (altitude1 < 0) {
    altitude1 = 0;
  }
  altPrev = altitude1;
}
