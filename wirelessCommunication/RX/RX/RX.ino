// RX - robot

struct dataRX
{
  float vertical1;   //joystick1
  float horizontal1;
  float vertical2;   // joystick2
  float horizontal2;
  
  float potKnob; //potentometer to control speed.
  int toggleManual; //toggle switch for selecting Manual/Auto control.
  
};
int led = 2;
dataRX Receive;
void setup() 
{                
    Serial.begin(9600);
    pinMode(led,OUTPUT);
}

void loop()
{

  if (Serial.available()>0)
  {
    receiveData();
  }  
  else // blink LED if there is no serial connection available 
  {
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
  }
}

//receiving the transmitted data :
void receiveData()
{
  Receive.vertical1=Serial.read();
  delay(100);
  Receive.horizontal1=Serial.read();
  delay(100);
  Receive.vertical2=Serial.read();
  delay(100);
  Receive.horizontal2=Serial.read();
  delay(100);
  Receive.potKnob=Serial.read();
  delay(100);
  Receive.toggleManual=Serial.read();
  delay(100);
}

