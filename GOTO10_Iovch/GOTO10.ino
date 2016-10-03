#define FO_STEP_PIN 2 //Контакт ардуино идущий на STEP драйвера фокусера
#define FO_DIR_PIN  3 //Контакт ардуино идущий на DIR  драйвера фокусера
#define DE_STEP_PIN 4 //Контакт ардуино идущий на STEP драйвера склонений
#define DE_DIR_PIN  5 //Контакт ардуино идущий на DIR  драйвера склонений
#define RA_STEP_PIN 6 //Контакт ардуино идущий на STEP драйвера ПВ
#define RA_DIR_PIN  7 //Контакт ардуино идущий на DIR  драйвера ПВ

#define LIGHT_SENCE_APIN A1 //Вход датчика освещенности
#define LCD_LIGHT_PIN 11    //Вывод управления подсветкой LCD

const unsigned long ms_in_star1Day=86164091; //Милисекунд в Звездных сутках

const int RA_microstep_value = 16; //дробление шага ПВ (микрошаг 1\N)
const int DEC_microstep_value =  4; //дробление шага СК (микрошаг 1\N)
const int FOCUS_microstep_value =  8; //дробление шага фокусера (микрошаг 1\N)

const int maxAllowed_RA_steps_in_1second = 1000*RA_microstep_value; //мах быстрый шаг мотора (шагов в секунду), зависит от качества мотора 
const int maxAllowed_DEC_steps_in_1second = 1000*DEC_microstep_value; //мах быстрый шаг мотора (шагов в секунду), зависит от качества мотора 
const int maxAllowed_FOCUS_steps_in_1second = 1000*FOCUS_microstep_value; //мах быстрый шаг мотора (шагов в секунду), зависит от качества мотора 

const unsigned long RA_ticks_in_1_revolution = 85473*RA_microstep_value;  //тиков двигателя на полный оборот оси прямого восхождения (зависит от редукции)
const unsigned long DEC_ticks_in_1_revolution = 261888*DEC_microstep_value; //тиков двигателя на полный оборот оси склонений (зависит от редукции)

const unsigned long RA_max_hex_value = 0xFFFFFFFF;  //Максимальное значение величины прямого восхождения
const unsigned long DEC_max_hex_value = 0xFFFFFFFF;  //Максимальное значение величины склонения

const unsigned long RA_STEPS_in_1_RA_MOTOR_STEP=RA_max_hex_value/RA_ticks_in_1_revolution; //Единиц прямого восхождения на 1 шаг двигателя
const unsigned long DEC_STEPS_in_1_RA_MOTOR_STEP=DEC_max_hex_value/DEC_ticks_in_1_revolution; //Единиц склонения на 1 шаг двигателя

const int RA_rotate_starspeed__ms_period_of_tick=int(ms_in_star1Day/RA_ticks_in_1_revolution); //Периодичность шага ДПВ для компенсации вращения Земли

const unsigned long compensateEarth_dRA_STEPS_in_1_RA_MOTOR_STEP = RA_max_hex_value/ms_in_star1Day*1000/maxAllowed_RA_steps_in_1second; //Поправка вращения Земли на 1 шаг ДПВ
 
const unsigned long compensateEarth_dDEC_STEPS_in_1_RA_MOTOR_STEP=7; //Поправка (доворот) ДСК в единицах СК на 1 шаг ДСК

unsigned long iMilisec=millis();   // Время предыдущего шага
unsigned long iPortTimer=millis(); // Время предыдущей посылки данных в порт

unsigned long iPRA=0;   //Текущее (исходное) значение прямого восхождения
unsigned long iPDE=0;   //Текущее (исходное) значение склонения
unsigned long iToPRA=0; //Целевое значение прямого восхождения
unsigned long iToPDE=0; //Целевое значение склонения

int iDeD= 0; // Телескоп слева от полярной оси 1, справа -1
int iRaD= 1; // Направление ведения NORTH (для Южного полушария -1)

int iLastMovement = 0;    // Направление перемещения монтировки в предыдущем цикле
int iMovement = 0;        // Направление перемещения монтировки в текущем цикле
int iSteps = 0;           // Подсчет количества шагов

boolean bAlignment=false; // Выравнивание не выполнено
boolean bRun=true;        // Ведение включено изначально
boolean bLCD=false;       // LCD врёт

String STR= "", STR1="", STR2="";
String LCDString1="";
String LCDString2="";
String RaStr, DeStr;

String GetString ()
{
  String STR="";
  char c;
  if (!Serial.available() && ((millis()-iPortTimer) >= 1000)) {iPortTimer=millis(); STR="e"; return STR;}
  while (Serial.available())  //если есть что читать;
  {
    c = Serial.read();       //читаем символ
    if (c!='\n' && c!='\r' ) //если это не спецсимвол прибавляем к строке
    STR += c;
   delay(1); //Необходимая задержка цикла, для синхронизации порта при 9600 бит/сек
  }
  return STR;
}

int GetSubStr ()
{
  int i;
  i=STR.indexOf(',');
  STR2=STR.substring(i+1);
  if (i<=1) STR1="";
  else STR1=STR.substring(1,i);
};

void HexRaToString(unsigned long ulRaVal, unsigned long ulMaxRaVal)
{
  double udRa=0;
  int iRaH=0, iRaM=0, iRaS=0;
  udRa=double(ulRaVal>>8)/double(ulMaxRaVal>>8);
  iRaH=udRa*24;
  udRa=udRa-double(iRaH)/24;
  iRaM=udRa*24*60;
  udRa=udRa-double(iRaM)/24/60;
  iRaS=round(udRa*24*60*60);
  RaStr=String("Ra="+String(iRaH)+"h"+String(iRaM)+"m"+String(iRaS)+"s");
  switch (iRaD)
  {
  case -1: {RaStr=RaStr+" S"; break;}
  case  1: {RaStr=RaStr+" N"; break;}
  }
}

void HexDeToString(unsigned long ulDeVal, unsigned long ulMaxDeVal)
{
  double udDe=0;
  int iDeG=0, iDeM=0, iDeS=0;
  udDe=double(ulDeVal>>8)/double(ulMaxDeVal>>8);
  if(udDe>0.5){udDe=udDe-1;}
  iDeG=udDe*360;
  udDe=udDe-double(iDeG)/360;
  iDeM=udDe*360*60;
  udDe=udDe-double(iDeM)/360/60;
  iDeM=abs(iDeM);
  iDeS=round(abs(udDe*360*60*60));
    
  DeStr=String("De="+String(iDeG)+"*"+String(iDeM)+"m"+String(iDeS)+"s");
  switch (iDeD)
  {
  case -1: {DeStr=DeStr+" N/E"; break;}
  case  1: {DeStr=DeStr+" N/W"; break;}
  }
}

void SetLCDLight(void)
{
 analogWrite(LCD_LIGHT_PIN,512-analogRead(LIGHT_SENCE_APIN)/4);
}

void LCDPrint()
 {
  if(!bLCD)
  {
   if(bAlignment)
   {
    HexRaToString(iPRA, RA_max_hex_value);
    LCDString1=RaStr;
    HexDeToString(iPDE, DEC_max_hex_value);
    LCDString2=DeStr;
   }
   else
   {
    if (iDeD== 1) LCDString1 = "   N/W or S/E   "; //Телескоп слева от полярной оси
    if (iDeD==-1) LCDString1 = "   N/E or S/W   "; //Телескоп справа от полярной оси
    if (iDeD== 0) LCDString1 = " Arduino GOTO10 "; //Телескоп не сориентирован по склонению
    if (iRaD== 0) LCDString1+= " RAERR";           //Не задано направление ведения телескопа
    if (bRun)    {LCDString2 = " TRACKING"; if(iRaD>0) LCDString2+=" NORTH"; if(iRaD<0) LCDString2+=" SOUTH";}
    else          LCDString2 = "    STOPPED     ";
   }
  //lcd.clear();
  SetLCDLight();
  LCDPrintString(LCDString1,1,1);
  LCDPrintString(LCDString2,2,1);
  bLCD=true;
  }
  else
  return;
 }

void LCDCOR (int pKey)
{
 if(!bAlignment)
 {
  switch (pKey)
  {
  case -11:  
  case  -1: if(iRaD!=0) LCDPrintSTR (" RA Correction < ", 2, 1); else LCDPrintSTR ("N/S Position ERR", 2, 1); break;
  case  11:
  case   1: if(iRaD!=0) LCDPrintSTR (" RA Correction > ", 2, 1); else LCDPrintSTR ("N/S Position ERR", 2, 1); break;
  case  22:
  case   2: if(iDeD!=0) LCDPrintSTR (" DE Correction ^ ", 2, 1); else LCDPrintSTR ("E/W Position ERR", 2, 1); break;
  case -22:
  case  -2: if(iDeD!=0) LCDPrintSTR (" DE Correction v ", 2, 1); else LCDPrintSTR ("E/W Position ERR", 2, 1); break;
  case   8: LCDPrintSTR ("+^+  FOCUS   +^+", 2, 1); break;
  case  -8: LCDPrintSTR ("-v-  FOCUS   -v-", 2, 1); break;
  };
 }
 else
 {
  switch (pKey)
  {
  case -11:
  case  -1: if(iRaD!=0) LCDPrintSTR (" < ", 1, 14); else LCDPrintSTR ("<!>", 2, 14); break;
  case  11:
  case   1: if(iRaD!=0) LCDPrintSTR (" > ", 1, 14); else LCDPrintSTR (">!<", 2, 14); break;
  case  22:
  case   2: if(iDeD!=0) LCDPrintSTR (" ^ ", 2, 14); else LCDPrintSTR ("^!v", 2, 14); break;
  case -22:
  case  -2: if(iDeD!=0) LCDPrintSTR (" v ", 2, 14); else LCDPrintSTR ("v!^", 2, 14); break;
  case   8: LCDPrintSTR ("F+", 1, 14); break;
  case  -8: LCDPrintSTR ("F-", 1, 14); break;
  };
 }
}

void RA_Stepper_step(int ipSteps)
{
  Stepper_step(ipSteps, RA_STEP_PIN, RA_DIR_PIN, maxAllowed_RA_steps_in_1second);
}

void DE_Stepper_step(int ipSteps)
{
  Stepper_step(ipSteps, DE_STEP_PIN, DE_DIR_PIN, maxAllowed_DEC_steps_in_1second);
}

void FO_Stepper_step(int ipSteps)
{
  Stepper_step(ipSteps, FO_STEP_PIN, FO_DIR_PIN, maxAllowed_FOCUS_steps_in_1second);
}

int AskControl()
{
  int iRetVal=0;
  iRetVal=AskAkaiUSB();
  return iRetVal;
 }

void reaction () //Обработка команд ПУ
  {
   int iKey=0;
       
// Здесь мы договариваемся, что функция int AskControl(),
// к чему бы она ни была привязана, возвращает при ее вызове следующие значения:

//   0 - когда ничего не надо делать
//   1 - когда надо сделать шаг вперед по прямому восхождению
//  -1 - когда надо сделать шаг назад по прямому восхождению
//  11 - когда надо включить постоянное перемещение по прямому восхождению вперед
// -11 - когда надо включить постоянное перемещение по прямому восхождению назад
//   2 - когда надо сделать шаг вверх по склонению
//  -2 - когда надо сделать шаг вниз по склонению
//  22 - когда надо включить постоянное увеличение склонения
// -22 - когда надо включить постоянное уменьшение склонения
//   3 - когда надо включить ведение по прямому восхождению
//  -3 - отключить ведение по прямому восхождению
//  -5 - отключить все перемещения STOP
//   5 - включение/выключение бесконечного повтора последнего перемещения
//   4 - Телескоп слева от полярной оси
//  -4 - Телескоп справа от полярной оси
//   6 - Телескоп в северном полушарии
//  -6 - Телескоп в южном полушарии
//   7 - Повтор последней выполненной команды с пульта
//  -7 - Сброс постоянных перемещений, кроме ведения iMovement=0
//   8 - шаг фокусера от зеркал
//  -8 - шаг фокусера к зеркалам
//  10 - сброс калибровки

  do 
   {
    iKey=AskControl();
    
    if (iKey == 11) {iMovement= 1;} //Включить движение в сторону уменьшения значения ПВ
    if (iKey ==-11) {iMovement=-1;} //Включить движение в сторону увеличения значения ПВ
    if (iKey == 22) {iMovement= 2;} //Включить движение в сторону увеличения значения СК
    if (iKey ==-22) {iMovement=-2;} //Включить движение в сторону уменьшения значения СК
    
    if (iKey==-1 || iMovement==-1) // Назад на шаг
    {
      if(iRaD!=0)
       {
        RA_Stepper_step(-iRaD*32); //Множитель 32 (кол-во микрошагов за одну команду с пульта) можно изменять
        iLastMovement = -1;
       }
      else iMovement=0;
      bLCD=false; 
    };
    if (iKey==-2 || iMovement==-2) // Вниз
    {
      if(iDeD!=0)
      {
       DE_Stepper_step( iDeD*64); //Множитель 64 (кол-во микрошагов за одну команду с пульта) можно изменять
       iLastMovement = -2;
      }
      else iMovement=0;
      bLCD=false;
    };
    if (iKey==2 || iMovement== 2) // Вверх
    {
      if(iDeD!=0)
      {
      DE_Stepper_step(-iDeD*64); //Множитель 64 (кол-во микрошагов за одну команду с пульта) можно изменять
      iLastMovement  =  2;
      }
      else iMovement=0;
      bLCD=false;
    };
    if (iKey==1 || iMovement== 1) // Вперед на шаг
    {
      if(iRaD!=0)
      {
       RA_Stepper_step( iRaD*32); //Множитель 32 (кол-во микрошагов за одну команду с пульта) можно изменять
       iLastMovement =  1;
      }
      else iMovement=0;
      bLCD=false;
    };
   
   if (iKey== 3) if(!bRun) {bRun=true;  bLCD=false;} //Включить ведение  (Tracking ON)
   if (iKey==-3) if( bRun) {bRun=false; bLCD=false;} //Отключить ведение (Tracking OFF)
   
   if (iKey== 4) if(iDeD!= 1) {iDeD= 1; bLCD=false;} //Телескоп слева от полярной оси
   if (iKey==-4) if(iDeD!=-1) {iDeD=-1; bLCD=false;} //Телескоп справа от полярной оси

   if (iKey== 5) {if(iMovement!=0) iMovement = iLastMovement; else {iMovement=0; iLastMovement=0;}} // Play/Pause
   if (iKey==-5) {bRun=false; iMovement = 0; iLastMovement=0; iSteps=0; bLCD=false;}                // Стоп

   if (iKey==-6) if (iRaD!=-1) {iRaD=-1; bLCD=false;} //Телескоп в южном полушарии
   if (iKey== 6) if (iRaD!= 1) {iRaD= 1; bLCD=false;} //Телескоп в северном полушарии

   if (iKey== 7); //В данной версии программы не предусмотрено
   if (iKey==-7) {iMovement=0; bLCD=false;} //Сброс постоянных перемещений, кроме ведения

   if (iKey==-8) {FO_Stepper_step(-1); bLCD=false;} //Фокусировка (-)
   if (iKey== 8) {FO_Stepper_step( 1); bLCD=false;} //Фокусировка (+)
  
   if (iKey==10) {bAlignment=false; bLCD=false;} //Сброс выравнивания монтировки
 
   LCDCOR(iKey);
   
   } while (iKey!=0 || iMovement!=0);
 } 

void To_PRADEC(void)
{
  int DirectRA=0;
  int DirectDE=0;
  unsigned long uldRA=0;
  unsigned long uldDE=0;
  unsigned long ulStartMilis=millis();

  GetSubStr ();
  iToPRA=StrToHEX (STR1);
  iToPDE=StrToHEX (STR2);

  if (iToPRA > iPRA) {uldRA = (iToPRA-iPRA); DirectRA=  1;}
  if (iToPRA < iPRA) {uldRA = (iPRA-iToPRA); DirectRA= -1;}
  if (uldRA > RA_max_hex_value/2) {uldRA = RA_max_hex_value-uldRA; DirectRA = -(DirectRA);}

  if (iToPDE > iPDE) {uldDE = (iToPDE-iPDE); DirectDE=  1;}
  if (iToPDE < iPDE) {uldDE = (iPDE-iToPDE); DirectDE= -1;}
  if (uldDE > DEC_max_hex_value/2) {uldDE = DEC_max_hex_value-uldDE; DirectDE = -(DirectDE);}
  
  if (uldRA > RA_max_hex_value/2) return; //Ошибка в расчете шагов по прямому восхождению
  if (uldDE > DEC_max_hex_value/2) return; //Ошибка в расчете шагов по склонению
  
  while ((((uldRA > RA_STEPS_in_1_RA_MOTOR_STEP) && iRaD!=0) || ((uldDE > DEC_STEPS_in_1_RA_MOTOR_STEP) && iDeD!= 0)) && bAlignment)
  {
    if (uldRA > RA_STEPS_in_1_RA_MOTOR_STEP)
    {
      if (DirectRA > 0) {RA_Stepper_step(-iRaD); uldRA-=(RA_STEPS_in_1_RA_MOTOR_STEP+compensateEarth_dRA_STEPS_in_1_RA_MOTOR_STEP); iPRA+=(RA_STEPS_in_1_RA_MOTOR_STEP+compensateEarth_dRA_STEPS_in_1_RA_MOTOR_STEP);}
      if (DirectRA < 0) {RA_Stepper_step( iRaD); uldRA-=(RA_STEPS_in_1_RA_MOTOR_STEP-compensateEarth_dRA_STEPS_in_1_RA_MOTOR_STEP); iPRA-=(RA_STEPS_in_1_RA_MOTOR_STEP-compensateEarth_dRA_STEPS_in_1_RA_MOTOR_STEP);}
      if (uldRA > RA_max_hex_value/2) uldRA =0;
    }
    if (uldDE > DEC_STEPS_in_1_RA_MOTOR_STEP)
    {
      if (DirectDE > 0) {DE_Stepper_step(-iDeD); uldDE-=(DEC_STEPS_in_1_RA_MOTOR_STEP+compensateEarth_dDEC_STEPS_in_1_RA_MOTOR_STEP); iPDE+=(DEC_STEPS_in_1_RA_MOTOR_STEP+compensateEarth_dDEC_STEPS_in_1_RA_MOTOR_STEP);}
      if (DirectDE < 0) {DE_Stepper_step( iDeD); uldDE-=(DEC_STEPS_in_1_RA_MOTOR_STEP+compensateEarth_dDEC_STEPS_in_1_RA_MOTOR_STEP); iPDE-=(DEC_STEPS_in_1_RA_MOTOR_STEP+compensateEarth_dDEC_STEPS_in_1_RA_MOTOR_STEP);}
      if (uldDE > DEC_max_hex_value/2) uldDE =0;
    }
   if ((millis()-ulStartMilis)>1000) {bLCD=false; LCDPrint(); STR="e"; action(STR); ulStartMilis=millis();}
  }
  if (!bAlignment)  //Первая команда GOTO задает координаты наведения телескопа, без его реального перемещения
  {
    iPRA=iToPRA;
    iPDE=iToPDE;
    bAlignment=true;
  }
  bLCD=false;
 };

void action(String STRA)
{
  char cAction;
  cAction=STRA.charAt(0);
  switch (cAction)
  {
    case 'e': {
		Serial.print(HexTo8D(iPRA)); 
		Serial.print(iPRA,HEX); 
		Serial.print(","); 
		Serial.print(HexTo8D(iPDE)); 
		Serial.print(iPDE,HEX); 
		Serial.print("#"); 
		break;
		}
    case 'r': {
		To_PRADEC(); 
		Serial.print("#"); 
		break;
		}
  };
}

void setup()
{
  //lcd.init();           
  //lcd.backlight();
  pinMode(A6, INPUT); // Проводной пульт входной контакт 1
  pinMode(A7, INPUT); // Проводной пульт входной контакт 2
  pinMode(LIGHT_SENCE_APIN, INPUT); // Вход датчика освещенности
  pinMode(LCD_LIGHT_PIN, OUTPUT); //Вывод на управление подсветкой LCD
  SetLCDLight(); //Устанавливаем яркость LCD
  pinMode(RA_STEP_PIN,  OUTPUT); // RA STEP PIN
  digitalWrite(RA_STEP_PIN, LOW);
  pinMode(RA_DIR_PIN,  OUTPUT);  // RA DIR PIN
  digitalWrite(RA_DIR_PIN, LOW);
  pinMode(FO_STEP_PIN,  OUTPUT); // FO STEP
  digitalWrite(FO_STEP_PIN, LOW);
  pinMode(FO_DIR_PIN,  OUTPUT); // FO DIR
  digitalWrite(FO_DIR_PIN, LOW);
  pinMode(DE_STEP_PIN,  OUTPUT); // DE STEP
  digitalWrite(DE_STEP_PIN, LOW);
  pinMode(DE_DIR_PIN,  OUTPUT); // DE DIR
  digitalWrite(DE_DIR_PIN, LOW);
  pinMode(13,  OUTPUT);  // Запасной индикатор
  digitalWrite(13, LOW); // Выключен
  LCDPrint();
  Serial.begin(9600);
  Serial.flush();
  iMilisec=millis();
}

void loop()
{
 STR = GetString();
 action(STR);
 reaction();
 if((millis()-iMilisec)>=RA_rotate_starspeed__ms_period_of_tick)
  {
   iMilisec+=RA_rotate_starspeed__ms_period_of_tick;
   if (bRun) RA_Stepper_step(iRaD); else if (bAlignment) {iPRA+=RA_STEPS_in_1_RA_MOTOR_STEP; bLCD=false;}
  }
 LCDPrint();
}



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

