int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
byte RXdata; 
byte state = 00000000; //contains the data for controlling the motor RXdataions
void setup()
{  
  state = state&0;
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);    
  pinMode(6, OUTPUT);//camera motor supply
  pinMode(5, OUTPUT);//l293 logic input
  pinMode(4, OUTPUT);//l293 logic input
  digitalWrite(latchPin, LOW);
  moving_direction('S');
  digitalWrite(5,LOW); digitalWrite(4,LOW);
  Serial.begin(9600);
}
//------------------------------------------------------------
// LOOP!
//------------------------------------------------------------
void loop()
{  
  state = state&0;
  RX();
  delay(150);
  //Serial.println(RXdata,BIN);
  //delay(300);
}
//************************************************************
//------------------------------------------------------------
void RX()
{
  if(Serial.available()>0)
  {
    RXdata = Serial.read(); 
    if(bitRead(RXdata,7) == 1){moving_direction('S'); Serial.println('S');}   
    if(bitRead(RXdata,6) == 1){moving_direction('F'); Serial.println('F');}   
    if(bitRead(RXdata,5) == 1){moving_direction('B'); Serial.println('B');}
    if(bitRead(RXdata,4) == 1){moving_direction('R'); Serial.println('R');}
    if(bitRead(RXdata,3) == 1){moving_direction('L'); Serial.println('L');}  
    
    camera_motor(RXdata);
  }
}
//************************************************************
//------------------------------------------------------------
void camera_motor(byte RXdata)
{
  if(bitRead(RXdata,2) == 1)
  { 
    if(bitRead(RXdata,1) == 1){digitalWrite(5,HIGH); digitalWrite(4,LOW);}
    if(bitRead(RXdata,0) == 1){digitalWrite(5,LOW); digitalWrite(4,HIGH);}
  }
  else {digitalWrite(5,LOW); digitalWrite(4,LOW); return;}  
}
//************************************************************
//------------------------------------------------------------
void moving_direction(char direct)
{
  switch(direct)
  {
    case 'F': state = state|202; break;
    case 'B': state = state|53; break;
    case 'L': state = state|43; break;
    case 'R': state = state|212; break;
    case 'S': state = state|0; break;
  }
  digitalWrite(latchPin,LOW);
  //shift out the bits:
  shiftOut(dataPin,clockPin,MSBFIRST,state);    
  digitalWrite(latchPin,HIGH);
}
//*******************END moving_direction(char)*********************
