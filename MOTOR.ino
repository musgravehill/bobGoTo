void MOTOR_init() {
  MOTOR_RA.setMaxSpeed(MOTOR_maxAllowed_ticks_per_second);
  MOTOR_RA.setAcceleration(MOTOR_acceleration); //tick per ticks (ticks on driver_step_pin)

  MOTOR_DEC.setMaxSpeed(MOTOR_maxAllowed_ticks_per_second);
  MOTOR_DEC.setAcceleration(MOTOR_acceleration); //tick per ticks (ticks on driver_step_pin)
}
