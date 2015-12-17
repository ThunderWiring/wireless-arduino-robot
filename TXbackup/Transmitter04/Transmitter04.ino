// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>

int verticalR=0, horizontalR =0, verticalL=0,horizontalL=0;
byte data = 0, data2=1;
int potValue=0, previousPotValue=0, potValueDifference=0;
boolean clockwise, anticlock, enable = true;
int upperToggle, sideToggle;

long previousMillis = 0;   
long interval = 100; 
unsigned long currentMillis;//= millis();
void setup()
{
  
  Serial.begin(9600);  
  Serial.write(0);
  pinMode(0,INPUT); // vertical right
  pinMode(1,INPUT); // horizontal right
  pinMode(2,INPUT); //vertical left
  pinMode(3,INPUT); // horizontal left.
  pinMode(4,INPUT); // potentometer
  pinMode(2,INPUT); // upper toggle
  pinMode(3,INPUT); // side toggle
  previousPotValue = analogRead(4);
  bitSet(data2,0);
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
 
  // set compare match register to desired timer count:
  OCR1A = 1561.5;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
  sei();
}
ISR(TIMER1_COMPA_vect)
{
    transmitt();   
    Serial.flush();
}
void loop()
{
  currentMillis = millis();
  data = 0;
  data2=0;
  read_data();
  /**/
  //Serial.print("previousPotValue: "); Serial.print(previousPotValue);
  //Serial.print("d7 = "); Serial.println(digitalRead(7));
  //Serial.println(data,BIN);
}
//************************************************************
//------------------------------------------------------------
void read_data()
{
  verticalR  =analogRead(0);
  horizontalR = analogRead(1);
  verticalL = analogRead(2);
  horizontalL = analogRead(3);
  upperToggle = digitalRead(2);
  sideToggle  = digitalRead(3);
  
  previousPotValue = potValue;
  potValue = analogRead(4);
  potValueDifference = potValue - previousPotValue; 
  compare();
}
//************************************************************
//------------------------------------------------------------
void transmitt()
{
  previousMillis = currentMillis; 
  if(verticalR<30)        {bitSet(data,3); data = data&65295;} //left
  else if(verticalR>900)  {bitSet(data,4); data = data&8183;} //right
  else if(horizontalR<30) {bitSet(data,6); data = data&32711;} //forward
  else if(horizontalR>900){bitSet(data,5); data = data&16359;} //backward
  else {bitSet(data,7); data = data&32647;}
  
  if(upperToggle == 1)bitSet(data,2);
  if(sideToggle == 1) bitSet(data,1);  
  
  if(enable)
  {
    bitSet(data2,3);
    if(clockwise) bitSet(data2,2);
    if(anticlock) bitSet(data2,1);
  }
  else data2 = data2&249;
  
  Serial.write(data);
  Serial.flush();
  Serial.write(data2);
  Serial.flush(); 
}
//************************************************************
//------------------------------------------------------------
void compare()
{
  if(potValueDifference<=3&&potValueDifference>=-3) 
  {
    enable = false;
    clockwise = false; anticlock = false;
  }
  else
  {
    enable = true;
   if(potValueDifference>=4) {clockwise = true; anticlock = false;}
   if(potValueDifference<=-4){clockwise = false; anticlock = true;}
  }  
}
