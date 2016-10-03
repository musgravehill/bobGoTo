void MOTOR_RA_TICK() {
  //DIR
  // state: GOTO OR starSpeed

  digitalWrite(DRIVER_RA_STEP,  1);
  delayMicroseconds(3); //DRV8825 needs 1.9 us
  digitalWrite(DRIVER_RA_STEP,  0);
}

void MOTOR_DEC_TICK() {
  //DIR
  // state: GOTO OR starSpeed

  digitalWrite(DRIVER_DEC_STEP,  1);
  delayMicroseconds(3); //DRV8825 needs 1.9 us
  digitalWrite(DRIVER_DEC_STEP,  0);
}
