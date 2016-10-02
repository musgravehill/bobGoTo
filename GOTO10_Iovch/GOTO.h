unsigned long StrToHEX (String STR)
{
  int  i;
  char c;
  unsigned long ulVal=0;
  for (i=0; i<STR.length(); i++)
  {
   ulVal=ulVal*16;
   c=STR.charAt(i);
   switch (c) 
    {
      case 'f': ;
      case 'F': ulVal++;
      case 'e': ;
      case 'E': ulVal++;
      case 'd': ;
      case 'D': ulVal++;
      case 'c': ;
      case 'C': ulVal++;
      case 'b': ;
      case 'B': ulVal++;
      case 'a': ;
      case 'A': ulVal++;
      case '9': ulVal++;
      case '8': ulVal++;
      case '7': ulVal++;
      case '6': ulVal++;
      case '5': ulVal++;
      case '4': ulVal++;
      case '3': ulVal++;
      case '2': ulVal++;
      case '1': ulVal++;
    };
  };
 return ulVal;
};

String HexTo8D (unsigned long Hex)
{
  String STR0="";
  char c = '0';
  if (Hex<0x10000000) STR0 += c;
  if (Hex<0x1000000)  STR0 += c;
  if (Hex<0x100000)   STR0 += c;
  if (Hex<0x10000)    STR0 += c;
  if (Hex<0x1000)     STR0 += c;
  if (Hex<0x100)      STR0 += c;
  if (Hex<0x10)       STR0 += c;
  return STR0;
};

String HexTo4D (unsigned int Hex)
{
  String STR0="";
  char c = '0';
  if (Hex<0x1000)      STR0 += c;
  if (Hex<0x100)       STR0 += c;
  if (Hex<0x10)        STR0 += c;
  return STR0;
};

long Stepper_step(long ipSteps, unsigned uStepPin, unsigned uDirPin, unsigned uStepsPS)
{
 long iSteps=ipSteps, lRetVal=0;
 if((uStepPin>53)||(uDirPin>53)) return lRetVal;
 
 if(iSteps > 0) digitalWrite(uDirPin,  LOW);
 if(iSteps < 0) digitalWrite(uDirPin,  HIGH);
 iSteps=abs(iSteps);

 while (iSteps>0)
 {
  digitalWrite(uStepPin,  HIGH);
  delay(1000/uStepsPS);
  delayMicroseconds(1000*(1000%uStepsPS));
  digitalWrite(uStepPin,  LOW);
  iSteps--;
  if (ipSteps>0) lRetVal++; else lRetVal--;
 }
 return lRetVal;
}

int LCDPrintString (String str, int row, int kol)
{
  int i=0;
  while (i<16 && i<str.length())
  {
    //lcd.setCursor(kol-1+i, row-1);
    //lcd.print(str.charAt(i));
    i++;
  }
}

int LCDPrintSTR (char* str, int row, int kol)
{
  int i=0;
  while (i<16 && str[i]!='\0')
  {
    //lcd.setCursor(kol-1+i, row-1);
    //lcd.print(str[i]);
    i++;
  }
}
