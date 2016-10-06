
/*  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs
*/
void serialEvent() {
  File dataFile = SD.open("serialEvent.txt", FILE_WRITE);
  while (Serial.available()) {
    char inChar = (char)Serial.read();

    if (dataFile) {
      dataFile.print(inChar);
    }

    // STELLARIUM DOESNOT SEND any \r \n! Only solid eeeeeeerFFFFFFFF,FFFFFFFeeeeeee
    //if (inChar == '\n' || inChar == '\r' )
    if (inChar == 'r') {
      SYS_str_from_stellarium = "r"; //make fresh new string
    }
    else if (inChar != 'e') { //not 'e', so it can be 1234567890ABCDEF, from Stellaium
      SYS_str_from_stellarium += inChar;
    }
  }


  if (SYS_str_from_stellarium.length() == 18 ) {
    GOTO_processSerialCommand(); //r017DDC8F,14AF9C50
    if (dataFile) {
      dataFile.println("SYS_str_from_stellarium=");
      dataFile.println(SYS_str_from_stellarium);      
    }
  }

  if (dataFile) {
    dataFile.print("|___|");
    dataFile.close();
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
