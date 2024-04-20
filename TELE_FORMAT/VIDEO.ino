
/*String dt[5], pesan;
  bool pemisah = false;

  void pisah(){
  //pressure2, altitude2, gps2_lat, gps2_long, gps2_alt

    if(Serial1.available() > 0){
    pesan = Serial1.read();
    pemisah = true;
  }
  Serial.print(pesan);


  if(pemisah){
    int mbuh = 0;
   for (int i = 0; i < 6; i++){
      dt[i] = "";
    }
    for (int i = 0; i < pesan.length(); i++){

      if(pesan[i] == ','){
        mbuh++;
        dt[mbuh] = "";
      }
      else{
        dt[mbuh] += pesan[i];
      }
      pemisah = false;
    }
    pressureBMP2 = dt[0];
    altitudeBMP2 = dt[1];
    latGPS2 = dt[2];
    longGPS2 = dt[3];
    alt_GPS2 = dt[4];

    altitude2 = altitudeBMP2.toFloat();
    pressure2 = pressureBMP2.toFloat();
    lat2 = latGPS2.toFloat();
    long2 = longGPS2.toFloat();
    altGPS2 = alt_GPS2.toFloat();

    Serial.print(altitude2);
    Serial.print(", ");
    Serial.print(pressure2);
    Serial.print(", ");
    Serial.print(lat2);
    Serial.print(", ");
    Serial.print(long2);
    Serial.print(", ");
    Serial.print(altGPS2);
    Serial.println("");

  }}*/
String dataIn = "", dataIn_command = "";
String dt[10], dt_command[10];
int i, zz;
boolean parsing = false, copot = false;


void pisah() {
    if(Serial5.available()>0) {
    char inChar = (char)Serial5.read();
    dataIn += inChar;
    if (inChar == '\n') {
    parsing = true;
  }else{
   
  }
}
  

  if (parsing) {
    parsingData();
    parsing = false;
    dataIn = "";
  }
}
void parsingData() {
  int j = 0;

  //kirim data yang telah diterima sebelumnya
  //Serial.print("data masuk : ");
  //Serial.print(dataIn);

  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  
  for (i = 1; i < 50; i++) {
     
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = "";     //inisialisasi variabel array dt[j]
      
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[i];
      //Serial.println("KONTOL");
    }
  }
  pressureBMP2 = dt[0];
  altitudeBMP2 = dt[1];
  latGPS2 = dt[2];
  longGPS2 = dt[3];
  alt_GPS2 = dt[4];

  altitude2 = altitudeBMP2.toFloat();
  pressure2 = pressureBMP2.toFloat();
  lat2 = latGPS2.toFloat();
  long2 = longGPS2.toFloat();
  altGPS2 = alt_GPS2.toFloat();
  /*
    //kirim data hasil parsing
    Serial.print("data 1 : ");
    Serial.println(dt[0]);
    Serial.print("data 2 : ");
    Serial.println(dt[1]);
    Serial.print("data 3 : ");
    Serial.println(dt[2]);
    Serial.print("data 4 : ");
    Serial.println(dt[3]);
    Serial.print("\n\n");*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
