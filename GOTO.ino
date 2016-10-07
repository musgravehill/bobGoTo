void GOTO_current_positions_send_to_stellarium() {
  Serial.print(SERIAL_prependZeroTo8Digits(RA_hex_position_curr)); //допишем нули спереди
  Serial.print(RA_hex_position_curr, HEX); //позиция в HEX, а перед ней дописали нули, чтобы общая длина стала 8 символов
  Serial.print(',');
  Serial.print(SERIAL_prependZeroTo8Digits(DEC_hex_position_curr));
  Serial.print(DEC_hex_position_curr, HEX);
  Serial.println('#');
  delay(1);
}

void GOTO_process() {
  //если начинается приязка к стеллариуму, то просто получим гото-задание
  if (SYS_STATE == SYS_STATE_GOTO_INIT) {
    char command_goto_RA_hex[9]; //one more element than your initialization is required, to hold the required null character \0
    char command_goto_DEC_hex[9];  //one more element than your initialization is required, to hold the required null character \0

    command_goto_RA_hex[0] = SYS_chars_from_stellarium[1]; // [0]=r
    command_goto_RA_hex[1] = SYS_chars_from_stellarium[2];
    command_goto_RA_hex[2] = SYS_chars_from_stellarium[3];
    command_goto_RA_hex[3] = SYS_chars_from_stellarium[4];
    command_goto_RA_hex[4] = SYS_chars_from_stellarium[5];
    command_goto_RA_hex[5] = SYS_chars_from_stellarium[6];
    command_goto_RA_hex[6] = SYS_chars_from_stellarium[7];
    command_goto_RA_hex[7] = SYS_chars_from_stellarium[8];

    command_goto_DEC_hex[0] =  SYS_chars_from_stellarium[10]; // [9]= ,
    command_goto_DEC_hex[1] =  SYS_chars_from_stellarium[11];
    command_goto_DEC_hex[2] =  SYS_chars_from_stellarium[12];
    command_goto_DEC_hex[3] =  SYS_chars_from_stellarium[13];
    command_goto_DEC_hex[4] =  SYS_chars_from_stellarium[14];
    command_goto_DEC_hex[5] =  SYS_chars_from_stellarium[15];
    command_goto_DEC_hex[6] =  SYS_chars_from_stellarium[16];
    command_goto_DEC_hex[7] =  SYS_chars_from_stellarium[17];

    RA_hex_position_goto =  strtoul(command_goto_RA_hex, NULL, 16);
    DEC_hex_position_goto =  strtoul(command_goto_DEC_hex, NULL, 16);

    //positions SYNCed!
    RA_hex_position_curr = RA_hex_position_goto;
    DEC_hex_position_curr = DEC_hex_position_goto;

    return;
  }
  else if (SYS_STATE == SYS_STATE_GOTO_READY) {
    //начали ГОТО-наведение
    SYS_STATE = SYS_STATE_GOTO_PROCESS;

    GOTO_RA_count_ticks_made_prev = 0L;
    GOTO_DEC_count_ticks_made_prev = 0L;

    //get goto-command from serial-string
    char command_goto_RA_hex[9]; //one more element than your initialization is required, to hold the required null character \0
    char command_goto_DEC_hex[9];  //one more element than your initialization is required, to hold the required null character \0

    command_goto_RA_hex[0] = SYS_chars_from_stellarium[1]; // [0]=r
    command_goto_RA_hex[1] = SYS_chars_from_stellarium[2];
    command_goto_RA_hex[2] = SYS_chars_from_stellarium[3];
    command_goto_RA_hex[3] = SYS_chars_from_stellarium[4];
    command_goto_RA_hex[4] = SYS_chars_from_stellarium[5];
    command_goto_RA_hex[5] = SYS_chars_from_stellarium[6];
    command_goto_RA_hex[6] = SYS_chars_from_stellarium[7];
    command_goto_RA_hex[7] = SYS_chars_from_stellarium[8];

    command_goto_DEC_hex[0] =  SYS_chars_from_stellarium[10]; // [9]= ,
    command_goto_DEC_hex[1] =  SYS_chars_from_stellarium[11];
    command_goto_DEC_hex[2] =  SYS_chars_from_stellarium[12];
    command_goto_DEC_hex[3] =  SYS_chars_from_stellarium[13];
    command_goto_DEC_hex[4] =  SYS_chars_from_stellarium[14];
    command_goto_DEC_hex[5] =  SYS_chars_from_stellarium[15];
    command_goto_DEC_hex[6] =  SYS_chars_from_stellarium[16];
    command_goto_DEC_hex[7] =  SYS_chars_from_stellarium[17];

    RA_hex_position_goto =  strtoul(command_goto_RA_hex, NULL, 16);
    DEC_hex_position_goto =  strtoul(command_goto_DEC_hex, NULL, 16);

    unsigned long RA_difference_abs;
    unsigned long DEC_difference_abs;

    /*                    RA_coordinate in h.m.s
                                STARs turns
                                 SKY turns
                               see from Earth

                             8h    ←★   11h
                          ★                ★
                         ↙                     14h
                       ★
                                                ↑
                   4h  ★          ★UMI         ★
                       ↓
                         ★                   16h
                                             ↗
                          0h ↘           21h
                                 ★→
             TELESCOPE turns counter-clock-wise too with star-speed
    */

    // RA_pos надо уменьшить => вращаемся за звездами (против часовой)  MOTOR_set_RA_dir(true) => RA_step_per_motor_microstep меньше (звезды убегают)
    // RA_pos надо увеличить => вращаемся против звезд (по часовой)  MOTOR_set_RA_dir(false) => RA_step_per_motor_microstep больше (звезды навстречу сами бегут)

    //если тупо наращивать или уменьшать до нужной точки
    unsigned long RA_difference_inc_or_dec_straight;
    // если перескочить через 0, выгодно если от края до края через 0, а не через весь диапазон
    unsigned long RA_difference_through_zero;

    if (RA_hex_position_goto > RA_hex_position_curr) {  //  0__curr______goto___MAX
      RA_difference_inc_or_dec_straight = RA_hex_position_goto - RA_hex_position_curr; //  0__curr>>>>>goto___MAX
      RA_difference_through_zero = RA_hex_position_curr + (RA_max_hex_value - RA_hex_position_goto); // 0<<<<curr_____goto<<<<MAX
      if (RA_difference_inc_or_dec_straight < RA_difference_through_zero) {
        //  0__curr>>>>>goto___MAX
        //select shorter RA_difference_inc_or_dec_straight
        RA_difference_abs = RA_difference_inc_or_dec_straight;
        MOTOR_set_RA_dir(false); //против звезд по часовой стрелке
        RA_dRA_sign = 1; //наращиваем координату
        RA_dRA_sky_offset_sign = 1; //шаг увеличится, плюсуем
      } else {
        // 0<<<<curr_____goto<<<<MAX
        //select shorter RA_difference_through_zero
        RA_difference_abs = RA_difference_through_zero;
        MOTOR_set_RA_dir(true); //за звездами против часовой стрелки
        RA_dRA_sign = -1; //уменьшаем координату
        RA_dRA_sky_offset_sign = -1;//шаг укоротим, за звездой гонимся
      }
    } else {
      RA_difference_inc_or_dec_straight =  RA_hex_position_curr - RA_hex_position_goto ; //  0__goto<<<<<<curr___MAX
      RA_difference_through_zero = RA_hex_position_goto + (RA_max_hex_value - RA_hex_position_curr); // 0>>>>goto_____curr>>>>>MAX
      if (RA_difference_inc_or_dec_straight < RA_difference_through_zero) {
        //  0__goto<<<<<<curr___MAX
        //select shorter RA_difference_inc_or_dec_straight
        RA_difference_abs = RA_difference_inc_or_dec_straight;
        MOTOR_set_RA_dir(true); //за звездой против часовой стрелки
        RA_dRA_sign = -1; //уменьшаем координату
        RA_dRA_sky_offset_sign = -1;//увеличим кол-во шагов, чтобы догнать звезду
      } else {
        // 0>>>>goto_____curr>>>>>MAX
        //select shorter RA_difference_through_zero
        RA_difference_abs = RA_difference_through_zero;
        MOTOR_set_RA_dir(false); //против звезд по часовой стрелке
        RA_dRA_sign = 1; //наращиваем координату
        RA_dRA_sky_offset_sign = 1; //шаг увеличится, плюсуем
      }
    }

    //если тупо наращивать или уменьшать до нужной точки
    unsigned long DEC_difference_inc_or_dec_straight;
    // если перескочить через 0, выгодно если от края до края через 0, а не через весь диапазон
    unsigned long DEC_difference_through_zero;

    if (DEC_hex_position_goto > DEC_hex_position_curr) {
      DEC_difference_inc_or_dec_straight = DEC_hex_position_goto - DEC_hex_position_curr; //  0__curr>>>>>goto___MAX
      DEC_difference_through_zero = DEC_hex_position_curr + (DEC_max_hex_value - DEC_hex_position_goto); // 0<<<<curr_____goto<<<<MAX
      if (DEC_difference_inc_or_dec_straight < DEC_difference_through_zero) {
        //  0__curr>>>>>goto___MAX
        //select shorter DEC_difference_inc_or_dec_straight
        DEC_difference_abs = DEC_difference_inc_or_dec_straight;
        MOTOR_set_DEC_dir(false); //TODO CHECK IT
        DEC_dDEC_sign = 1; //наращиваем координату
      } else {
        // 0<<<<curr_____goto<<<<MAX
        //select shorter DEC_difference_through_zero
        DEC_difference_abs = DEC_difference_through_zero;
        MOTOR_set_DEC_dir(true); //TODO CHECK IT
        DEC_dDEC_sign = -1; //уменьшаем координату
      }
    } else {
      DEC_difference_inc_or_dec_straight =  DEC_hex_position_curr - DEC_hex_position_goto ; //  0__goto<<<<<<curr___MAX
      DEC_difference_through_zero = DEC_hex_position_goto + (DEC_max_hex_value - DEC_hex_position_curr); // 0>>>>goto_____curr>>>>>MAX
      if (DEC_difference_inc_or_dec_straight < DEC_difference_through_zero) {
        //  0__goto<<<<<<curr___MAX
        //select shorter DEC_difference_inc_or_dec_straight
        DEC_difference_abs = DEC_difference_inc_or_dec_straight;
        MOTOR_set_DEC_dir(true); //TODO CHECK IT
        DEC_dDEC_sign = -1; //уменьшаем координату
      } else {
        // 0>>>>goto_____curr>>>>>MAX
        //select shorter DEC_difference_through_zero
        DEC_difference_abs = DEC_difference_through_zero;
        MOTOR_set_DEC_dir(false); //TODO CHECK IT
        DEC_dDEC_sign = 1; //наращиваем координату
      }
    }

    //calc count ticks for ra, dec
    RA_GOTO_count_ticks_made = 0L;
    DEC_GOTO_count_ticks_made = 0L;

    //пока мы крутим - а это время зависит от МАХ кол-ва тиков по RA или DEC
    //например, RA_difference_abs=0 DEC_difference_abs=100500 => будем тикать по DEC, а RA будет стоять,
    //НО небо-то ползет. Надо компенсировать дополнительно!
    // значит, надо RA перекрутить или недокрутить
    //Когда RA кончился, но еще тикает DEC...

    //Путь RA = смещениеКоординатыRA ± (сдвиг_неба_пока_RA_наводится или сдвиг_неба_пока_DEC_наводится, смотря кто дольше)
    RA_GOTO_count_ticks_need = RA_difference_abs / (RA_step_per_motor_microstep + RA_dRA_sky_offset_sign * GOTO_plusminus_dRA_per_1_tick); //шаг укорачивается или удлиняется
    DEC_GOTO_count_ticks_need = DEC_difference_abs / DEC_step_per_motor_microstep; //изменение координаты только

    //Когда RA кончился, но еще тикает DEC...
    GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_made = 0L;
    if (DEC_GOTO_count_ticks_need > RA_GOTO_count_ticks_need) {
      //GOTO_countTicksDEC_afterFinishingRA = DEC_GOTO_count_ticks_need - RA_GOTO_count_ticks_need;
      //ГОТО-скорость выше звездной в [N = goto_motor-freq / star_moto_freq] раз, тикать надо меньше в N раз.
      GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_need = 0.003819370188195 * (DEC_GOTO_count_ticks_need - RA_GOTO_count_ticks_need);
    }
    else {
      GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_need = 0L;
    }

    /*Serial.println(" ");
    Serial.print(" RAt=");  Serial.print(RA_GOTO_count_ticks_need, DEC);
    Serial.print(" DECt=");  Serial.print(DEC_GOTO_count_ticks_need, DEC);
    Serial.print(" RAtCompens=");  Serial.println(GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_need, DEC);
    */

    TIMER_GOTO_config(); //SET FAST TIMER TO TICK MOTORS FAST

    return;

  }
  else if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    //не обрабатываем новые гото-приказы, сейчас идет процесс наведения телескопа
    return;
  }
}

void GOTO_tick() {
  if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    if (RA_GOTO_count_ticks_made < RA_GOTO_count_ticks_need) {
      MOTOR_RA_TICK();
      RA_GOTO_count_ticks_made++;
    } else {
      //Когда RA кончился, но еще тикает DEC...
      if (GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_made == 0) {
        MOTOR_set_RA_dir(true); //направление за звездами поставим перед первым тиком компенсации
      }
      if (GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_made < GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_need) {
        MOTOR_RA_TICK();
        GOTO_countTicksRA_forSkyRotationCompensation_afterFinishingRA_whileInProcessDEC_made++;
      }
    }
    if (DEC_GOTO_count_ticks_made < DEC_GOTO_count_ticks_need) {
      MOTOR_DEC_TICK();
      DEC_GOTO_count_ticks_made++;
    }
  }
}

void GOTO_calc_positions() {
  //------------гото в процессе наведения, будем считать позицию по сделанным шагам---------------------------------
  //if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
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
  //}
  //else координаты достигнуты, всё устаканилось
}

void GOTO_check_goto_is_completed() {
  if (SYS_STATE == SYS_STATE_GOTO_PROCESS) {
    if  ((RA_GOTO_count_ticks_made >= RA_GOTO_count_ticks_need) && (DEC_GOTO_count_ticks_made >= DEC_GOTO_count_ticks_need)) {
      GOTO_set_normal_mode();
    }
  }
}

//press btn GOTO_SYNC => coordinates set synced
void GOTO_BUTTON_coordinates_sync_ok() {
  GOTO_set_normal_mode();
}

//пусть вращается за звездами, наводка окончена
void GOTO_set_normal_mode() {
  TIMER_STAR_config();
  SYS_STATE = SYS_STATE_GOTO_READY; //run star-speed
  MOTOR_set_RA_dir(true); //run star-speed

  //off for debugging
  //RA_hex_position_curr =  RA_hex_position_goto;
  //DEC_hex_position_curr = DEC_hex_position_goto;
}


