#include <stdlib.h>

//=======================INIT IN-OUT======================================================

#define DRIVER_RA_DIR 12 //PB4
#define DRIVER_RA_STEP 13 //PB5
#define DRIVER_DEC_DIR 10 //PB2
#define DRIVER_DEC_STEP 11 //PB3

#define BUTTON_GOTO_SYNC 5 //PD5
#define BUTTON_GOTO_SYNC_LED 3 //PD3

#define BUTTON_DEC_INVERT_DIR 4 //PD4
#define BUTTON_DEC_INVERT_DIR_LED 2 //PD2

#define BUTTON_RA_FORWARD 7 //PD7
#define BUTTON_RA_BACKWARD 6 //PD6
#define BUTTON_DEC_FORWARD 9 //PB1
#define BUTTON_DEC_BACKWARD 8 //PB0

//========================================SKY MATH=========================================

//Floats have only 6-7 decimal digits of precision
//you can get more precision by using a double (e.g. up to 15 digits), on the Arduino, double is the same size as float!

//max 4000 ticks per second for Arduino 16MHz, http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html

#define STARDAY_us = 86164090531L //micro second in star-day

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
//когда вращение ГОТО за звездами, то они убегают, RA_step_per_motor_microstep уменьшается и микрошагов мотора надо больше
//когда вращение ГОТО против звезд, то они навстречу, RA_step_per_motor_microstep увеличивается и микрошагов мотора надо меньше
// при звездной скорости на каждый шаг небо сползает на RA_step_per_motor_microstep и мотор шагает на столько же. В итоге, мы ведем за небом.
// при высокой скорости ГОТО небо успевает сдвинуться на меньшую величину, чем выше скорость ГОТО, тем меньше сдвинется небо
// если скорость ГОТО сверх-высокая, то небо не успеет сползти) И будет меняться только координата)))
// = (gotoSpeed_us_for_microtick/starSpeed_us_for_microtick) * RA_step_per_motor_microstep
//or  (star_moto_freq/goto_motor-freq) *RA_step_per_motor_microstep=  (/)  * 932.0675553385417
#define GOTO_plusminus_dRA_per_1_tick 3.559911034243855

int8_t RA_dRA_sky_offset_sign = 1;
int8_t RA_dRA_sign = 1;
int8_t DEC_dDEC_sign = 1;

//телескоп ВСЕГДА вращается за небом
#define SYS_STATE_GOTO_INIT 0 //в Стеллариуме ГОТО на ВЕГУ, телескоп навести Вегу и нажать кнопку пульта STELLARIUM_LINK
#define SYS_STATE_GOTO_READY 1 //телескоп привязался к Стеллариуму, можно ГОТО в Стеллариуме, телескоп исполнил приказ GOTO
#define SYS_STATE_GOTO_PROCESS 2  //телескоп исполняет приказ GOTO в процессе наведения

//TIMEMACHINE
uint32_t TIMEMACHINE_prevMicros_173ms = 1L;
uint32_t TIMEMACHINE_prevMicros_873ms = 1L;

char SYS_chars_from_stellarium[19]; //r00000000,00000000\0  19 chars with terminator \0
uint8_t SYS_chars_from_stellarium_pos = 0;

unsigned long RA_hex_position_curr = 0L;
unsigned long DEC_hex_position_curr = 0L;
unsigned long RA_hex_position_goto = 0L;
unsigned long DEC_hex_position_goto = 0L;

//телескоп вручную наводим, жмем ГОТО туда же. Коорд совпали. Жми кнопку goto_synced
uint8_t SYS_STATE = SYS_STATE_GOTO_INIT; //SYS_STATE_GOTO_INIT

//управляем с пульта, автоматически тикать по осям не надо
bool SYS_IS_RC_PROCESS = false;

//телескоп справо или слева - это влияет на DEC подъем-опускание, инвертируется всё
bool DEC_IS_INVERT_DIR = false;

unsigned long RA_GOTO_count_ticks_made = 0L;
unsigned long DEC_GOTO_count_ticks_made = 0L;
unsigned long RA_GOTO_count_ticks_need = 0L;
unsigned long DEC_GOTO_count_ticks_need = 0L;

//RA навелось, а DEC еще в процессе. Для компенсации вращения неба RA должно еще тикать, пока тикает DEC.
unsigned long GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_need = 0L;
unsigned long GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_made = 0L;
//когда RA закончит наводку, включим направление мотора "за звездами"
bool GOTO_skyRotationCompensation_RAstop_DECprocess_isNeedSetRaStarRotationDir = true;

//for position calculation while goto in process
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
  BUTTON_GOTO_SYNC_LED_onoff(true);
  BUTTON_DEC_INVERT_DIR_LED_onoff(DEC_IS_INVERT_DIR);
}

void loop() {
  TIMEMACHINE_loop();
}
