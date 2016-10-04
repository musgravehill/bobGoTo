#include <stdlib.h>

//Floats have only 6-7 decimal digits of precision
//you can get more precision by using a double (e.g. up to 15 digits), on the Arduino, double is the same size as float!

//max 4000 ticks per second for Arduino 16MHz, http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html

#define STARDAY_us = 86164090531L //micro second in star-day

#define DRIVER_RA_STEP 12
#define DRIVER_RA_DIR 13
#define DRIVER_DEC_STEP 7
#define DRIVER_DEC_DIR 8

//Единиц прямого восхождения за 1 микротик двигателя
// RA_max_hex_value / RA_microticks_per_revolution
#define RA_step_per_motor_microstep   932.0675553385417

//Единиц склонения за 1 микротик двигателя
// DEC_max_hex_value / DEC_microticks_per_revolution
#define DEC_step_per_motor_microstep   932.0675553385417

//ошибка наведения, когда уже можно успокоиться и прекратить наведение
#define GOTO_position_error_allow 4000 // > 4*DEC_step_per_motor_microstep, > 4*RA_step_per_motor_microstep 

//= STARDAY_us/RA_microticks_for_revolution
#define starSpeed_us_for_microtick  18699

//Разложим RA_starSpeed_us_for_microtick на простые множители 18699 = 3 · 23 · 271
//Берем 271us для быстрого движения, min=250us for AVR 16MH
//это наведение быстрее зведной скорости в 3 · 23 = ровно(!) 69 раз
#define gotoSpeed_us_for_microtick  271 //69 * RA_starSpeed_us_for_microtick 

//прирост RA позиции при большой скорости ГОТО.
//Если бы скоростьГОТО==скоростьЗвезд, то в догонку прирост = 0, система "застыла" на месте и ведет точку.
//А против звезд получится сложение 2х скоростей: goto+звздная
#define dRA_pos_by_goto_proStar 68  // (starSpeed_us_for_microtick / gotoSpeed_us_for_microtick)  - 1
#define dRA_pos_by_goto_contraStar 70  // (starSpeed_us_for_microtick / gotoSpeed_us_for_microtick)  + 1

//прирост RA при ГОТО
// RA_pos = RA_pos + k_direction* RA_MAX_VALUE * dRA_pos_by_goto * count_goto_microsteps,
//  k_direction = 1 OR -1

// тиков двигателя на полный оборот монти (зависит от редукции)
// 144(червь)*2.5(ремень)*400(шагов на 1 оборот ШД)* 32(микрошаг DRIVER_MICROSTEP_X=32)
#define RA_microticks_per_revolution  4608000L; //ось прямого восхождения
#define DEC_microticks_per_revolution  4608000L; //ось склонений

#define RA_max_hex_value 0xFFFFFFFFL  //Максимальное значение величины прямого восхождения
#define DEC_max_hex_value 0xFFFFFFFFL  //Максимальное значение величины склонения

//телескоп ВСЕГДА вращается за небом
#define SYS_STATE_GOTO_INIT 0 //в Стеллариуме ГОТО на ВЕГУ, телескоп навести Вегу и нажать кнопку пульта STELLARIUM_LINK
#define SYS_STATE_GOTO_READY 1 //телескоп привязался к Стеллариуму, можно ГОТО в Стеллариуме, телескоп исполнил приказ GOTO
#define SYS_STATE_GOTO_PROCESS 2  //телескоп исполняет приказ GOTO в процессе наведения

//STATEMACHINE
uint32_t STATEMACHINE_prevMicros_starSpeed = 1L;
uint32_t STATEMACHINE_prevMicros_gotoSpeed = 1L;
uint32_t STATEMACHINE_prevMicros_331000us = 1L;
uint32_t STATEMACHINE_prevMicros_1013000us = 1L;
uint32_t STATEMACHINE_prevMicros_3313000us = 1L;

String SYS_str_from_stellarium = "";

unsigned long RA_nextstar_position_curr = 0L;
unsigned long DEC_nextstar_position_curr = 0L;
unsigned long RA_nextstar_position_goto = 0L;
unsigned long DEC_nextstar_position_goto = 0L;

uint8_t SYS_STATE = SYS_STATE_GOTO_INIT;

unsigned long RA_GOTO_count_ticks_made = 0L;
unsigned long DEC_GOTO_count_ticks_made = 0L;
unsigned long RA_GOTO_count_ticks_need = 0L;
unsigned long DEC_GOTO_count_ticks_need = 0L;
bool RA_GOTO_dir = true; //true, false
bool DEC_GOTO_dir = true; //true, false

void setup() {
  MOTOR_init();
  Serial.begin(9600);
  delay(10);
}

void loop() {
  STATEMACHINE_loop();
}
