void BUTTON_check() {
  uint8_t state;

  //BUTTON_GOTO_SYNC 6 //PD6
  state = bitRead(PIND, 6);
  if (state == 1) {
    GOTO_BUTTON_coordinates_sync_ok();
  }

  //GOTO ready => allow button-control
  if (SYS_STATE == SYS_STATE_GOTO_READY) {

    //BUTTON_RA_FORWARD 7 //PD7
    state = bitRead(PIND, 7);
    if (state == 1) {
      MOTOR_set_RA_dir(true);
      for (uint8_t i = 0; i < 15; i++) {
        MOTOR_RA_TICK();
        delay(8);
      }
    }

    //BUTTON_RA_BACKWARD 8 //PB0
    state = bitRead(PINB, 0);
    if (state == 1) {
      MOTOR_set_RA_dir(false);
      for (uint8_t i = 0; i < 15; i++) {
        MOTOR_RA_TICK();
        delay(4);
      }
      MOTOR_set_RA_dir(true);
    }

    //BUTTON_DEC_FORWARD 9 //PB1
    state = bitRead(PINB, 1);
    if (state == 1) {
      MOTOR_set_DEC_dir(true);
      for (uint8_t i = 0; i < 15; i++) {
        MOTOR_DEC_TICK();
        delay(4);
      }
    }

    //BUTTON_DEC_BACKWARD 10 //PB2
    state = bitRead(PINB, 2);
    if (state == 1) {
      MOTOR_set_DEC_dir(false);
      for (uint8_t i = 0; i < 15; i++) {
        MOTOR_DEC_TICK();
        delay(4);
      }
    }

  }
}

void BUTTON_init() {
  pinMode(BUTTON_GOTO_SYNC,  INPUT);
}


