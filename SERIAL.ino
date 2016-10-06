
/*  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs
*/
void serialEvent() {

  while (Serial.available()) {
    char inChar = (char)Serial.read();

    // STELLARIUM DOESNOT SEND any \r \n! Only solid eeeeeeerFFFFFFFF,FFFFFFFeeeeeee
    if (inChar == 'r') {
      SYS_chars_from_stellarium_pos = 0;
      SYS_chars_from_stellarium[SYS_chars_from_stellarium_pos] = 'r';
    }
    else if ((SYS_chars_from_stellarium_pos <= 16) &&  (inChar != 'e')) {
      //in with 0..16, then pos++, so last char: 16 in -> 16++ -> [17]=lastChar
      //not 'e', so it can be 1234567890,ABCDEF from Stellaium
      SYS_chars_from_stellarium_pos++;
      SYS_chars_from_stellarium[SYS_chars_from_stellarium_pos] = inChar;
    }
  }

  //rABC45678,12345678 = 17 position; 0=r 1=A 2=B
  if (SYS_chars_from_stellarium_pos == 17) {
    if ('r' == SYS_chars_from_stellarium[0]) {
      Serial.println("#");
      GOTO_process();
    }
    SYS_chars_from_stellarium_pos = 0;
  }
}



/*void SERIAL_listen() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    // STELLARIUM DOESNOT SEND any \r \n! Only solid eeeeeeerFFFFFFFF,FFFFFFFeeeeeee
    //if (inChar == '\n' || inChar == '\r' )
    if (inChar == 'r') {
      SYS_str_from_stellarium = "r";
    }
    else if (inChar != 'e') {
      SYS_str_from_stellarium += inChar;
    }
  }
  }*/

//выдаст нули для дополнения строки до 8 символов: 4FA -> 00000
String SERIAL_prependZeroTo8Digits(unsigned long value_hex) {
  String result_str_8 = "";
  char c = '0';
  if (value_hex < 0x10000000) result_str_8 += c;
  if (value_hex < 0x1000000)  result_str_8 += c;
  if (value_hex < 0x100000)   result_str_8 += c;
  if (value_hex < 0x10000)    result_str_8 += c;
  if (value_hex < 0x1000)     result_str_8 += c;
  if (value_hex < 0x100)      result_str_8 += c;
  if (value_hex < 0x10)       result_str_8 += c;
  return result_str_8;
};
