void saveData(){
  // now append new data file
  sensorData = SD.open("DATA.csv", FILE_WRITE);
  //Serial.println("Tersimpan");
  if (sensorData) {
    if(State < 6){
      sensorData.println(paketKirim1);
      }else if(State >= 6){
      sensorData.println(paketKirim2);
      }
    sensorData.close(); // close the file
  }

}
