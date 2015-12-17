int verticalR=0, horizontalR =0, verticalL=0,horizontalL=0;
byte data = 0, data2=1;
int potValue=0, previousPotValue=0, potValueDifference=0;
boolean clockwise, anticlock, enable = true;
int upperToggle, sideToggle;
//--------blinking without delay--------
#define led_tx 7
int ledState = LOW; 
unsigned long currentMillis;
unsigned long currentMillis_led;
long previousMillis_led = 0;   
long interval_led = 200; 
//----------------//------//-------//-----
void setup()
{
  
  Serial.begin(9600);  
  Serial.write(0);
  pinMode(0,INPUT); // vertical right
  pinMode(1,INPUT); // horizontal right
  pinMode(2,INPUT); //vertical left
  pinMode(3,INPUT); // horizontal left.
  pinMode(4,INPUT); // potentometer
  pinMode(2,INPUT); // upper toggle
  pinMode(3,INPUT); // side toggle
  previousPotValue = analogRead(4);
  bitSet(data2,0);
}
void loop()
{
  currentMillis = millis();
  data = 0;
  data2=0;
  read_data();
  //if(currentMillis - previousMillis > interval) 
  transmitt();   
  delay(100);
  Serial.flush();
  /**/
  //Serial.print("previousPotValue: "); Serial.print(previousPotValue);
  //Serial.print("d7 = "); Serial.println(digitalRead(7));
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
  upperToggle = digitalRead(2);
  sideToggle  = digitalRead(3);
  
  previousPotValue = potValue;
  potValue = analogRead(4);
  potValueDifference = potValue - previousPotValue; 
  compare();
}
//************************************************************
//------------------------------------------------------------
void transmitt()
{
  if(verticalR<30)        {bitSet(data,3); data = data&65295;} //left
  else if(verticalR>900)  {bitSet(data,4); data = data&8183;} //right
  else if(horizontalR<30) {bitSet(data,6); data = data&32711;} //forward
  else if(horizontalR>900){bitSet(data,5); data = data&16359;} //backward
  else {bitSet(data,7); data = data&32647;}
  
  if(upperToggle == 1)bitSet(data,2);
  if(sideToggle == 1) bitSet(data,1);
  
  
  if(enable)
  {
    bitSet(data2,3);
    if(clockwise) bitSet(data2,2);
    if(anticlock) bitSet(data2,1);
  }
  else data2 = data2&249;
  
  Serial.write(data);
  Serial.flush();
  Serial.write(data2);
  Serial.flush(); 
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
//************************************************************
//------------------------------------------------------------
/*void blinkWithoutDelay_tx()
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
}*/
//************************************************************
//------------------------------------------------------------

