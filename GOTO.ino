void GOTO_processSerialCommand(String str_from_stellarium) {
  char command = str_from_stellarium.charAt(0);
  switch (command) {
    case 'e':
      GOTO_currPos_return();
      break;
    case 'r':
      String command_goto_RA_hex = str_from_stellarium.substring(1, 8);
      String command_goto_DEC_hex = str_from_stellarium.substring(9);

      //DBG
      Serial.print("r");
      Serial.print(command_goto_RA_hex);
      Serial.println(command_goto_DEC_hex);
      //END DBG
      
      Serial.println("#");
      break;
  }
}

void GOTO_currPos_return() {
  Serial.println("12345678,12345678#");
}
