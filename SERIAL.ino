void SERIAL_listen() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    SYS_str_from_stellarium += inChar;
    //if (inChar == '\n' || inChar == '\r' ) // STELLARIUM DOESNOT SEND any \r \n! Only solid eeeeeeerFFFFFFFF,FFFFFFFeeeeeee
    if (inChar == 'r') {
      SYS_str_from_stellarium = "r";
    }
    else if (inChar != 'e') {
      SYS_str_from_stellarium += inChar;
    }
  }
}

String prependZeroTo8Digits(unsigned long value_hex) {
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
