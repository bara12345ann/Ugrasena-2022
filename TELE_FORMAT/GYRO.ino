float yaw_ds, yaw_ds1;

void inisialisasiGYRO() {
  byte status = mpu.begin();
 delay(1000);
  mpu.calcOffsets(); // gyro and accelero
}

void hitungGYRO() {
  //Set angular payload
  roll = (mpu.getAngleX());
  pitch = (mpu.getAngleY());
  yaw = (mpu.getAngleZ());
}
void hitungSPIN() {
  if (turun == 0) {
    yaw_ds = yaw;
  }
  else if (turun == 1) {
    yaw_ds1 = yaw - yaw_ds;
    spin = yaw_ds1 / 360;
  }
}
