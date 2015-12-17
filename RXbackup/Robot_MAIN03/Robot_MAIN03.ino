int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
word RXdata = 0,RXdata2 = 0;
byte state = 00000000; //contains the data for controlling the motor RXdataions
int irLeft = 0, irRight = 0, ultrasonic = 0; // ALL distance sensors' values.
int trigPin = 3;
int echoPin = 2;
/////blinking without delay/////
int ledState = LOW;        
long previousMillis = 0;   
long interval = 200; 

int minDistance = 350; // irSensor:minimum distance allowed between robot and object
//**********************variables*******************************************
void setup()
{  
  state = state&0;
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
  delay(150);
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
    
    if(bitRead(RXdata,7) == 1){moving_direction('S'); Serial.println('S');}   
    if(bitRead(RXdata,6) == 1){moving_direction('F'); Serial.println('F');}   
    if(bitRead(RXdata,5) == 1){moving_direction('B'); Serial.println('B');}
    if(bitRead(RXdata,4) == 1){moving_direction('R'); Serial.println('R');}
    if(bitRead(RXdata,3) == 1){moving_direction('L'); Serial.println('L');}
  
    if(bitRead(RXdata,2) == 1) digitalWrite(7,HIGH);
    if(bitRead(RXdata,2) == 0) digitalWrite(7,LOW);
    if(bitRead(RXdata,1) == 1) digitalWrite(9,HIGH);
    if(bitRead(RXdata,1) == 0) digitalWrite(9,LOW);
    
    camera_motor(RXdata2);
    
  }
}
//************************************************************
//------------------------------------------------------------
void camera_motor(byte RXdata)
{
  if(bitRead(RXdata,3) == 0){digitalWrite(5,LOW); digitalWrite(10,LOW);return;} 
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
//------------------------------------------------------------
// reads sensors' value.
//------------------------------------------------------------
void readSensors()
{
  //reading ir sensor values.
  irLeft = analogRead(1);
  irRight = analogRead(4);
  //reading ultrasonic sensor value , distance in [cm].
  int duration =0;
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  ultrasonic = (duration / 2) / 29.1;
  
}
//------------------------------------------------------------
// detects obstacles infront of the robot.
//------------------------------------------------------------
void detect_object()
{
  digitalWrite(7,HIGH);
  readSensors();
  int distanceAvg = (irLeft+irRight)/2;
  if(distanceAvg > minDistance) {digitalWrite(9,HIGH); moving_direction('S');}
  else digitalWrite(9,LOW);
}
//***********************END detect_objects****************************
//------------------------------------------------------------
// blink LED. INPUT = LED pin
//------------------------------------------------------------
void blinkWithoutDelay(int ledPin)
{
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;  

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
      
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

