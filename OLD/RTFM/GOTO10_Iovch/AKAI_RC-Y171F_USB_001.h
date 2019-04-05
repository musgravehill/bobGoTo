int AskAkaiUSB()
{
  int iA1=0, iA2=0;
  float iAS1=0.0, iAS2=0.0;
  int iRetValue=0, iN=0;
  boolean bContinue=true;
  
  do
   {
   ++iN;
   iA1 = analogRead(A6);
   iA2 = analogRead(A7);
   iAS1=((iN-1)*iAS1+iA1)/iN;
   iAS2=((iN-1)*iAS2+iA2)/iN;
   if((325>iA1||iA1>880) && (325>iA2||iA2>890)) {bContinue=false; iAS1=0; iAS2=0;} 
   if(iN>=50) {bContinue=false; iAS1=0; iAS2=0;}
   } while ((abs(iAS1-iA1)>0.3 || abs(iAS2-iA2)>0.3 || iN<5) &&  bContinue);

  if (325<iAS1&&iAS1<880) iA1=iAS1; else iA1=0;
  if (325<iAS2&&iAS2<890) iA2=iAS2; else iA2=0;
    
  if(iA1>325 && iA1<340) { iRetValue=112;}  //Serial.print("TV/VCR");
  if(iA1>340 && iA1<355) { iRetValue= -1;}  //Serial.print("<");
  if(iA1>355 && iA1<370) { iRetValue=  6;}  //Serial.print("MENU");
  if(iA1>375 && iA1<395) { iRetValue= -2;}  //Serial.print("V");
  if(iA1>395 && iA1<405) { iRetValue=119;}  //Serial.print("OK");
  if(iA1>410 && iA1<425) { iRetValue=  2;}  //Serial.print("^");
  if(iA1>425 && iA1<445) { iRetValue=  0;}  //Serial.print("AUDIO");
  if(iA1>445 && iA1<480) { iRetValue=  1;}  //Serial.print(">");
  if(iA1>500 && iA1<545) { iRetValue= -3;}  //Serial.print("STOP");
  if(iA1>545 && iA1<575) { iRetValue= -8;}  //Serial.print("<<");
  if(iA1>600 && iA1<620) { iRetValue=100;}  //Serial.print("0");
  if(iA1>630 && iA1<690) { iRetValue=  3;}  //Serial.print("PLAY");
  if(iA1>695 && iA1<730) { iRetValue=  8;}  //Serial.print(">>");
  if(iA1>750 && iA1<780) { iRetValue= 10;}  //Serial.print("ANNUL");
  if(iA1>860 && iA1<880) { iRetValue=109;}  //Serial.print("9");
  if(iA2>325 && iA2<335) { iRetValue=  4;}  //Serial.print("AFFICH.");
  if(iA2>335 && iA2<355) { iRetValue=101;}  //Serial.print("1");
  if(iA2>370 && iA2<390) { iRetValue= -5;}  //Serial.print("Power ON/OFF");
  if(iA2>395 && iA2<405) { iRetValue= -4;}  //Serial.print("SHOWVIEW");
  if(iA2>405 && iA2<425) { iRetValue= 22;}  //Serial.print("2");
  if(iA2>425 && iA2<445) { iRetValue=103;}  //Serial.print("3");
  if(iA2>450 && iA2<470) { iRetValue=120;}  //Serial.print("Timer/Clock");
  if(iA2>490 && iA2<510) { iRetValue=129;}  //Serial.print("VITESSE");
  if(iA2>520 && iA2<540) { iRetValue=128;}  //Serial.print("RECORD");
  if(iA2>550 && iA2<570) { iRetValue=107;}  //Serial.print("7");
  if(iA2>570 && iA2<610) { iRetValue=-11;}  //Serial.print("4");
  if(iA2>640 && iA2<660) { iRetValue=-22;}  //Serial.print("8");
  if(iA2>670 && iA2<725) { iRetValue= -7;}  //Serial.print("5");
  if(iA2>730 && iA2<765) { iRetValue=  5;}  //Serial.print("PLAY/PAUSE");
  if(iA2>820 && iA2<890) { iRetValue= 11;}  //Serial.print("6");
  return iRetValue;
}
