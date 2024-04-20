
/*STATE LIST
   PENDING /JIKA ALT<5 METER?
   ELEVATION / LAUNCH ALT>5 METER + KECEPATAN?
   MODEL SATELLITE DESCENT (LEAVING?) / PISAH DARI ROKET (apogee)
   MODEL SATELITE DESCENT / TERBANG PAKE PARASUT
   LEAVING (?)//SEPARATE (400m)
   SCIENCE PAYLOAD DESCENT / SETELAH SEPARASI 4-6 M/S
   HOVERING / 200m selama 10 detik
   SCIENCE PAYLOAD DESCENT / setelah separasi 4-6 m/s
   RESQUE /ALT NOW DENGAN PREV < 2 METER? (diam)
*/

void stateKontol(){
  if(State == 0){
    State = 1; // BOOT, nunggu gps
  }
  else if (State == 1 && gps_stat > 0) {
    State = 2; // PENDING WAITING FOR LAUNCH
  }
  else if (State == 2  && velocity > -5) {
    State = 3; // PAYLOAD ELEVATION
  }
  else if (State == 3 && apogee == true) {
    State = 4; // PAYLOAD AND CONTAINER DESCENDING
  }
  else if (State == 4 && altitudeBMP <= 400) { 
    State = 5; // PAYLOAD CONFIRM FOR THE SEPARATION / LEAVING
  }
  else if (State == 5 && velocity > 15 && altitudeBMP <= 400) { //semisal kecepatan turun udah di titik -20m/s motor bole jalan
    State = 6; // Science payload descend
    //start motor, maka kecepatan 6-8m/s
  }
  else if(State == 6 && altitudeBMP <= 400 && altitudeBMP >= 201){
    State = 6; // Stay di active descend
  }
  else if (State == 6 && altitudeBMP <= 200 && altitudeBMP >= 190) { // atau setting timer 20s? perlu evaluasi lagi
    State = 7; // Hover
  }
  else if (State == 7 && timer_state >= timer_state2){
    State = 8; //descend 4-6m/s
  }
  else if (State == 8 && velocity > (0.5) && altitudeBMP < 20){
    State = 9; //recover
  }

  if (State == 7){
    timer_state = millis()/1000; //20
  }
  if (ones == 0 && State == 7){
    timer_state2 = (timer_state + 20); //40
    ones++;
  }
}
