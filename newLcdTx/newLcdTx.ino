#include <SoftwareSerial.h>
SoftwareSerial mySerial(12,13); // RX, TX

#define vertical_left      0
#define horizontal_left    1
#define key_left           7
#define vertical_right     2
#define horizontal_right   3
#define key_right          6

int verticalLeft=0,horizontalLeft=0,verticalRight=0,horizontalRight=0;
boolean keyLeft=false,keyRight=false;
byte TXdata = 00000000; //containing the content of the booleans bellow:
boolean slideUp =false, slideDown = false, slideLeft=false, slideRight=false;
boolean forward =false, backward = false, right=false, left=false;
//--------------------------------------------------------------
void setup()
{
  Serial.begin(9600); 
  mySerial.begin(9600);
  
  pinMode(0,INPUT); // vertical left(horizental in practice)
  pinMode(1,INPUT); // horizontal left  
  pinMode(2,INPUT); //right vertical 
  pinMode(3,INPUT); //right horizental(forwardBack)
  pinMode(7,INPUT); // left key
  pinMode(6,INPUT); // right key 
}
void loop()
{
  read_joystick();
  delay(100);
  TX();
  mySerial.write(TXdata);
  delay(100);
  //setFlags(TXdata);
  /*Serial.print(slideRight);
  Serial.print(slideLeft);
  Serial.print(slideUp);
  Serial.print(slideDown); 
  Serial.print(" ");
  Serial.print(forward);
  Serial.print(backward);
  Serial.print(left);
  Serial.println(right);*/
}
//**************************loop****************************
//----------------------------------------------------------
// reads the analog values from the joysticks(left+right)
//----------------------------------------------------------
void read_joystick()
{
  verticalLeft = analogRead(vertical_left);    
  horizontalLeft = analogRead(horizontal_left);  
  verticalRight = analogRead(vertical_right);
  horizontalRight = analogRead(horizontal_right);
  keyLeft = digitalRead(key_left);
  keyRight = digitalRead(key_right); 
}
//***********************read_joystick***********************
//----------------------------------------------------------
// buildingand transmitting Data byte
//----------------------------------------------------------
void TX()
{
  if(verticalLeft < 30)
  {
    slideRight = true; slideLeft=false; slideDown = false; slideUp = false; 
    bitSet(TXdata,0); 
  }
  if(verticalLeft > 900) 
  {
    slideLeft = true; slideRight = false; slideDown = false; slideUp = false; 
    bitSet(TXdata,1);
  }   
  if(verticalLeft>450&&verticalLeft<900)
  {
    slideLeft = false; slideRight = false; //slideDown = false; slideUp = false; 
    TXdata = TXdata&252; //11111100
  }  
  
  if(horizontalLeft < 30)
  {
    slideDown = true; slideUp = false; slideRight = false; slideLeft=false;                            
    bitSet(TXdata,2);    
  }
  if(horizontalLeft > 900) 
  {
    slideUp = true; slideDown = false; slideRight = false; slideLeft=false; 
    bitSet(TXdata,3);
  }   
  if(horizontalLeft>450&&horizontalLeft<900)
  {
    slideDown = false; slideUp = false; 
    TXdata = TXdata & 243;//11110011   
  }  
  
  if(verticalRight < 30)
  {
    left = true; right = false; forward = false; backward = false;
    bitSet(TXdata,4);
  }
  if(verticalRight > 900) 
  {
    right = true; left = false; forward = false; backward = false;
    bitSet(TXdata,5);
  }   
  if(verticalRight>450&&verticalRight<900)
  {
    left = false; right = false;
    TXdata = TXdata & 207; //11001111
  }  
  
  if(horizontalRight < 30)
  {
    forward = true; backward = false; right = false; left = false;
    bitSet(TXdata,6);
  }
  if(horizontalRight > 900) 
  {
    backward = true; forward = false; right = false; left = false;
    bitSet(TXdata,7);
  }   
  if(horizontalRight>450&&horizontalRight<900)
  {
    forward = false; backward = false;
    TXdata = TXdata & 63; //00111111
  }
  Serial.println(TXdata,BIN);  
   
}
//***********************dataByte***********************
