void STATEMACHINE_loop() {
  uint32_t  STATEMACHINE_currMillis = millis();
  if ((STATEMACHINE_currMillis - STATEMACHINE_prevMillis_250ms) > 250L) {
    STATEMACHINE_250ms();
    STATEMACHINE_prevMillis_250ms = STATEMACHINE_currMillis;
  }
  if ((STATEMACHINE_currMillis - STATEMACHINE_prevMillis_1s) > 1000L) {
    STATEMACHINE_1s();
    STATEMACHINE_prevMillis_1s = STATEMACHINE_currMillis;
  }

}

void STATEMACHINE_250ms() {
  SERIAL_listen();
}
void STATEMACHINE_1s() {

}
