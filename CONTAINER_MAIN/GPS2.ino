static const int RXPin = 14, TXPin = 15;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;


void inisialisasiGPS() {
  Serial3.begin(GPSBaud);
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial3.available())
      gps.encode(Serial3.read());
  } while (millis() - start < ms);
}

void GPS() {
  gps2_lat = gps.location.lat();
  gps2_long = gps.location.lng();
  gps2_alt = gps.altitude.meters();
  smartDelay(0);
}
