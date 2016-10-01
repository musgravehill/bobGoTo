void STATEMACHINE_loop() {
  int32_t  STATEMACHINE_currMillis = millis();

  if ((STATEMACHINE_currMillis - STATEMACHINE_prevMillis_1s) > 1000L) {
    STATEMACHINE_1s();
    STATEMACHINE_prevMillis_1s = STATEMACHINE_currMillis;
  }

}

void STATEMACHINE_1s() {
  //TEST
  long currPos = motor_RA.currentPosition();
  Serial.println(currPos);
  //END TEST
}
