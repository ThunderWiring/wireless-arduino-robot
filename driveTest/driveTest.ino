int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
byte state = 00000000; //contains the data for controlling the motor directions
int vertical = 0;
int horizental = 0;

void setup()
{
  //set all the pins used to talk to the chip
  //as output pins so we can write to them
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //take the latchPin low so the motors don't
  //change while we are writing data
  digitalWrite(latchPin, LOW);
}
void loop()
{
  state = state&0;
  Stop();
  delay(2000);
  left();
  delay(5000);
  
}
void forward()
{
  state = state|202; //11001010
  digitalWrite(latchPin,LOW);
  //shift out the bits:
  shiftOut(dataPin,clockPin,MSBFIRST,state);    
  digitalWrite(latchPin,HIGH);
}

void backward()
{
  state = state|53; //00110101
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,state);  
  digitalWrite(latchPin,HIGH);  
}

void right()
{
  state = state|212; //11010100
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,state);  
  digitalWrite(latchPin,HIGH);  
}

void left()
{
  state = state|43; //00101011
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,state);  
  digitalWrite(latchPin,HIGH);  
}

void Stop()
{
  state = state|0; //00000000
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,state);  
  digitalWrite(latchPin,HIGH);  
}
