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

void MOTOR_STARSPEED_tick() {
  //goto ready => only RA rotate star speed
  if (SYS_STATE != SYS_STATE_GOTO_PROCESS ) {
    //set RA_DIR
    MOTOR_RA_TICK();
  }
}

void MOTOR_set_RA_dir() {
}

void MOTOR_init() {
  pinMode(DRIVER_RA_STEP,  OUTPUT);
  digitalWrite(DRIVER_RA_STEP, LOW);

  pinMode(DRIVER_RA_DIR,  OUTPUT);
  digitalWrite(DRIVER_RA_DIR, LOW);

  pinMode(DRIVER_DEC_STEP,  OUTPUT);
  digitalWrite(DRIVER_DEC_STEP, LOW);

  pinMode(DRIVER_DEC_DIR,  OUTPUT);
  digitalWrite(DRIVER_DEC_DIR, LOW);
}

