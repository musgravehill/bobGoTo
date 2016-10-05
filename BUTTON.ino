void BUTTON_check() {
  uint8_t state;
  state = bitRead(PINB, 1); //BUTTON_GOTO_SYNC 9 //PB1  
  if (state == 1) {
    GOTO_BUTTON_coordinates_sync_ok();    
  }
  
}
void BUTTON_init() {
  pinMode(BUTTON_GOTO_SYNC,  INPUT);  
}


