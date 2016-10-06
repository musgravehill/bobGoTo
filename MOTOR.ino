void MOTOR_RA_TICK() {
  PORTD |= _BV(PD3); //high
  delayMicroseconds(8); //DRV8825 needs 1.9 us
  PORTD &= ~_BV(PD3); //low
}

void MOTOR_DEC_TICK() {
  PORTD |= _BV(PD5); //high
  delayMicroseconds(8); //8us by OWON. DRV8825 needs 1.9 us
  PORTD &= ~_BV(PD5); //low
}

//RA true== star_speed; false == counter star rotation
void MOTOR_set_RA_dir(bool dir_forward) {
  if (dir_forward) {
    PORTD |= _BV(PD2); //high    
  } else {
    PORTD &= ~_BV(PD2); //low    
  }
}
void MOTOR_set_DEC_dir(bool dir_forward) {
  if (dir_forward) {
    PORTD |= _BV(PD4); //high
  } else {
    PORTD &= ~_BV(PD4); //low
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

  MOTOR_set_RA_dir(true); //run star-speed
  TIMER_STAR_config(); //run star-speed
}

