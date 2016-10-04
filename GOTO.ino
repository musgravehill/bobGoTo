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

    //стеллариум ГОТО шлет приказ, а мы выставили телескоп туда и сразу пишем гото-координаты в текущие
    if (SYS_STATE == SYS_STATE_GOTO_INIT) {
      RA_nextstar_position_curr =  RA_nextstar_position_goto;
      DEC_nextstar_position_curr = DEC_nextstar_position_goto;
    }
    else {
      SYS_STATE = SYS_STATE_GOTO_PROCESS;
    }

    Serial.println("#");
  }
  SYS_str_from_stellarium = "";
}



void GOTO_nextstar_position_curr_send_to_stellarium() {
  Serial.print(SERIAL_prependZeroTo8Digits(RA_nextstar_position_curr)); //допишем нули спереди
  Serial.print(RA_nextstar_position_curr, HEX); //позиция в HEX, а перед ней дописали нули, чтобы общая длина стала 8 символов
  Serial.print(',');
  Serial.print(SERIAL_prependZeroTo8Digits(DEC_nextstar_position_curr));
  Serial.print(DEC_nextstar_position_curr, HEX);
  Serial.println('#');
}

void GOTO_process(){
  }

void GOTO_tick() {
  if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    long RA_pos_error = RA_nextstar_position_curr - RA_nextstar_position_goto;
    long DEC_pos_error = DEC_nextstar_position_curr - DEC_nextstar_position_goto;
    unsigned long RA_pos_error_abs = abs(RA_pos_error);
    unsigned long DEC_pos_error_abs = abs(DEC_pos_error);

    //set RA_DIR
    //set DEC_DIR

    // calc new RA_pos, new DEC_pos
    // RA_pos =  RA_pos  +_   (KT=68) * RA_stephex_by_1_motor_microstep

    if (RA_pos_error_abs > GOTO_position_error_allow) {
      MOTOR_RA_TICK();
    }
    if (DEC_pos_error_abs > GOTO_position_error_allow) {
      MOTOR_DEC_TICK();
    }

    if (RA_pos_error_abs <= GOTO_position_error_allow && DEC_pos_error_abs <= GOTO_position_error_allow) {
      SYS_STATE = SYS_STATE_GOTO_READY;
    }

  }
}

