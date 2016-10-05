void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();

  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_331ms) > 331L) {
    TIMEMACHINE_331ms();
    TIMEMACHINE_prevMicros_331ms = TIMEMACHINE_currMillis;
    return;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_1013ms) > 1013L) {
    TIMEMACHINE_1013ms();
    TIMEMACHINE_prevMicros_1013ms = TIMEMACHINE_currMillis;
    return;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_3313ms) > 3313L) {
    TIMEMACHINE_3313ms();
    TIMEMACHINE_prevMicros_3313ms = TIMEMACHINE_currMillis;
    return;
  }
}

void TIMEMACHINE_331ms() {
  GOTO_check_goto_is_completed();
  //button
}

void TIMEMACHINE_1013ms() {
  SERIAL_listen();
  GOTO_current_positions_send_to_stellarium();
}
void TIMEMACHINE_3313ms() {
  GOTO_calc_positions();
  GOTO_processSerialCommand();
}

