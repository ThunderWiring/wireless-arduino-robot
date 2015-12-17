int verticalR=0, horizontalR =0, verticalL=0,horizontalL=0;
byte data = 00000000;
int potValue=0, previousPotValue=0, potValueDifference=0;;
void setup()
{
  Serial.begin(9600);
  //pinMode(trigPin,OUTPUT);
  //pinMode(echoPin,INPUT);
  
  pinMode(0,INPUT); // vertical right
  pinMode(1,INPUT); // horizontal right
  pinMode(2,INPUT); //vertical left
  pinMode(3,INPUT); // horizontal left.
  pinMode(4,INPUT); // potentometer
  previousPotValue = analogRead(4);
}
void loop()
{
  read_data();
  transmitt();  
  delay(100);
  /*int duration =0 , distance = 0;
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration / 2) / 29.1;*/
  /*Serial.print(" A0= "); Serial.print(analogRead(0));
  Serial.print(" A1= "); Serial.print(analogRead(1));
  Serial.print(" A2= "); Serial.print(analogRead(2));
  Serial.print(" A3= "); Serial.println(analogRead(3));
  
  //delay(500);*/
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
  Serial.write(data);
  //if(verticalR<300&&verticalR<600 && horizontalR<300&&horizontalR<600) 
}
//************************************************************
//------------------------------------------------------------
  
