void SERIAL_listen() {
  String str_from_stellarium = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    str_from_stellarium += inChar;
    if (inChar == '\n') {
      GOTO_processSerialCommand(str_from_stellarium);
    }
  }
}
