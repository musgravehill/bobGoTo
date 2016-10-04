void MOTOR_RA_TICK() {
  PORTB |= _BV(PB0); //high
  delayMicroseconds(9); //DRV8825 needs 1.9 us
  PORTB &= ~_BV(PB0); //low
  delayMicroseconds(9); //DRV8825 needs 1.9 us
}

void MOTOR_DEC_TICK() {
  PORTD |= _BV(PD4); //high
  delayMicroseconds(9); //8us by OWON. DRV8825 needs 1.9 us
  PORTD &= ~_BV(PD4); //low
  delayMicroseconds(9); //DRV8825 needs 1.9 us
}

void MOTOR_STARSPEED_tick() {
  //goto ready => only RA rotate star speed
  if (SYS_STATE != SYS_STATE_GOTO_PROCESS ) {    
    MOTOR_RA_TICK();
  }
}

void MOTOR_set_RA_dir(bool dir_forward) {
  if (dir_forward) {
    PORTD |= _BV(PD7); //high
  } else {
    PORTD &= ~_BV(PD7); //low
  }
}
void MOTOR_set_DEC_dir(bool dir_forward) {
  if (dir_forward) {
    PORTD |= _BV(PD2); //high
  } else {
    PORTD &= ~_BV(PD2); //low
  }
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

