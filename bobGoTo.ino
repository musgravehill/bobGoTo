#include <AccelStepper.h>

const unsigned long starDay_ms = 86164091L;

#define DRIVER_RA_STEP 12
#define DRIVER_RA_DIR 13
#define DRIVER_DEC_STEP 7
#define DRIVER_DEC_DIR 8

#define MOTOR_maxAllowed_ticks_per_second 530
#define MOTOR_acceleration 10

// тиков двигателя на полный оборот монти (зависит от редукции)
// 144(червь)*2.5(ремень)*400(шагов на 1 оборот ШД)* 32(микрошаг)
const unsigned long RA_ticks_per_revolution = 4608000L; //ось прямого восхождения
const unsigned long DEC_ticks_per_revolution = 4608000L; //ось склонений

const unsigned long RA_max_hex_value = 0xFFFFFFFFL;  //Максимальное значение величины прямого восхождения
const unsigned long DEC_max_hex_value = 0xFFFFFFFFL;  //Максимальное значение величины склонения

const unsigned long RA_step_per_motor_step = RA_max_hex_value / RA_ticks_per_revolution; //Единиц прямого восхождения за 1 тик двигателя
const unsigned long DEC_step_per_motor_step = DEC_max_hex_value / DEC_ticks_per_revolution; //Единиц склонения за 1 тик двигателя

//Floats have only 6-7 decimal digits of precision
//you can get more precision by using a double (e.g. up to 15 digits), on the Arduino, double is the same size as float!
const float RA_starSpeed_ticks_per_second = 53.47935487417839; //= RA_ticks_for_revolution \ (starDay_ms\1000) ;

//STATEMACHINE
uint32_t STATEMACHINE_prevMillis_250ms = 1L;
uint32_t STATEMACHINE_prevMillis_1s = 1L;

AccelStepper MOTOR_RA(AccelStepper::DRIVER, DRIVER_RA_STEP, DRIVER_RA_DIR);
AccelStepper MOTOR_DEC(AccelStepper::DRIVER, DRIVER_DEC_STEP, DRIVER_DEC_DIR);

void setup() {
  Serial.begin(9600);
  MOTOR_init();
}

void loop() {
  STATEMACHINE_loop();
}
