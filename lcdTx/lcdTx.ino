#include <SoftwareSerial.h>

SoftwareSerial mySerial(12,13); // RX, TX
int value = 0;
int keyStatus =0;
int forwardBack = 0; //holds analog value from right joystick which is responsible for Forward/Backward
int rightLeft=0;//holds analog value from right joystick which is responsible for Right/Left
byte TXdata = 00000000;
void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600); 
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600); 
  
  pinMode(0,INPUT); // vertical(horizental in practice)
  pinMode(7,INPUT); // left key
  pinMode(8,OUTPUT); // direction indicating LED
  pinMode(9,OUTPUT);// direction indicating LED
  pinMode(10,OUTPUT);// direction indicating LED
  pinMode(11,OUTPUT); // direction indicating LED
  pinMode(1,INPUT); //right joystick - vertical(riightLeft)
  pinMode(2,INPUT); //right joystick - horizental(forwardBack)
}

void loop() // run over and over
{
  int value = analogRead(0);
  int keyStatus = digitalRead(7);  
  forwardBack = analogRead(2);
  rightLeft = analogRead(1);
  delay(100);  
  TXdata = 0;
  transmittData(value, keyStatus); 
  receiveData();
  Serial.println(TXdata,BIN);
}

// reads data from joystick and builds up a string 
//describing that data which is being transmitted over mySerial to the GLCD board
void transmittData(int value, int keyStatus)
{   
  if(value<30)
  {
    bitSet(TXdata,0);
    bitSet(TXdata,1);
    mySerial.write('r'); //joystick moved to the right
    
  }
  if(value >900) 
  {
    bitSet(TXdata,0);
    bitSet(TXdata,2);
    mySerial.write('l'); //joystick moved to the left    
  }   
  if(value>450&&value<900)
  {
    TXdata = TXdata&248;//111111000
    if(keyStatus == HIGH )
    { 
      mySerial.write('f');  //key released   
    }
    if(keyStatus==LOW)
    {
      mySerial.write('t'); //key pressed
      bitSet(TXdata,3); 
    } 
  }  
  delay(1);
  if(forwardBack>900)
  {
    mySerial.write('B');
    bitSet(TXdata,4);
  }
  else if(forwardBack<10)
  {
    mySerial.write('F');
    bitSet(TXdata,5);
  }
  else if(rightLeft<10)
  {
    mySerial.write('L');
    bitSet(TXdata,6);
  }
  else if(rightLeft>900) 
  {
    mySerial.write('R');
    bitSet(TXdata,7); 
  } 
  //mySerial.write(TXdata); 
  delay(100);   
}

void receiveData()
{
  char rx_data;
  if (mySerial.available()>0)
  {
    rx_data = mySerial.read();
    if(rx_data == '1')
    {
      if(forwardBack>900)//bk
      {
        digitalWrite(9,HIGH);        
        digitalWrite(8,LOW);
        digitalWrite(10,LOW);
        digitalWrite(11,LOW);
      }
      else if(forwardBack<10)//fd
      {
        digitalWrite(8,LOW);        
        digitalWrite(10,HIGH);
        digitalWrite(9,LOW);
        digitalWrite(11,LOW);
      }
      else if(rightLeft<10)//l
      {
        digitalWrite(10,LOW);        
        digitalWrite(9,LOW);
        digitalWrite(8,HIGH);
        digitalWrite(11,LOW);               
      }
      else if(rightLeft>900) //rt
      {
        digitalWrite(8,LOW);        
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        digitalWrite(11,HIGH);
      }
    }
    else if(rx_data == '0')
    {
      digitalWrite(8,LOW);        
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
    }
  }
}
