static const int RXPin = 14, TXPin = 15;
static const uint32_t GPSBaud = 9600;

int jam, menit, detik, bulan, hari, tahun;
const int UTC_offset = 3;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void inisialisasiGPS() {
  ss.begin(GPSBaud);
}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void GPS() {
  lat1 = gps.location.lat();
  long1 = gps.location.lng();
  altGPS1 = gps.altitude.meters();
  jam = gps.time.hour() + UTC_offset;
  hari = gps.date.day();
  menit = gps.time.minute();
  detik = gps.time.second();
  bulan = gps.date.month();
  tahun = gps.date.year();
  if (jam >= 24) {
    jam = gps.time.hour() + UTC_offset - 24;
    hari = gps.date.day() + 1;
  }
  mission_time = (String(hari) + "/" + String(bulan) + "/" + String(tahun) + "," + String(jam) + "/" + String(menit) + "/" + String(detik));
  datagps = (String(lat1,6) + "," + String(long1,6) + "," + String(altGPS1));
  if(lat1 != 0){
    gps_stat = 10;
  }
  smartDelay(0);
}
