/*
  ATmega168, ATmega328:
  Using Timer 1 disables PWM (analogWrite) on pins 9 and 10

  ATmega2560:
  Using Timer 1 disables PWM (analogWrite) on pins 11 and 12
  Using Timer 3 disables PWM (analogWrite) on pins 2, 3 and 5
  Using Timer 4 disables PWM (analogWrite) on pins 6, 7 and 8
  Using Timer 5 disables PWM (analogWrite) on pins 44, 45 and 46
*/

void TIMER_STAR_config() {
  /////  starSpeed_us_for_microtick  18698.8043687066 us = 53.479355165272 Hz
  //F\f = 299180.8698993056
  //ocr = (299180.8698993056/N)   -1
  //N=256
  //ocr = 1168.675273044163 - 1 = 1168
  //f= 53.464499572284 Hz

  cli(); //запретить все прерывания

  //------ Timer1 ----------
  TCCR1B = (1 << WGM12);  // Режим CTC (сброс по совпадению)
  TCCR1B |= (1 << CS12);         // CLK/256
  // Если нужен предделитель :
  // TCCR1B |= (1 << CS10);         // Тактирование от CLK/1
  // TCCR1B |= (1<<CS11);           // CLK/8
  // TCCR1B |= (1<<CS10)|(1<<CS11); // CLK/64
  // TCCR1B |= (1<<CS12);           // CLK/256
  // TCCR1B |= (1<<CS10)|(1<<CS12); // CLK/1024

  OCR1A = 1168;    // 53.464499572284 Hz       // Верхняя граница счета. Диапазон от 0 до 65535.
  // Частота прерываний будет = Fclk/(N*(1+OCR1A))
  // где N - коэф. предделителя (1, 8, 64, 256 или 1024)
  TIMSK1 = (1 << OCIE1A); // Разрешить прерывание по совпадению

  sei ();
}

void TIMER_GOTO_config() {
  //goto freq = 16000000\(1*(1+999))  = 16 000 Hz
  //N=1
  //ocr =  999

  cli(); //запретить все прерывания

  //------ Timer1 ----------
  TCCR1B = (1 << WGM12);  // Режим CTC (сброс по совпадению)
  TCCR1B |= (1 << CS10);
  // Если нужен предделитель :
  // TCCR1B |= (1 << CS10);         // Тактирование от CLK/1
  // TCCR1B |= (1<<CS11);           // CLK/8
  // TCCR1B |= (1<<CS10)|(1<<CS11); // CLK/64
  // TCCR1B |= (1<<CS12);           // CLK/256
  // TCCR1B |= (1<<CS10)|(1<<CS12); // CLK/1024

  OCR1A = 999;           // Верхняя граница счета. Диапазон от 0 до 65535.
  // Частота прерываний будет = Fclk/(N*(1+OCR1A))
  // где N - коэф. предделителя (1, 8, 64, 256 или 1024)
  TIMSK1 = (1 << OCIE1A); // Разрешить прерывание по совпадению

  sei ();
}

// Обработчик прерывания таймера 1
ISR (TIMER1_COMPA_vect) {
  if (SYS_STATE == SYS_STATE_GOTO_INIT) {
    MOTOR_RA_TICK(); //star speed
  }
  else if (SYS_STATE == SYS_STATE_GOTO_READY) {
    MOTOR_RA_TICK(); //star speed
  }
  else if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    GOTO_tick();
  }
}


