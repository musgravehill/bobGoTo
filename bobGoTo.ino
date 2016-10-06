#include <stdlib.h>

//Floats have only 6-7 decimal digits of precision
//you can get more precision by using a double (e.g. up to 15 digits), on the Arduino, double is the same size as float!

//max 4000 ticks per second for Arduino 16MHz, http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html

#define STARDAY_us = 86164090531L //micro second in star-day

#define DRIVER_RA_STEP 8 //PB0
#define DRIVER_RA_DIR 7 //PD7
#define DRIVER_DEC_STEP 4 //PD4
#define DRIVER_DEC_DIR 2 //PD2

#define BUTTON_GOTO_SYNC 9 //PB1

// тиков двигателя на полный оборот монти (зависит от редукции)
// 144(червь)*2.5(ремень)*400(шагов на 1 оборот ШД)* 32(микрошаг DRIVER_MICROSTEP_X=32)
#define RA_microticks_per_revolution  4608000L; //ось прямого восхождения
#define DEC_microticks_per_revolution  4608000L; //ось склонений

#define RA_max_hex_value 0xFFFFFFFFL  //Максимальное значение величины прямого восхождения
#define DEC_max_hex_value 0xFFFFFFFFL  //Максимальное значение величины склонения

//Единиц прямого восхождения за 1 микротик двигателя
// RA_max_hex_value / RA_microticks_per_revolution
#define RA_step_per_motor_microstep   932.0675553385417

//Единиц склонения за 1 микротик двигателя
// DEC_max_hex_value / DEC_microticks_per_revolution
#define DEC_step_per_motor_microstep   932.0675553385417

//= STARDAY_us/RA_microticks_for_revolution
//starSpeed_us_for_microtick  18699.***? // 53.464499572284 Hz by Timer1

//это наведение быстрее зведной скорости в
//NOTE DRV8825 max step frequency is 250kHz ~ 4us
//gotoSpeed_us_for_microtick  13998.25021872266 Hz by Timer1; recalc GOTO_plusminus_dRA_per_1_tick

//прирост RA позиции при большой скорости ГОТО.
//Если бы скоростьГОТО==скоростьЗвезд, то в догонку прирост = 0, система "застыла" на месте и ведет точку.
//А против звезд получится сложение 2х скоростей: goto+звздная
// = (gotoSpeed_us_for_microtick/starSpeed_us_for_microtick) * RA_step_per_motor_microstep = (64/18699) * 932.0675553385417
//or  (star_moto_freq/goto_motor-freq)   * 932.0675553385417
#define GOTO_plusminus_dRA_per_1_tick 3.559911034243855

int8_t RA_dRA_star_compensation_sign = 1; // 1==counter star; -1 = chase the star
int8_t RA_dRA_sign = 1;
int8_t DEC_dDEC_sign = 1;

//телескоп ВСЕГДА вращается за небом
#define SYS_STATE_GOTO_INIT 0 //в Стеллариуме ГОТО на ВЕГУ, телескоп навести Вегу и нажать кнопку пульта STELLARIUM_LINK
#define SYS_STATE_GOTO_READY 1 //телескоп привязался к Стеллариуму, можно ГОТО в Стеллариуме, телескоп исполнил приказ GOTO
#define SYS_STATE_GOTO_PROCESS 2  //телескоп исполняет приказ GOTO в процессе наведения

//TIMEMACHINE
uint32_t TIMEMACHINE_prevMicros_331ms = 1L;
uint32_t TIMEMACHINE_prevMicros_873ms = 1L;

char SYS_chars_from_stellarium[19]; //r00000000,00000000\0  19 chars with terminator \0
uint8_t SYS_chars_from_stellarium_pos = 0;

unsigned long RA_hex_position_curr = 0L;
unsigned long DEC_hex_position_curr = 0L;
unsigned long RA_hex_position_goto = 0L;
unsigned long DEC_hex_position_goto = 0L;

//телескоп вручную наводим, жмем ГОТО туда же. Коорд совпали. Жми кнопку goto_synced
uint8_t SYS_STATE = SYS_STATE_GOTO_INIT;

unsigned long RA_GOTO_count_ticks_made = 0L;
unsigned long DEC_GOTO_count_ticks_made = 0L;
unsigned long RA_GOTO_count_ticks_need = 0L;
unsigned long DEC_GOTO_count_ticks_need = 0L;

unsigned long GOTO_RA_count_ticks_made_prev = 0L;
unsigned long GOTO_DEC_count_ticks_made_prev = 0L;

////////////////////////////////////////////////////////////////////////////////
//** MOSI - pin 11
//** MISO - pin 12
//** CLK - pin 13
//** CS - pin 10
//#include <SPI.h>
//#include <SD.h>
//const int chipSelect = 10;
//SD.begin(chipSelect);
//File dataFile = SD.open("event.txt", FILE_WRITE);
/*if (dataFile) {
    uint8_t si = SYS_str_from_stellarium.length();
    dataFile.print("L=");
    dataFile.print(si, DEC);
    dataFile.print(" S=");
    dataFile.println(SYS_str_from_stellarium);
    dataFile.close();
    }*/
///////////////////////////////////////////////////////////////////////////////


void setup() {
  BUTTON_init();
  MOTOR_init();
  delay(10);
  Serial.begin(9600);
  delay(10);  
}

void loop() {
  TIMEMACHINE_loop();
}
