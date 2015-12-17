// TX - transmitter

struct dataTX
{
  float vertical1;   //joystick1
  float horizontal1;
  float vertical2;   // joystick2
  float horizontal2;
  
  float potKnob; //potentometer to control speed.
  int toggleManual; //toggle switch for selecting Manual/Auto control.
  
};

dataTX Transmitt;
void setup()
{   
  Serial.begin(9600);
}

void loop() 
{  
  readData();
  transmittData();
}

// reading data from all inputs.
void readData()
{
  Transmitt.vertical1=analogRead(0);
  Transmitt.horizontal1=analogRead(1);
  Transmitt.vertical2=analogRead(2);
  Transmitt.horizontal2=analogRead(3);
  Transmitt.potKnob=analogRead(4);
  Transmitt.toggleManual=digitalRead(2);
}

// transmitt the received data:
void transmittData()
{
  Serial.println(Transmitt.vertical1);
  delay(100);
  Serial.println(Transmitt.horizontal1);
  delay(100);
  Serial.println(Transmitt.vertical2);
  delay(100);
  Serial.println(Transmitt.horizontal2);
  delay(100);
  Serial.println(Transmitt.potKnob);
  delay(100);
  Serial.println(Transmitt.toggleManual);
  delay(100);
}
