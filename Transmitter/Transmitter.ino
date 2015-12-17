int verticalR=0, horizontalR =0, verticalL=0,horizontalL=0;
void setup()
{
  Serial.begin(9600);
  //pinMode(trigPin,OUTPUT);
  //pinMode(echoPin,INPUT);
  
  pinMode(0,INPUT); // vertical right
  pinMode(1,INPUT); // horizontal right
  pinMode(2,INPUT); //vertical left
  pinMode(3,INPUT); // horizontal left.
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
}
//************************************************************
//------------------------------------------------------------
void transmitt()
{
  if(verticalR<30) Serial.write('L');
  else if(verticalR>900)Serial.write('R');
  else if(horizontalR<30) Serial.write('F');
  else if(horizontalR>900) Serial.write('B');
  else Serial.write('S');
  //if(verticalR<300&&verticalR<600 && horizontalR<300&&horizontalR<600) 
}
//************************************************************
//------------------------------------------------------------
  
