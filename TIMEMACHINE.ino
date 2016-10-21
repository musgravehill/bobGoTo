void TIMEMACHINE_loop() {
  uint32_t  TIMEMACHINE_currMillis = millis();

  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_173ms) > 173L) {
    TIMEMACHINE_173ms();
    TIMEMACHINE_prevMicros_173ms = TIMEMACHINE_currMillis;
  }
  if ((TIMEMACHINE_currMillis - TIMEMACHINE_prevMicros_873ms) > 873L) {
    TIMEMACHINE_873ms();
    TIMEMACHINE_prevMicros_873ms = TIMEMACHINE_currMillis;
  }  
}

void TIMEMACHINE_173ms() {
  BUTTON_check();    
}
void TIMEMACHINE_873ms() {    
  GOTO_check_goto_is_completed();
  GOTO_calc_positions();    
  GOTO_current_positions_send_to_stellarium();  //делай МАХ частоту отправки, Стеллариум отзывчивее и не тупит
}


