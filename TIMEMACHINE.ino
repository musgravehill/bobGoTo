void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();

  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_331ms) > 331L) {
    TIMEMACHINE_331ms();
    TIMEMACHINE_prevMicros_331ms = TIMEMACHINE_currMillis;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_1013ms) > 1013L) {
    TIMEMACHINE_1013ms();
    TIMEMACHINE_prevMicros_1013ms = TIMEMACHINE_currMillis;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_1979ms) > 1979L) {
    TIMEMACHINE_1979ms();
    TIMEMACHINE_prevMicros_1979ms = TIMEMACHINE_currMillis;
  }
}

void TIMEMACHINE_331ms() {
  BUTTON_check();
  GOTO_check_goto_is_completed();  
}
void TIMEMACHINE_1013ms() {  
  GOTO_processSerialCommand();   //from prev serial
  SERIAL_listen();   //listen string, but process in next step => for serial-sync-period like delay
}
void TIMEMACHINE_1979ms() {
  GOTO_calc_positions();  
  GOTO_current_positions_send_to_stellarium();
}

