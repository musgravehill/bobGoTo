void MOTOR_init() {
  motor_RA.setMaxSpeed(300);
  motor_RA.setAcceleration(10); //tick per ticks (ticks on driver_step_pin)
}
