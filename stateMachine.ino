void STATEMACHINE_loop() {
  uint32_t  STATEMACHINE_currMicros = micros();
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMillis_331ms) > 331L) {
    STATEMACHINE_331ms();
    STATEMACHINE_prevMillis_331ms = STATEMACHINE_currMicros;
  }
  if ((STATEMACHINE_currMicros - STATEMACHINE_prevMillis_1013ms) > 1013L) {
    STATEMACHINE_1013ms();
    STATEMACHINE_prevMillis_103ms = STATEMACHINE_currMicros;
  }

}

void STATEMACHINE_331ms() {
  SERIAL_listen();
}
void STATEMACHINE_1013ms() {

}
