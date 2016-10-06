void BUTTON_check() {
  uint8_t state;
  state = bitRead(PIND, 6); //BUTTON_GOTO_SYNC 6 //PD6  
  if (state == 1) {
    GOTO_BUTTON_coordinates_sync_ok();    
  }  
}
void BUTTON_init() {
  pinMode(BUTTON_GOTO_SYNC,  INPUT);  
}


