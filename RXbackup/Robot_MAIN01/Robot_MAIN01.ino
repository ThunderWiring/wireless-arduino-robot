int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
byte direct; 
byte state = 00000000; //contains the data for controlling the motor directions
void setup()
{  
  state = state&0;
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);    
  pinMode(2, OUTPUT);//right led
  pinMode(3, OUTPUT);//left  led
  pinMode(4, OUTPUT);//front led
  digitalWrite(2, LOW);//right led
  digitalWrite(3, LOW);//left  led
  digitalWrite(4, LOW);//front led
  pinMode(13, OUTPUT);//led
  digitalWrite(latchPin, LOW);
  moving_direction('S');
  Serial.begin(9600);
}
//------------------------------------------------------------
// LOOP!
//------------------------------------------------------------
void loop()
{  
  state = state&0;
  RX();  
}
//************************************************************
//------------------------------------------------------------
void RX()
{
  if(Serial.available()>0)
  {
    direct = Serial.read(); 
    if(bitRead(direct,7) == 1){moving_direction('S'); Serial.println('S');}   
    if(bitRead(direct,6) == 1){moving_direction('F'); Serial.println('F');}   
    if(bitRead(direct,5) == 1){moving_direction('B'); Serial.println('B');}
    if(bitRead(direct,4) == 1){moving_direction('R'); Serial.println('R');}
    if(bitRead(direct,3) == 1){moving_direction('L'); Serial.println('L');}
    /*switch(direct)
    {
      case 'F': moving_direction('F'); Serial.println('F'); break;
      case 'B': moving_direction('B'); Serial.println('B');  break;
      case 'R': moving_direction('R'); Serial.println('R');  break;
      case 'L': moving_direction('L'); Serial.println('L');  break;
      case 'S': moving_direction('S'); Serial.println('S');  break;
    }*/
  }
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
