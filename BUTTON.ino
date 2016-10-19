void BUTTON_check() {
  uint8_t state;

  //BUTTON_GOTO_SYNC 5 //PD5
  state = bitRead(PIND, 5);
  if (state == 0) {
    GOTO_BUTTON_coordinates_sync_ok();
  }

  //GOTO ready => allow button-control
  if (SYS_STATE == SYS_STATE_GOTO_READY) {

    //BUTTON_RA_FORWARD 7 //PD7
    state = bitRead(PIND, 7);
    if (state == 0) {
      SYS_IS_RC_PROCESS = true;
      MOTOR_set_RA_dir(true);
      for (uint8_t i = 0; i < 63; i++) {
        MOTOR_RA_TICK();
        delay(4);
      }
      SYS_IS_RC_PROCESS = false;
    }

    //BUTTON_RA_BACKWARD 6 //PD6
    state = bitRead(PIND, 6);
    if (state == 0) {
      SYS_IS_RC_PROCESS = true;
      MOTOR_set_RA_dir(false);
      for (uint8_t i = 0; i < 63; i++) {
        MOTOR_RA_TICK();
        delay(4);
      }
      delay(50);
      MOTOR_set_RA_dir(true); //return normal direction for sky-correction
      delay(50);
      SYS_IS_RC_PROCESS = false;
    }

    //BUTTON_DEC_FORWARD 9 //PB1
    state = bitRead(PINB, 1);
    if (state == 0) {      
      MOTOR_set_DEC_dir(true);
      for (uint8_t i = 0; i < 63; i++) {
        MOTOR_DEC_TICK();
        delay(4);
      }      
    }

    //BUTTON_DEC_BACKWARD 8 //PB0
    state = bitRead(PINB, 0);
    if (state == 0) {      
      MOTOR_set_DEC_dir(false);
      for (uint8_t i = 0; i < 63; i++) {
        MOTOR_DEC_TICK();
        delay(4);
      }      
    }

  }
}

void BUTTON_init() {
  pinMode(BUTTON_GOTO_SYNC,  INPUT);
  pinMode(BUTTON_GOTO_SYNC_LED,  OUTPUT);

  pinMode(BUTTON_RA_FORWARD,  INPUT);
  pinMode(BUTTON_RA_BACKWARD,  INPUT);
  pinMode(BUTTON_DEC_FORWARD,  INPUT);
  pinMode(BUTTON_DEC_BACKWARD,  INPUT);

  PORTD |= _BV(PD3); //LED goto_sync ON
}


