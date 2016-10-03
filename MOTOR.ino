void MOTOR_RA_TICK() {
  digitalWrite(DRIVER_RA_STEP,  1);
  delayMicroseconds(3); //DRV8825 needs 1.9 us
  digitalWrite(DRIVER_RA_STEP,  0);
}

void MOTOR_RA_RUN_STARSPEED() {
  delayMicroseconds(us)
}
