void GOTO_processSerialCommand() {
  char command = SYS_str_from_stellarium.charAt(0);
  switch (command) {
    case 'e':
      GOTO_nextstar_position_curr_send_to_stellarium();
      break;
    case 'r':
      String command_goto_RA_hex = SYS_str_from_stellarium.substring(1, 9);
      String command_goto_DEC_hex = SYS_str_from_stellarium.substring(9);

      RA_nextstar_position_goto = atol(command_goto_RA_hex);
      DEC_nextstar_position_goto = atol(command_goto_DEC_hex);

      // TODO нажали кнопку синхры, выставили телескоп на Вегу, в Стеллариуме сказали goto на Вегу, то
       RA_nextstar_position_curr =  RA_nextstar_position_goto;
       DEC_nextstar_position_curr = DEC_nextstar_position_goto;

      Serial.println("#");
      break;
  }
}

void GOTO_nextstar_position_curr_send_to_stellarium() {
  Serial.print(RA_nextstar_position_curr, HEX);
  Serial.print(',');
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

