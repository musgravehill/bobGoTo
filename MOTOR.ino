void MOTOR_RA_TICK() {
  digitalWrite(DRIVER_RA_STEP,  1);
  delayMicroseconds(3); //DRV8825 needs 1.9 us
  digitalWrite(DRIVER_RA_STEP,  0);
  delayMicroseconds(3); //DRV8825 needs 1.9 us
}

void MOTOR_DEC_TICK() {
  //DIR
  // state: GOTO OR starSpeed

  digitalWrite(DRIVER_DEC_STEP,  1);
  delayMicroseconds(3); //DRV8825 needs 1.9 us
  digitalWrite(DRIVER_DEC_STEP,  0);
  delayMicroseconds(3); //DRV8825 needs 1.9 us
}

void MOTOR_STARSPEED_process() {
  //goto ready => only RA rotate star speed
  if (SYS_STATE == SYS_STATE_GOTO_READY) {
    //set RA_DIR
    MOTOR_RA_TICK();
  }
}
