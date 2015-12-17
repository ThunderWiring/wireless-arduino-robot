#include <NewPing.h>
// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>

#define trigPin  3  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define echoPin     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
unsigned int ultraSonic;
NewPing sonar(trigPin, echoPin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
#define SERVO_PIN  13 
//----------Servo variables-----------------
int lenMicroSecondsOfPeriod = 20 * 1000; // 20 milliseconds (ms)
int lenMicroSecondsOfPulse = 0; // used in the 'for' loop below
int increment = 0.01 * 1000;
int currentServoPosition = 1.34 * 1000; // middle position, 90 degrees.
int ctrl  = 0; //recognizing first time entering the servo method.
//----------END of Servo variables-----------------
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
word RXdata = 0,RXdata2 = 0;
byte state = 00000000; //contains the data for controlling the motor RXdataions
int irLeft = 0, irRight = 0, ultrasonic = 0; // ALL distance sensors' values.
int firstReading = 1; // checks if its the first time we read from the sensor, since its not accurate
int leftDistance,rightDistance;
int distanceAvg;
//--------blinking without delay--------
int ledState = LOW; 
unsigned long currentMillis;
unsigned long currentMillis_led;
long previousMillis_led = 0;   
long interval_led = 200; 
//----------------//------//-------//-----
int minDistance = 19; // irSensor:minimum distance[cm] allowed between robot and object
boolean manual=false, autonomus=false;

byte shiftLED = 0; //for the 2nd shiftregister leds
//**********************variables*******************************************
void setup()
{  
  state = state&0;
  pinMode(SERVO_PIN, OUTPUT);  
  servoWrite(90);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);    
  pinMode(6, OUTPUT);//camera motor supply
  pinMode(5, OUTPUT);//l293 logic input
  pinMode(10, OUTPUT);//l293 logic input
  pinMode(1, INPUT);//ir sensor
  pinMode(4, INPUT);//ir sensor
  pinMode(3,OUTPUT); // trigPin - ulrasonic sensor
  pinMode(2,INPUT); // echoPin - ulrasonic sensor
  pinMode(6, OUTPUT);// led pin
  pinMode(7, OUTPUT);// led pin
  pinMode(9, OUTPUT);// led pin
  pinMode(2,OUTPUT);//analog test pwm
  pinMode(4, OUTPUT); //isClear() indication LED.
  digitalWrite(latchPin, LOW);
  moving_direction('S');
  irLeft =0; 
  irRight = 0;
  Serial.begin(9600);
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 858;
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
  RX();
  Serial.flush();

}
//------------------------------------------------------------
// LOOP!
//------------------------------------------------------------
void loop()
{  
  currentMillis = millis();
  state = state&0;  
  shiftLED = shiftLED & 0;

  if(autonomus) {
    auto_drive();
  }
  if (manual) {
    manual_drive(RXdata);
  }
}
//************************************************************
//------------------------------------------------------------
void RX()
{  
  if(Serial.available()>=1)
  {     
    RXdata = Serial.read(); 
    Serial.flush();     
    RXdata2 = Serial.read();
    Serial.flush(); 
    blinkWithoutDelay(6);

    if(bitRead(RXdata,2) == 1) {
      digitalWrite(7,HIGH);
      autonomus=true;
      manual=false;
    }//auto_drive();} 
    if(bitRead(RXdata,2) == 0) {
      digitalWrite(7,LOW);
      manual=true;
      autonomus=false;
    }
    if(bitRead(RXdata,1) == 1) digitalWrite(9,HIGH);
    if(bitRead(RXdata,1) == 0) digitalWrite(9,LOW);        
    camera_motor(RXdata2);

    return;    
  }
}
//************************************************************
//------------------------------------------------------------
void manual_drive(byte RXdata)
{
  servoWrite(90);
  if(bitRead(RXdata,7) == 1){
    moving_direction('S'); /*Serial.println('S');*/
  }   
  if(bitRead(RXdata,6) == 1){
    moving_direction('F'); /*Serial.println('F');*/
  }   
  if(bitRead(RXdata,5) == 1){
    moving_direction('B'); /*Serial.println('B');*/
  }
  if(bitRead(RXdata,4) == 1){
    moving_direction('R'); /*Serial.println('R');*/
  }
  if(bitRead(RXdata,3) == 1){
    moving_direction('L'); /*Serial.println('L');*/
  }
  return;
}
//************************************************************
//------------------------------------------------------------
void camera_motor(byte RXdata)
{
  if(bitRead(RXdata,3) == 0){
    digitalWrite(5,LOW); 
    digitalWrite(10,LOW);
    return;
  } 
  else if(bitRead(RXdata,3) == 1 && bitRead(RXdata,2) == 1)
  {
    digitalWrite(5,HIGH); 
    digitalWrite(10,LOW);
    return;
  }
  else if(bitRead(RXdata,3) == 1 && bitRead(RXdata,1) == 1)
  {
    digitalWrite(5,LOW); 
    digitalWrite(10,HIGH);
    return;
  }  
}
//************************************************************
//------------------------------------------------------------
void moving_direction(char direct)
{
  switch(direct)
  {
  case 'F': 
    state = state|202; 
    break;
  case 'B': 
    state = state|53; 
    break;
  case 'L': 
    state = state|43; 
    break;
  case 'R': 
    state = state|212; 
    break;
  case 'S': 
    state = state|0; 
    break;
  }
  registerWrite(state,shiftLED);
}
//*******************END moving_direction(char)***************
//------------------------------------------------------------
void registerWrite(byte registerOne, byte registerTwo)
{
  digitalWrite(latchPin,LOW);
  //shift out the bits:
  shiftOut(dataPin,clockPin,MSBFIRST,registerTwo);    
  shiftOut(dataPin,clockPin,MSBFIRST,registerOne);    
  digitalWrite(latchPin,HIGH);
}
//************************************************************
//------------------------------------------------------------
void auto_drive()
{
  //boolean x = isClear();

  if(isClear() == true)
  {
    digitalWrite(4,HIGH);
    servoWrite(90);    
    moving_direction('F');    
  }
  else if(isClear() == false)  
  {
    digitalWrite(4,LOW);
    moving_direction('S');    
    servoWrite(45);  
    if(isClear()) moving_direction('L');  
    else
    {    
      servoWrite(135);  
      if(isClear()) {moving_direction('R');  }
      else
      {
        moving_direction('B'); delay(200);
        if(ultraSonic < minDistance){moving_direction('R'); delay(500);}
        else{moving_direction('L'); delay(500);}
      }
    }
  }  
}
//************************************************************
//------------------------------------------------------------
// reads sensors' value.
//------------------------------------------------------------
void readSensors()
{
  /*for( int i  = 1; i<4; i++)
   {irLeft = analogRead(1);irRight = analogRead(4);}*/

  //reading ir sensor values.
  irLeft = (6762/(analogRead(1)-9))-4;
  irRight = (6762/(analogRead(4)-9))-4;
  distanceAvg = (irLeft+irRight)/2;
  //Serial.print(irLeft); Serial.print(" "); Serial.println(irRight);
  //reading ultrasonic sensor value , distance in [cm].
  ultraSonic = sonar.ping_cm();  
}
//***********************END detect_objects*********************
/*-------------------------------------------------------------
 comparing the current sensor values
 input: readings for sensorValueA, sensorValueB from analog values from ir sensors
 output: true:path is clear of obstales. false: presence of obstacle
 -------------------------------------------------------------*/
boolean isClear()
{
  int t = 0,f = 0; 
  boolean cleared = true;  
  for(int i = 0; i<4;i++)
  {
    readSensors();
    if(distanceAvg < minDistance) f++;
    else t++;
  }
  if(t > f) cleared = true;
  else cleared = false;
    

  /*readSensors();
  if(distanceAvg < minDistance) cleared = false;
  if(distanceAvg > minDistance) cleared = true;*/

  /*Serial.print(irLeft); 
  Serial.print(" "); 
  Serial.print(irRight); 
  Serial.print("    "); 
  Serial.println(cleared);*/
  return cleared;
}
//*******************END isClear()*************************
//------------------------------------------------------------
// blink LED. INPUT = LED pin
//------------------------------------------------------------
void blinkWithoutDelay(int ledPin)
{   
  currentMillis_led = millis();
  if(currentMillis - previousMillis_led > interval_led) {
    // save the last time you blinked the LED
    previousMillis_led = currentMillis;  

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}
//******************END blinkWithoutDelay******************
// controlling the servo without the library.
//---------------------------------------------------------
void servoWrite(int degree)
{
  if(ctrl ==  0) {
    currentServoPosition = 0; 
    ctrl++;
  }
  switch(degree)
  {
  case 0 : 
    lenMicroSecondsOfPulse = 0.50 * 1000; 
    break;
  case 45: 
    lenMicroSecondsOfPulse = 0.85 * 1000; 
    break;
  case 90: 
    lenMicroSecondsOfPulse = 1.34 * 1000; 
    break;
  case 135: 
    lenMicroSecondsOfPulse = 1.85 * 1000; 
    break;
  case 180: 
    lenMicroSecondsOfPulse = 2.20 * 1000; 
    break;
  }    
  if(lenMicroSecondsOfPulse > currentServoPosition)
  {
    for(int i = currentServoPosition; i<lenMicroSecondsOfPulse; i += increment)
    {
      // Turn voltage high to start the period and pulse
      digitalWrite(SERVO_PIN, HIGH);
      // Delay for the length of the pulse
      delayMicroseconds(i);
      // Turn the voltage low for the remainder of the pulse
      digitalWrite(SERVO_PIN, LOW);
      // Delay this loop for the remainder of the period so we don't
      // send the next signal too soon or too late
      delayMicroseconds(lenMicroSecondsOfPeriod - lenMicroSecondsOfPulse); 
    }
  }
  else
  {
    for(int i = currentServoPosition; i>lenMicroSecondsOfPulse; i -= increment)
    {
      // Turn voltage high to start the period and pulse
      digitalWrite(SERVO_PIN, HIGH);
      // Delay for the length of the pulse
      delayMicroseconds(i);
      // Turn the voltage low for the remainder of the pulse
      digitalWrite(SERVO_PIN, LOW);
      // Delay this loop for the remainder of the period so we don't
      // send the next signal too soon or too late
      delayMicroseconds(lenMicroSecondsOfPeriod - lenMicroSecondsOfPulse); 
    }
  }  
  currentServoPosition = lenMicroSecondsOfPulse;
  return;
}
//***********************END servo write************************

