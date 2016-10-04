void GOTO_processSerialCommand() {
  if ('r' == SYS_str_from_stellarium.charAt(0)) {
    GOTO_process();
    Serial.println("#");
  }
  SYS_str_from_stellarium = "";
}

void GOTO_current_positions_send_to_stellarium() {
  Serial.print(SERIAL_prependZeroTo8Digits(RA_hex_position_curr)); //допишем нули спереди
  Serial.print(RA_hex_position_curr, HEX); //позиция в HEX, а перед ней дописали нули, чтобы общая длина стала 8 символов
  Serial.print(',');
  Serial.print(SERIAL_prependZeroTo8Digits(DEC_hex_position_curr));
  Serial.print(DEC_hex_position_curr, HEX);
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

    RA_hex_position_goto =  strtoul(command_goto_RA_hex, NULL, 16);
    DEC_hex_position_goto =  strtoul(command_goto_DEC_hex, NULL, 16);
  }
  else if (SYS_STATE == SYS_STATE_GOTO_READY) {
    //начали ГОТО-наведение
    SYS_STATE = SYS_STATE_GOTO_PROCESS;

    GOTO_RA_count_ticks_made_prev = 0L;
    GOTO_DEC_count_ticks_made_prev = 0L;

    //get goto-command from serial-string
    char command_goto_RA_hex[9]; //one more element than your initialization is required, to hold the required null character.
    char command_goto_DEC_hex[9];  //one more element than your initialization is required, to hold the required null character.

    uint8_t i;
    i = SYS_str_from_stellarium.indexOf(',');
    (SYS_str_from_stellarium.substring(1, i)).toCharArray(command_goto_RA_hex, 9);
    (SYS_str_from_stellarium.substring(i + 1)).toCharArray(command_goto_DEC_hex, 9);

    RA_hex_position_goto =  strtoul(command_goto_RA_hex, NULL, 16);
    DEC_hex_position_goto =  strtoul(command_goto_DEC_hex, NULL, 16);

    /*Serial.print("RA_goto=");
    Serial.print(RA_hex_position_goto, HEX);
    Serial.print(" DEC_goto=");
    Serial.println(DEC_hex_position_goto, HEX);*/

    unsigned long RA_difference_abs;
    unsigned long DEC_difference_abs;

    //calc direction for motor spinning
    if (RA_hex_position_goto > RA_hex_position_curr) {
      RA_difference_abs = RA_hex_position_goto - RA_hex_position_curr;
      MOTOR_set_RA_dir(true);
      RA_dRA_sign = 1; //TODO CHECK IT
    } else {
      RA_difference_abs = - RA_hex_position_goto + RA_hex_position_curr;
      MOTOR_set_RA_dir(false);
      RA_dRA_sign = -1; //TODO CHECK IT
    }

    if (DEC_hex_position_goto > DEC_hex_position_curr) {
      DEC_difference_abs = DEC_hex_position_goto - DEC_hex_position_curr;
      MOTOR_set_DEC_dir(true);
      DEC_dDEC_sign = 1; //TODO CHECK IT
    } else {
      DEC_difference_abs = - DEC_hex_position_goto + DEC_hex_position_curr;
      MOTOR_set_DEC_dir(false);
      DEC_dDEC_sign = -1; //TODO CHECK IT
    }

    //calc count ticks for ra, dec
    RA_GOTO_count_ticks_made = 0L;
    DEC_GOTO_count_ticks_made = 0L;

    //пока мы крутим, небо сползет на GOTO_plusminus_dRA_per_1_tick * RA_GOTO_count_ticks_need
    // значит, надо RA перекрутить или недокрутить
    RA_GOTO_count_ticks_need = 0.51 + RA_difference_abs / (RA_step_per_motor_microstep - RA_dRA_sign * GOTO_plusminus_dRA_per_1_tick);

    DEC_GOTO_count_ticks_need = 0.51 + DEC_difference_abs / DEC_step_per_motor_microstep; // 0.51 + 99.5 = 100

    /*Serial.print("RA_ticks=");
    Serial.print(RA_GOTO_count_ticks_need, DEC);
    Serial.print(" DEC_ticks=");
    Serial.println(DEC_GOTO_count_ticks_need, DEC);*/

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
    if (DEC_GOTO_count_ticks_made < DEC_GOTO_count_ticks_need) {
      MOTOR_DEC_TICK();
      DEC_GOTO_count_ticks_made++;
    }
  }
}

void GOTO_calc_positions() {
  if  ((RA_GOTO_count_ticks_made >= RA_GOTO_count_ticks_need) && (DEC_GOTO_count_ticks_made >= DEC_GOTO_count_ticks_need)) {
    SYS_STATE = SYS_STATE_GOTO_READY;
    MOTOR_set_RA_dir(true);
  }

  //-------стеллариум ГОТО шлет приказ, а мы выставили телескоп туда и сразу пишем гото-координаты в текущие------
  if (SYS_STATE == SYS_STATE_GOTO_INIT) {
    RA_hex_position_curr =  RA_hex_position_goto;
    DEC_hex_position_curr = DEC_hex_position_goto;
  }
  //--------гото завершен, координаты совпали----------------------------------------------------------------------
  else if (SYS_STATE == SYS_STATE_GOTO_READY) {
    RA_hex_position_curr =  RA_hex_position_goto;
    DEC_hex_position_curr = DEC_hex_position_goto;
  }
  //------------гото в процессе наведения, будем считать позицию по сделанным шагам---------------------------------
  else if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    //TODO CHECK IT sign +-
    unsigned long tmp = (RA_GOTO_count_ticks_made - GOTO_RA_count_ticks_made_prev) * (RA_step_per_motor_microstep - RA_dRA_sign * GOTO_plusminus_dRA_per_1_tick);
    if (RA_dRA_sign > 0) {
      RA_hex_position_curr += tmp;
    } else {
      RA_hex_position_curr -= tmp;
    }
    
    //TODO CHECK IT sign +-
    tmp = (DEC_GOTO_count_ticks_made - GOTO_DEC_count_ticks_made_prev) * DEC_step_per_motor_microstep;
    if (DEC_dDEC_sign > 0) {
      DEC_hex_position_curr +=  tmp;
    } else {
      DEC_hex_position_curr -=  tmp;
    }    

    GOTO_RA_count_ticks_made_prev = RA_GOTO_count_ticks_made;
    GOTO_DEC_count_ticks_made_prev = DEC_GOTO_count_ticks_made;

    /*Serial.print(" RA_dRA_sign=");
      Serial.print(RA_dRA_sign, DEC);
      Serial.print(" DEC_dDEC_sign=");
      Serial.println(DEC_dDEC_sign, DEC);*/

    /*Serial.print(" RA_GOTO_count_ticks_made=");
      Serial.print(RA_GOTO_count_ticks_made, DEC);
      Serial.print(" DEC_GOTO_count_ticks_made=");
      Serial.println(DEC_GOTO_count_ticks_made, DEC);*/

  }
}



