void SERIAL_listen() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    SYS_str_from_stellarium += inChar;
    if (inChar == 'e' || inChar == '\n' || inChar == '\r' ) {
      SYS_isset_new_command_from_stellarium = true;      
    }
  }
}
