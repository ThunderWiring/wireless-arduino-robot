#define SERVO_PIN  13 

int lenMicroSecondsOfPeriod = 20 * 1000; // 20 milliseconds (ms)
int lenMicroSecondsOfPulse = 0; // used in the 'for' loop below
int increment = 0.01 * 1000;
int currentServoPosition = 1.34 * 1000; // middle position, 90 degrees.
int ctrl  = 0; //recognizing first time entering the servo method.

void setup()
{
  pinMode(SERVO_PIN, OUTPUT);  
  servoWrite(90);
}
 
 
void loop()
{
  servoWrite(45);
  delay(500);
  servoWrite(135);
  delay(500);
}

void servoWrite(int degree)
{
  if(ctrl ==  0) {currentServoPosition = 0; ctrl++;}
  switch(degree)
  {
    case 0 : lenMicroSecondsOfPulse = 0.50 * 1000; break;
    case 45: lenMicroSecondsOfPulse = 0.85 * 1000; break;
    case 90: lenMicroSecondsOfPulse = 1.34 * 1000; break;
    case 135: lenMicroSecondsOfPulse = 1.85 * 1000; break;
    case 180: lenMicroSecondsOfPulse = 2.20 * 1000; break;
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
