
void pid_call_orientation() {
  /*///////////////////////////P I D///////////////////////////////////*/
  //roll_desired_angle = map(input_ROLL,1000,2000,-10,10);
  //pitch_desired_angle = map(input_PITCH,1000,2000,-10,10);

  /*First calculate the error between the desired angle and
    the real measured angle*/
  roll_error = roll - roll_desired_angle;
  pitch_error = pitch - pitch_desired_angle;
  /*Next the proportional value of the PID is just a proportional constant
    multiplied by the error*/
  roll_pid_p = roll_kp * roll_error;
  pitch_pid_p = pitch_kp * pitch_error;
  /*The integral part should only act if we are close to the
    desired position but we want to fine tune the error. That's
    why I've made a if operation for an error between -2 and 2 degree.
    To integrate we just sum the previous integral value with the
    error multiplied by  the integral constant. This will integrate (increase)
    the value each loop till we reach the 0 point*/
  if (-3 < roll_error < 3)
  {
    roll_pid_i = roll_pid_i + (roll_ki * roll_error);
  }
  if (roll_error < -3 || roll_error > 3)
  {
    roll_pid_i = 0;
  }

  if (-3 < pitch_error < 3)
  {
    pitch_pid_i = pitch_pid_i + (pitch_ki * pitch_error);
  }
  if (pitch_error < -3 || pitch_error > 3)
  {
    pitch_pid_i = 0;
  }

  /*The last part is the derivate. The derivate acts upon the speed of the error.
    As we know the speed is the amount of error that produced in a certain amount of
    time divided by that time. For taht we will use a variable called previous_error.
    We substract that value from the actual error and divide all by the elapsed time.
    Finnaly we multiply the result by the derivate constant*/
  roll_pid_d = roll_kd * ((roll_error - roll_previous_error) / elapsedTimePID);
  pitch_pid_d = pitch_kd * ((pitch_error - pitch_previous_error) / elapsedTimePID);
  /*The final PID values is the sum of each of this 3 parts*/
  roll_PID = roll_pid_p + roll_pid_i + roll_pid_d;
  pitch_PID = pitch_pid_p + pitch_pid_i + pitch_pid_d;
  /*We know taht the min value of PWM signal is 1000us and the max is 2000. So that
    tells us that the PID value can/s oscilate more than -1000 and 1000 because when we
    have a value of 2000us the maximum value taht we could substract is 1000 and when
    we have a value of 1000us for the PWM signal, the maximum value that we could add is 1000
    to reach the maximum 2000us. But we don't want to act over the entire range so -+400 should be enough*/
  if (roll_PID < -300) {
    roll_PID = -300;
  }
  if (roll_PID > 300) {
    roll_PID = 300;
  }

  if (pitch_PID < -300) {
    pitch_PID = -300;
  }
  if (pitch_PID > 300) {
    pitch_PID = 300; //750
  }

  /*Finnaly we calculate the PWM width. We sum the desired throttle and the PID value*/
  pwm_Front  = 1040  - roll_PID + (altitude_PID) + velocity_PID;
  pwm_Back  = 1040  + roll_PID + (altitude_PID) + velocity_PID;
  pwm_Left  = 1040  - pitch_PID + (altitude_PID) + velocity_PID;
  pwm_Right  = 1040  + pitch_PID + (altitude_PID) + velocity_PID;

  if (pwm_Front < 1040)
  {
    pwm_Front = 1040;
  }
  if (pwm_Front > 1960)
  {
    pwm_Front = 1960;
  }

  //Back
  if (pwm_Back < 1040)
  {
    pwm_Back = 1040;
  }
  if (pwm_Back > 1960)
  {
    pwm_Back = 1960;
  }

  //Left
  if (pwm_Left < 1040)
  {
    pwm_Left = 1040;
  }
  if (pwm_Left > 1960)
  {
    pwm_Left = 1960;
  }

  //Right
  if (pwm_Right < 1040)
  {
    pwm_Right = 1040;
  }
  if (pwm_Right > 1960)
  {
    pwm_Right = 1960;
  }

  depan = map(pwm_Front, 1040, 1960, 0, 180);
  belakang = map(pwm_Back, 1040, 1960, 0, 180);
  kiri = map(pwm_Left, 1040, 1960, 0, 180);
  kanan = map(pwm_Right, 1040, 1960, 0, 180);
  /*Once again we map the PWM values to be sure that we won't pass the min
    and max values. Yes, we've already maped the PID values. But for example, for
    throttle value of 1300, if we sum the max PID value we would have 2300us and
    that will mess up the ESC.*/
  //Front

  roll_previous_error = roll_error; //Remember to store the previous error.
  pitch_previous_error = pitch_error; //Remember to store the previous error.
}

void pid_call_hovering() {
  //dipanggil jika akan melakukan hovering
  altitude_error = desired_altitude - altitudeBMP;

  altitude_pid_p = altitude_kp * altitude_error;

  if (-5 < altitude_error < 5)
  {
    altitude_pid_i = altitude_pid_i + (altitude_ki * altitude_error);
  }
  if (altitude_error < -5 || altitude_error > 5)
  {
    altitude_pid_i = 0;
  }

  altitude_pid_d = altitude_kd * ((altitude_error - altitude_previous_error) / elapsedTimePID);

  if (altitude_PID < 60) {
    altitude_PID = 60;
  }
  if (altitude_PID > 750) {
    altitude_PID = 750;
  }

  altitude_PID = altitude_pid_p + altitude_pid_i + altitude_pid_d + 360;

  altitude_previous_error = altitude_error; //Remember to store the previous error.
}

void pid_call_descend() {
  //dipanggil jika state descend
  velocity_error = velocity - desired_velocity;

  velocity_pid_p = velocity_kp * velocity_error;

  if (-1 < velocity_error < 1)
  {
    velocity_pid_i = velocity_pid_i + (velocity_ki * velocity_error);
  }
  if (velocity_error < -1 || velocity_error > 1)
  {
    velocity_pid_i = 0;
  }

  velocity_pid_d = velocity_kd * ((velocity_error - velocity_previous_error) / elapsedTimePID);

  velocity_PID = velocity_pid_p + velocity_pid_i + velocity_pid_d;

  if (velocity_PID < -500) {
    velocity_PID = -500;
  }
  if (velocity_PID > 500) {
    velocity_PID = 500;
  }

  velocity_previous_error = velocity_error;
}
