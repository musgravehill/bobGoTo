#include <AccelStepper.h>



//CONFIG
uint8_t driver_RA_step = 13;
uint8_t driver_RA_dir = 12;
float speed_star = 53.3333333; //Floats have only 6-7 decimal digits of precision
//you can get more precision by using a double (e.g. up to 15 digits), on the Arduino, double is the same size as float.

//STATEMACHINE
uint32_t STATEMACHINE_prevMillis_1s = 1L;

//STEPPER MOTORS
long motor_RA_position = 0L; //+-count ticks for 1\32 microstepping + fullstep_tick * 32
long motor_DEC_position = 0L; //+-count ticks for 1\32 microstepping + fullstep_tick * 32


AccelStepper motor_RA(AccelStepper::DRIVER, driver_RA_step, driver_RA_dir);

void setup() {
  Serial.begin(9600);

  MOTOR_init();

  motor_RA.setSpeed(50);
}

void loop() {
  STATEMACHINE_loop();

  motor_RA.runSpeed(); // TODO add SYSTEM_STATE: goto | star-speed | buttons_moveTo 

}
