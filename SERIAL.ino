void SERIAL_listen() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    SYS_str_from_stellarium += inChar;
    if (inChar == 'e' || inChar == '\n' || inChar == '\r' ) {
      SYS_isset_new_command_from_stellarium = true;
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
