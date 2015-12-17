#include <Servo.h> 
 
Servo myservo;
int sensorPin0 = 0;
int sensorPin1 = 1;
int ledPin = 13;
float sensorRightValue;
float sensorLeftValue;
boolean range; // true if there is a close object, false if there is no object infront of robot
void setup()
{
  Serial.begin(9600);
  pinMode(sensorPin0,INPUT);
  pinMode(sensorPin1,INPUT);
  pinMode(13,OUTPUT);
  myservo.attach(9);
  myservo.write(0);
}
void loop()
{
        
  sensorRightValue = analogRead(sensorPin0);
  sensorLeftValue = analogRead(sensorPin1);
  range = distance(sensorRightValue,  sensorLeftValue);
  
  Serial.print("sensor 1 = ");
  Serial.print( sensorRightValue);
  Serial.print( ", sensor2 = " );
  Serial.print(sensorLeftValue);
  Serial.println();
   
  if(range)
  {
    turnRight();
    sensorRightValue = analogRead(sensorPin0);
    sensorLeftValue = analogRead(sensorPin1);
    range = distance(sensorRightValue,  sensorLeftValue);
    if(range) 
    {
      turnLeft();
      sensorRightValue = analogRead(sensorPin0);
      sensorLeftValue = analogRead(sensorPin1);
      range = distance(sensorRightValue,  sensorLeftValue);
      if(range)
      {
        Serial.println("Manual");
        turnToMiddle('L');
      }
      else
      {
        blinkLED();
        // move robot to the left
        Serial.println("move to the left");
        //return servo to middle position
        turnToMiddle('L');
      }
    }
    else
    {
      blinkLED();
      // move robot to the right
      Serial.println("move to the right");
      //return to middle potition
      turnToMiddle('R');
    }
  }
  else blinkLED();
  
}

// return true if the ir sensors show close range, else, if the 
// object is still far, return false.
boolean distance(float sensorRightValue, float sensorLeftValue)
{
  if(sensorRightValue>=400 && sensorRightValue<700 || sensorLeftValue>=400 && sensorLeftValue <700)
  {
    return true;
  }
  else return false;
}


void turnRight()
{
  int pos=0;
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  delay(500);
}
void turnLeft()
{
  int pos = 0;
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
    delay(500);
}

void turnToMiddle(char currentDirection)
{
  if(currentDirection == 'L')
  {
    int pos=0;
    for(pos = 1; pos < 90; pos += 1)  // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    } 
    delay(500);
  }
  else if(currentDirection == 'R')
  {
    int pos = 0;
    for(pos = 179; pos>=90; pos-=1)     // goes from 180 degrees to 0 degrees 
    {                                
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    } 
      delay(500);
  }
}

void blinkLED()
{
    digitalWrite(13,HIGH);
    delay(400);
    digitalWrite(13,LOW);
    digitalWrite(13,HIGH);
    delay(400);
    digitalWrite(13,LOW);
 
}

