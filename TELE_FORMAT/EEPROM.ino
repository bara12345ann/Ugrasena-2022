void assignEEPROM(){
  //Read EEPROM
  packetCount     = EEPROM.read(0)<<24  |EEPROM.read(1)<<16 |EEPROM.read(2)<<8   |EEPROM.read(3);
  //State   = EEPROM.read(4)<<24  |EEPROM.read(5)<<16 |EEPROM.read(6)<<8   |EEPROM.read(7);
  //apogee          = EEPROM.read(8)<<24  |EEPROM.read(9)<<16 |EEPROM.read(10)<<8  |EEPROM.read(1); //ganti altitudeBMP
}
void writeEEPROM(){
     // Save packet count Container to EEPROM
  EEPROM.write(0, (packetCount & 0xFF000000) >> 24);
  EEPROM.write(1, (packetCount & 0x00FF0000) >> 16);
  EEPROM.write(2, (packetCount & 0x0000FF00) >> 8);
  EEPROM.write(3,  packetCount & 0x000000FF);
  /*
      // Save Software State to EEPROM
  EEPROM.write(4, (State & 0xFF000000) >> 24);
  EEPROM.write(5, (State & 0x00FF0000) >> 16);
  EEPROM.write(6, (State & 0x0000FF00) >> 8);
  EEPROM.write(7,  State & 0x000000FF);
       // Save Altitude referance to EEPROM
  EEPROM.write(8, (apogee & 0xFF000000) >> 24);
  EEPROM.write(9, (apogee & 0x00FF0000) >> 16);
  EEPROM.write(10, (apogee & 0x0000FF00) >> 8);
  EEPROM.write(11,  (apogee & 0x000000FF));
  //print_message("Write to EEPROM");*/
}
