void GOTO_processSerialCommand() {
  if ('r' == SYS_str_from_stellarium.charAt(0)) {
    
    char command_goto_RA_hex[9]; //one more element than your initialization is required, to hold the required null character.
    char command_goto_DEC_hex[9];  //one more element than your initialization is required, to hold the required null character.

    uint8_t i;
    i = SYS_str_from_stellarium.indexOf(',');
    (SYS_str_from_stellarium.substring(1, i)).toCharArray(command_goto_RA_hex, 9);
    (SYS_str_from_stellarium.substring(i + 1)).toCharArray(command_goto_DEC_hex, 9);

    RA_nextstar_position_goto =  strtoul(command_goto_RA_hex, NULL, 16);
    DEC_nextstar_position_goto =  strtoul(command_goto_DEC_hex, NULL, 16);

    /*/r12345678,ABCDEF78
      Serial.print("rr");
      Serial.print(RA_nextstar_position_goto, HEX);
      Serial.print(',');
      Serial.println(DEC_nextstar_position_goto, HEX);
    */

    // TODO нажали кнопку синхры, выставили телескоп на Вегу, в Стеллариуме сказали goto на Вегу, то
    RA_nextstar_position_curr =  RA_nextstar_position_goto;
    DEC_nextstar_position_curr = DEC_nextstar_position_goto;

    Serial.println("#");
  }
  SYS_str_from_stellarium = "";
}

void GOTO_nextstar_position_curr_send_to_stellarium() {
  Serial.print(prependZeroTo8Digits(RA_nextstar_position_curr)); //допишем 000 спереди
  Serial.print(RA_nextstar_position_curr, HEX); //позиция в HEX, а перед ней дописали нули, чтобы общая длина стала 8 символов
  Serial.print(',');
  Serial.print(prependZeroTo8Digits(DEC_nextstar_position_curr));
  Serial.print(DEC_nextstar_position_curr, HEX);
  Serial.println('#');
}

void GOTO_process() {
  if (SYS_STATE == SYS_STATE_GOTO) {
    //set RA_DIR
    //set DEC_DIR


    //условия тиков по осям, достижение заданных координат
    MOTOR_RA_TICK();
    MOTOR_DEC_TICK();

  }
}

