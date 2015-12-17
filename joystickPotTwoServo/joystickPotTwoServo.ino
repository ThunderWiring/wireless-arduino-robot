#include <Servo.h> 
 
Servo servoA;  // create servo object to control a servo 
Servo servoB;  // create servo object to control a servo 
 
int potpin = 0;  // analog pin used to connect the potentiometer
int valuePot =0;    // variable to read the value from the analog pin 
int verticalPin=2; //joystick
int horizontalPin=3;//joyystick
int button= 7;
float verticalValue=0;
float horizontalValue=0;
boolean joystick=true;
boolean pot = false;

void setup() 
{ 
  servoA.attach(9);  // attaches the servo on pin 9 to the servo object 
  servoB.attach(8);
  pinMode(potpin,INPUT);
  pinMode(verticalPin,INPUT);
  pinMode(horizontalPin,INPUT);
  pinMode(7,INPUT);
} 
 
void loop() 
{ 
  if(digitalRead(button)==HIGH&&joystick==true&&pot==false)
  {
    servoPot();
  }
  else if(digitalRead(button)==HIGH&&joystick==false&&pot==true)
  {
    servojoystick();
  }
  
} 
void servojoystick()
{
   
    verticalValue = analogRead(verticalPin);            // reads the value of the potentiometer (value between 0 and 1023) 
    verticalValue = map(verticalValue, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
    servoA.write(verticalValue);                  // sets the servo position according to the scaled value 
    delay(15);                           // waits for the servo to get there 
    
    horizontalValue = analogRead(horizontalPin);            // reads the value of the potentiometer (value between 0 and 1023) 
    horizontalValue = map(horizontalValue, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
    servoB.write(horizontalValue);                  // sets the servo position according to the scaled value 
    delay(15); 
    
}
void servoPot()
{
  valuePot = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
    valuePot = map(valuePot, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
    servoA.write(valuePot);                  // sets the servo position according to the scaled value 
    servoB.write(valuePot);      
    delay(15); 
}
