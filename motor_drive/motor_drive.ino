#include <Servo.h> 
 
Servo myservo;
int ledPin = 13;
//ir sensor
int sensorPin0 = 4;
int sensorPin1 = 1;
int sensorRightValue=0,sensorLeftValue=0; //reading sensor data
int leftDistance=0, rightDistance=0; //distances on either side
boolean range; // true if there isnt a close object, false if there is object infront of robot
//joystick
int vertical = 0;
int horizental = 0;
boolean manual = false;
//shift register:
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
byte state = 00000000; //contains the data for controlling the motor directions

void setup()
{
  pinMode(sensorPin0,INPUT);
  pinMode(sensorPin1,INPUT);
  pinMode(ledPin,OUTPUT);
  digitalWrite(13,LOW);
  myservo.attach(9);
  myservo.write(90); //set pan servo to center
  
  //set all the pins used to talk to the chip
  //as output pins so we can write to them
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //take the latchPin low so the motors don't
  //change while we are writing data
  digitalWrite(latchPin, LOW);
   
  Serial.begin(9600);
}

void loop()
{ 
  state = state&0; 
  /*Serial.println(manual);
  if(digitalRead(10)==LOW) manual= true;
  if(manual)
  {
    blinkLED();    
    manualControl(); 
  }*/
  sensorRightValue = analogRead(sensorPin0);
  sensorLeftValue = analogRead(sensorPin1); 
  Serial.println(sensorRightValue);  
  delay(500);
  if((sensorRightValue+sensorLeftValue)/2 <300) forward(); //no obstacle in sight:defult motion is set to forwad    
  else sonarScan(); //
  //there is an obstacle in sight(path is blocked)  
}

//reads sensor data from either side.
void sonarScan()
{
  Stop();
  myservo.write(45); //scan to the right
  delay(500);
  rightDistance = analogRead(sensorPin0); 
  delay(500);
  myservo.write(135); //scan to the left
  delay(700);
  leftDistance = analogRead(sensorPin0);
  delay(500);
  myservo.write(90); //return to center
  delay(100);
  compareDistance();  
  return;
}

void compareDistance()
{
  if (leftDistance>rightDistance) //if left is less obstructed 
  {
    left(); //turn left 
    return;
  }
  else if (rightDistance>leftDistance) //if right is less obstructed
  {
    right(); //turn right
    return;
  }
   else //if they are equally obstructed
  {
    Stop();
    blinkLED();
    manualControl();
    return;
  }
}

void blinkLED()
{
  digitalWrite(13,HIGH);
  delay(50);
  digitalWrite(13,LOW);   
  delay(200);
}

// control the robot manually by joystick
void manualControl()
{
  vertical = analogRead(0);
  horizental=analogRead(2);
  Serial.println(vertical);
  if(vertical>1000) forward(); 
  else if(vertical<500) backward();
  else if(horizental > 1000)  right();
  else if(horizental < 500)  left();
  else Stop();
}

//**********************************************************************
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
