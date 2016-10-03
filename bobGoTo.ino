//Floats have only 6-7 decimal digits of precision
//you can get more precision by using a double (e.g. up to 15 digits), on the Arduino, double is the same size as float!

//max 4000 ticks per second for Arduino 16MHz, http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html

#define STARDAY_us = 86164090531L; //micro second in star-day

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

//= STARDAY_us/RA_microticks_for_revolution 
#define RA_starSpeed_us_for_microtick  18699; 

//Разложим RA_starSpeed_us_for_microtick на простые множители 18699 = 3 · 23 · 271
//Берем 271us для быстрого движения, min=250us for AVR 16MH
//это наведение быстрее зведной скорости в 3 · 23 = ровно(!) 69 раз
#define RA_gotoSpeed_us_for_microtick  271; //69 * RA_starSpeed_us_for_microtick 

// тиков двигателя на полный оборот монти (зависит от редукции)
// 144(червь)*2.5(ремень)*400(шагов на 1 оборот ШД)* 32(микрошаг DRIVER_MICROSTEP_X=32)
#define RA_microticks_per_revolution  4608000L; //ось прямого восхождения
#define DEC_microticks_per_revolution  4608000L; //ось склонений

#define RA_max_hex_value 0xFFFFFFFFL;  //Максимальное значение величины прямого восхождения
#define DEC_max_hex_value 0xFFFFFFFFL;  //Максимальное значение величины склонения


//STATEMACHINE
uint32_t STATEMACHINE_prevMillis_331ms = 1L;
uint32_t STATEMACHINE_prevMillis_1013ms = 1L;

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.flush();
  delay(10);

}

void loop() {
  STATEMACHINE_loop();
}
