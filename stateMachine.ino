void STATEMACHINE_loop() {
  uint32_t  STATEMACHINE_currMicros = micros();

  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_gotoSpeed) > gotoSpeed_us_for_microtick) {
    GOTO_process();
    STATEMACHINE_prevMicros_gotoSpeed = STATEMACHINE_currMicros;
    return true;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_starSpeed) > starSpeed_us_for_microtick) {
    MOTOR_STARSPEED_process();
    STATEMACHINE_prevMicros_starSpeed = STATEMACHINE_currMicros;
    return true;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_331000us) > 331000L) {
    STATEMACHINE_331000us();
    STATEMACHINE_prevMicros_331000us = STATEMACHINE_currMicros;
    return true;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_591000us) > 591000L) {
    STATEMACHINE_591000us();
    STATEMACHINE_prevMicros_591000us = STATEMACHINE_currMicros;
    return true;
  }
}

void STATEMACHINE_331000us() {
  //button
}

void STATEMACHINE_591000us() {
  if (SYS_isset_new_command_from_stellarium) {
    GOTO_processSerialCommand();
    SYS_isset_new_command_from_stellarium = false;
    return true; // => SERIAL_listen in next times
  }
  SERIAL_listen();
}
