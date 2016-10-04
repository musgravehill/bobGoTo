void GOTO_processSerialCommand() {
  if ('r' == SYS_str_from_stellarium.charAt(0)) {
    GOTO_process();
    Serial.println("#");
  }
  SYS_str_from_stellarium = "";
}

void GOTO_current_positions_send_to_stellarium() {
  Serial.print(SERIAL_prependZeroTo8Digits(RA_nextstar_position_curr)); //допишем нули спереди
  Serial.print(RA_nextstar_position_curr, HEX); //позиция в HEX, а перед ней дописали нули, чтобы общая длина стала 8 символов
  Serial.print(',');
  Serial.print(SERIAL_prependZeroTo8Digits(DEC_nextstar_position_curr));
  Serial.print(DEC_nextstar_position_curr, HEX);
  Serial.println('#');
}

void GOTO_process() {
  //если начинается приязка к стеллариуму, то просто получим гото-задание
  if (SYS_STATE == SYS_STATE_GOTO_INIT) {
    char command_goto_RA_hex[9]; //one more element than your initialization is required, to hold the required null character.
    char command_goto_DEC_hex[9];  //one more element than your initialization is required, to hold the required null character.

    uint8_t i;
    i = SYS_str_from_stellarium.indexOf(',');
    (SYS_str_from_stellarium.substring(1, i)).toCharArray(command_goto_RA_hex, 9);
    (SYS_str_from_stellarium.substring(i + 1)).toCharArray(command_goto_DEC_hex, 9);

    RA_nextstar_position_goto =  strtoul(command_goto_RA_hex, NULL, 16);
    DEC_nextstar_position_goto =  strtoul(command_goto_DEC_hex, NULL, 16);
  }
  else if (SYS_STATE == SYS_STATE_GOTO_READY) {
    SYS_STATE = SYS_STATE_GOTO_PROCESS; //начали ГОТО-наведение

    char command_goto_RA_hex[9]; //one more element than your initialization is required, to hold the required null character.
    char command_goto_DEC_hex[9];  //one more element than your initialization is required, to hold the required null character.

    uint8_t i;
    i = SYS_str_from_stellarium.indexOf(',');
    (SYS_str_from_stellarium.substring(1, i)).toCharArray(command_goto_RA_hex, 9);
    (SYS_str_from_stellarium.substring(i + 1)).toCharArray(command_goto_DEC_hex, 9);

    RA_nextstar_position_goto =  strtoul(command_goto_RA_hex, NULL, 16);
    DEC_nextstar_position_goto =  strtoul(command_goto_DEC_hex, NULL, 16);




    //calc count ticks for ra, dec. Calc dir_ra, dir_dec

    RA_GOTO_count_ticks_made = 0L;
    DEC_GOTO_count_ticks_made = 0L;
    RA_GOTO_count_ticks_need = 0L;
    DEC_GOTO_count_ticks_need = 0L;
    RA_GOTO_dir = true; //true, false
    DEC_GOTO_dir = true; //true, false


  }
  else if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    //не обрабатываем новые гото-приказы, сейчас идет процесс наведения телескопа
  }
}

void GOTO_tick() {
  if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    if (RA_GOTO_count_ticks_made < RA_GOTO_count_ticks_need) {
      MOTOR_RA_TICK();
      RA_GOTO_count_ticks_made++;
    }
    else if (DEC_GOTO_count_ticks_made < DEC_GOTO_count_ticks_need) {
      MOTOR_DEC_TICK();
      DEC_GOTO_count_ticks_made++;
    }
    else {
      SYS_STATE = SYS_STATE_GOTO_READY;
    }
  }
}

void GOTO_calc_positions() {
  //стеллариум ГОТО шлет приказ, а мы выставили телескоп туда и сразу пишем гото-координаты в текущие
  if (SYS_STATE == SYS_STATE_GOTO_INIT) {
    RA_nextstar_position_curr =  RA_nextstar_position_goto;
    DEC_nextstar_position_curr = DEC_nextstar_position_goto;
  }
  //гото завершен, координаты совпали
  else if (SYS_STATE == SYS_STATE_GOTO_READY) {
    RA_nextstar_position_curr =  RA_nextstar_position_goto;
    DEC_nextstar_position_curr = DEC_nextstar_position_goto;
  }
  //гото в процессе наведения, будем считать позицию по сделанным шагам
  else if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    
  }
}



