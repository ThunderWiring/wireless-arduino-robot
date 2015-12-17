//autonomus mission:
/*1-travel forward.
  2-if obstacle got identified: get around it*/
//----------------------------------------------------------------#include <Servo.h> 
#include <Servo.h>
Servo front_servo; 
/*shift register & motor*/
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
char direct; //indicating direction: F,B,L,R,S(capital letter).
byte state = 00000000; //contains the data for controlling the motor directions
/*sensor & distance*/
int sensorPinA = 4, sensorPinB = 1; //ir sensors attach pins on arduino. A=right, B=left
int sensorValueA = 0, sensorValueB = 0; // ir sensor analog input.
int minDistance = 350; // minimum distance allowed between robot and object
int firstReading = 1; // checks if its the first time we read from the sensor, since its not accurate
boolean pathClear = true; //true:path is clear of obstales. false: presence of obstacle
int counter1 = 0,/*for level 1*/ counter2 = 0 /*for level 2*/ ;
int value=0;
boolean turnAround = false;
void setup()
{
  front_servo.attach(9);
  front_servo.write(90); //positioning front servo to center
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
  digitalWrite(13,LOW);
  digitalWrite(latchPin, LOW); 
  Serial.begin(9600);
}
//------------------------------------------------------------
// LOOP!
//------------------------------------------------------------
void loop()
{
  digitalWrite(2, LOW);//right led
  digitalWrite(3, LOW);//left  led
  digitalWrite(4, LOW);//front led
  readSensors(); 
  pathClear = isClear();
  /*Serial.print("sensor A = "); Serial.print(sensorValueA);
  Serial.print(" sensor B = "); Serial.println(sensorValueB);*/
  //Serial.print(" pathClear = "); Serial.println(pathClear);  
  state = state&0;      
  if(pathClear) 
  {
    moving_direction('F');
    digitalWrite(2, LOW);//right led
    digitalWrite(3, LOW);//left  led
    digitalWrite(4, HIGH);//front led
  }
  else if(pathClear == false) get_around();
}
//**********************END loop()*****************************
/*-------------------------------------------------------------
  avoiding objet by getting around it.
  input: using information from the other functions.
  output: getting aroung the obstacle object
-------------------------------------------------------------*/
void get_around()
{  
  avoid();   //LEVEL 1
  evade();   //LEVEL 2
  adjust();  //LEVEL 3
  bypass();  //LEVEL 4 
  //dispatch();//LEVEL 5 
  digitalWrite(2, LOW);//right led
  digitalWrite(3, LOW);//left  led
  digitalWrite(4, LOW);//front led
  return;
}
//**********************END get_around()***********************
/*-------------------------------------------------------------
  avoiding objet by turning right/left ****LEVEL 1****
  input: using sensor reading to avoid obstacle
  output: turning right/left
-------------------------------------------------------------*/
void avoid()
{
  digitalWrite(2, HIGH);//right led
  digitalWrite(3, LOW);//left  led
  digitalWrite(4, LOW);//front led
  state = state&0;
  moving_direction('R');
  front_servo.write(90); 
  counter1 ++;
  //Serial.println(counter1);
  readSensors();
  pathClear = isClear();
  if(pathClear == false) avoid();
  else return;
}
//*********************END avoid()*****************************
/*-------------------------------------------------------------
  evading objet by traveling forward beside it ****LEVEL 2****
  input: using sensor reading to avoid obstacle + servo side tilt
  output: traveling forward beside obstacle => evading it.
-------------------------------------------------------------*/
void evade()
{
  digitalWrite(2, LOW);//right led
  digitalWrite(3, LOW);//left  led
  digitalWrite(4, HIGH);//front led
  front_servo.write(45);
  state = state&0;  
  moving_direction('F');  
  counter2 ++;  
  readSensors();
  pathClear = isClear();
  if(pathClear == false) evade();
  else return;  
}
//**********************END evade()****************************
/*-------------------------------------------------------------
  returning to initial rute by turning left/right ****LEVEL 3****
  input: using sensor reading to avoid obstacle + servo side tilt
  output: turning left/right.
-------------------------------------------------------------*/
void adjust()
{
  digitalWrite(13, HIGH);
  digitalWrite(2, LOW);//right led
  digitalWrite(3, HIGH);//left  led
  digitalWrite(4, LOW);//front led
  state = state&0;  
  for(int i = 1; i<= counter1; i++) 
  {
    //Serial.println(i);
    moving_direction('L');
    delay(100);
    front_servo.write(90);    
  }   
}
//**********************END adjust()***************************
/*-------------------------------------------------------------
  travel forward to bypass obstacle completly ****LEVEL 4****
  input: counter2  
  output: turning left/right.
-------------------------------------------------------------*/
void bypass()
{
  digitalWrite(2, LOW);//right led
  digitalWrite(3, LOW);//left  led
  digitalWrite(4, HIGH);//front led
  state = state&0;   
  for(int i = 1; i<= counter2; i++) 
  {
    moving_direction('F');
    delay(100);
    front_servo.write(90);
  }
}
//**********************END bypass()***************************
/*-------------------------------------------------------------
  returning to initial position angle via turning opposite to level3 ****LEVEL 5****
  input: sensor reading to avoid obstacle + servo side tilt
  output: turning left/right.
-------------------------------------------------------------*/
void dispatch()
{
  digitalWrite(2, LOW);//right led
  digitalWrite(3, HIGH);//left  led
  digitalWrite(4, LOW);//front led
  state = state&0;   
  for(int i = 1; i<= counter1; i++) 
  {
    moving_direction('L');
    delay(100);
    front_servo.write(90);
  }
}
//**********************END dispatch()***************************
/*-------------------------------------------------------------
  reading sensor values & comparing them.
  input: readings for sensorValueA, sensorValueB from analog values from ir sensors
  output: true:path is clear of obstales. false: presence of obstacle
-------------------------------------------------------------*/
void readSensors()
{
  delay(100);
  if(firstReading >=3)
  {    
    firstReading++;
    return;
  }
  sensorValueA = analogRead(sensorPinA);
  sensorValueB = analogRead(sensorPinB); 
}
//*******************END readSensors()*********************
/*-------------------------------------------------------------
  comparing the current sensor values
  input: readings for sensorValueA, sensorValueB from analog values from ir sensors
  output: true:path is clear of obstales. false: presence of obstacle
-------------------------------------------------------------*/
boolean isClear()
{
  int distanceAvg = (sensorValueA+sensorValueB)/2;
  if(distanceAvg <= minDistance) return true;
  else return false;  
}
//*******************END isClear()*************************
/*-------------------------------------------------------------
  direction control function:
  input: char indicating direction(direct): F,B,L,R,S(capital letter)=>forward, backward,left,right,stop
  output: moving motors accordingly.
-------------------------------------------------------------*/
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
