void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();

  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_331ms) > 331L) {
    TIMEMACHINE_331ms();
    TIMEMACHINE_prevMicros_331ms = TIMEMACHINE_currMillis;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_873ms) > 873L) {
    TIMEMACHINE_873ms();
    TIMEMACHINE_prevMicros_873ms = TIMEMACHINE_currMillis;
  }  
}

void TIMEMACHINE_331ms() {
  BUTTON_check();  
  GOTO_calc_positions();  
}
void TIMEMACHINE_873ms() {    
  GOTO_calc_positions();    
  GOTO_current_positions_send_to_stellarium();  //делай МАХ частоту отправки, Стеллариум отзывчивее и не тупит
}


