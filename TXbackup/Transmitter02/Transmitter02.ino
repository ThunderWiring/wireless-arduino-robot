int verticalR=0, horizontalR =0, verticalL=0,horizontalL=0;
byte data = 00000000;
int potValue=0, previousPotValue=0, potValueDifference=0;
boolean clockwise, anticlock, enable = true;
void setup()
{
  Serial.begin(9600);
  //pinMode(trigPin,OUTPUT);
  //pinMode(echoPin,INPUT);
  Serial.write(0);
  pinMode(0,INPUT); // vertical right
  pinMode(1,INPUT); // horizontal right
  pinMode(2,INPUT); //vertical left
  pinMode(3,INPUT); // horizontal left.
  pinMode(4,INPUT); // potentometer
  previousPotValue = analogRead(4);
}
void loop()
{
  data = data&0;
  read_data();
  transmitt(); 
  delay(100);
  /*int duration =0 , distance = 0;
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration / 2) / 29.1;*/
  //Serial.print("previousPotValue: "); Serial.print(previousPotValue);
  //Serial.print("potValue: "); Serial.print(potValue);
  //Serial.println(data,BIN);
}
//************************************************************
//------------------------------------------------------------
void read_data()
{
  verticalR  =analogRead(0);
  horizontalR = analogRead(1);
  verticalL = analogRead(2);
  horizontalL = analogRead(3);
  
  previousPotValue = potValue;
  potValue = analogRead(4);
  potValueDifference = potValue - previousPotValue; 
  compare();
}
//************************************************************
//------------------------------------------------------------
void transmitt()
{
  if(verticalR<30)        {bitSet(data,3); data = data&15;} //left
  else if(verticalR>900)  {bitSet(data,4); data = data&23;} //right
  else if(horizontalR<30) {bitSet(data,6); data = data&71;} //forward
  else if(horizontalR>900){bitSet(data,5); data = data&39;} //backward
  else {bitSet(data,7); data = data&135;}
  
  if(enable)
  {
    bitSet(data,2);
    if(clockwise) bitSet(data,1);
    if(anticlock) bitSet(data,0);
  }
  Serial.write(data);  
}
//************************************************************
//------------------------------------------------------------
void compare()
{
  if(potValueDifference<=3&&potValueDifference>=-3) 
  {
    enable = false;
    clockwise = false; anticlock = false;
  }
  else
  {
    enable = true;
   if(potValueDifference>=4) {clockwise = true; anticlock = false;}
   if(potValueDifference<=-4){clockwise = false; anticlock = true;}
  }  
}
