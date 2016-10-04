void STATEMACHINE_loop() {
  uint32_t  STATEMACHINE_currMicros = micros();

  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_gotoSpeed) > gotoSpeed_us_for_microtick) {
    GOTO_tick();
    STATEMACHINE_prevMicros_gotoSpeed = STATEMACHINE_currMicros;
    return;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_starSpeed) > starSpeed_us_for_microtick) {
    MOTOR_STARSPEED_tick();
    STATEMACHINE_prevMicros_starSpeed = STATEMACHINE_currMicros;
    return;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_331000us) > 331000L) {
    STATEMACHINE_331000us();
    STATEMACHINE_prevMicros_331000us = STATEMACHINE_currMicros;
    return;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_1013000us) > 1013000L) {
    STATEMACHINE_1013000us();
    STATEMACHINE_prevMicros_1013000us = STATEMACHINE_currMicros;
    return;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMicros_3313000us) > 3313000L) {
    STATEMACHINE_3313000us();
    STATEMACHINE_prevMicros_3313000us = STATEMACHINE_currMicros;
    return;
  }
}

void STATEMACHINE_331000us() {
  //button
  //рассчет тек.позиции, если работает ГОТО, то надо знать кол-во пройденных шагов и направление. Отсюда считаем позиции.
}

void STATEMACHINE_1013000us() {
  GOTO_calc_positions();
  SERIAL_listen();
  GOTO_current_positions_send_to_stellarium();
}
void STATEMACHINE_3313000us() {
  GOTO_processSerialCommand();
}

