//read RX

int state = -1;
void setup()
{
  Serial.begin(9600);
  pinMode(7,OUTPUT);
}
void loop()
{
  state = Serial.read();
  if(state == 1) digitalWrite(7,HIGH);
  else digitalWrite(7,LOW);
}
