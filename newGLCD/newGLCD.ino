#include <glcd.h>
// include the Fonts
#include <fonts/allFonts.h>
#include "fonts/allFonts.h"  // system and arial14 fonts are used
#include <SoftwareSerial.h>
//--------------------------------------------------------------
//variables:
gText textArea;
SoftwareSerial mySerial(12,13); // RX, TX
int slide_left =0, slide_right = 0; // counters
byte data = 00000000; /*recieved data via SoftwareSerial*/
boolean slideUp =false, slideDown = false, slideLeft=false, slideRight=false;
boolean forward =false, backward = false, right=false, left=false;
boolean BmanualDrive = false, Bmenu=true;
boolean clearScreen = true;
//--------------------------------------------------------------
void setup()
{
  Serial.begin(9600); 
  mySerial.begin(9600); 
  pinMode(13,OUTPUT); // tx
  pinMode(12,INPUT); // rx
  pinMode(2,INPUT); // left joystick key
  GLCD.Init();
  GLCD.SelectFont(SystemFont5x7);   
}
//................................................
void loop()
{
  GLCD.DrawString("fghfhfh",0,10);
  RX(); 
  delay(50);
  /*Serial.print(slideRight);
  Serial.print(slideLeft);
  Serial.print(slideUp);
  Serial.print(slideDown); 
  Serial.print(" ");
  Serial.print(forward);
  Serial.print(backward);
  Serial.print(left);
  Serial.println(right); */
  if(Bmenu)mainMenu();
  if(BmanualDrive) manual_drive();
}
//******************loop()************************
//------------------------------------------------
// initial windo to appear when turning the screen on
//--------------------------------------------------
void mainMenu()
{
  scroll_horizontal(2);
  
  GLCD.CursorToXY(64, 1);
  GLCD.SetFontColor(BLACK);
  GLCD.println("Main Menu");
  GLCD.DrawHLine(0,9,128); 
  GLCD.DrawRect(0,25,GLCD.StringWidth("Autonomus")+2,16);
  GLCD.DrawRect(70,25,GLCD.StringWidth("Manual")+6,16);
  GLCD.DrawVLine(2+GLCD.StringWidth("Autonomus"),25,16);
  GLCD.DrawVLine(76+GLCD.StringWidth("Manual"),25,16);
  if(slide_right%2 == 1)
  {
    GLCD.SetFontColor(BLACK);  
    GLCD.DrawString("Autonomus",2,30);  
    GLCD.SetFontColor(WHITE);  
    GLCD.DrawString("Manual",73,30);
    if(digitalRead(2) == 0)//left joystick key pressed
    {
      BmanualDrive = true;
      Bmenu = false;
      clearScreen = true;
    }
  }
  else if(slide_right%2 == 0)
  {
    GLCD.SetFontColor(WHITE);  
    GLCD.DrawString("Autonomus",2,30);  
    GLCD.SetFontColor(BLACK);  
    GLCD.DrawString("Manual",73,30);
  }  
}
//*********************introScreen**********************
//------------------------------------------------------
// input: the number of strings per menu.
//------------------------------------------------------
void scroll_horizontal(int numberOfStrings)
{ 
  if(slideRight) slide_right++;
  if(slideLeft)  slide_left ++;
  if(slide_right > numberOfStrings) slide_right=1;         
  else if(slide_left > numberOfStrings) slide_left=1; 
  Serial.println(slide_right);
  delay(130);
}
//*****************scroll_horizontal********************
//------------------------------------------------------
// screen window showing manual direction driving
//------------------------------------------------------
void manual_drive()
{
  if(clearScreen)
  {
    GLCD.ClearScreen();
    clearScreen = false;
  }
  GLCD.CursorToXY(64, 1);
  GLCD.SetFontColor(BLACK);
  GLCD.println("ManualCtrl");
  GLCD.DrawHLine(0,9,128);
  
  GLCD.DrawRect(85,18,14,10); //forward
  GLCD.DrawRect(85,38,14,10); //backward
  GLCD.DrawRect(68,28,14,10); //left
  GLCD.DrawRect(102,28,14,10);//right  
  
  if(forward)   fillRec('f');
  if(backward)  fillRec('b');
  if(left)      fillRec('l');
  if(right)     fillRec('r');
}
//*****************manual_drive*************************
//------------------------------------------------------
// filling the rectangulars from manual_drive window screen
// input: char representing the rect. position to be filled
// i.e: f=> forward, l=>left..
//------------------------------------------------------
void fillRec(char pos)
{
  switch(pos)
  {
    case 'f':
              GLCD.FillRect(85,18,14,10);
              GLCD.FillRect(86,39,12,8,WHITE);
              GLCD.FillRect(69,29,12,8,WHITE);
              GLCD.FillRect(103,29,12,8,WHITE);
              break;
    case 'b':
              GLCD.FillRect(86,19,12,8,WHITE);
              GLCD.FillRect(85,38,14,10);
              GLCD.FillRect(69,29,12,8,WHITE);
              GLCD.FillRect(103,29,12,8,WHITE);
              break;
    case 'l':
              GLCD.FillRect(86,19,12,8,WHITE);
              GLCD.FillRect(86,39,12,8,WHITE);
              GLCD.FillRect(68,28,14,10);
              GLCD.FillRect(103,29,12,8,WHITE);
              break;
    case 'r':
              GLCD.FillRect(86,19,12,8,WHITE);
              GLCD.FillRect(86,39,12,8,WHITE);
              GLCD.FillRect(69,29,12,8,WHITE);
              GLCD.FillRect(102,28,14,10);
              break;
  }
}
//*****************fillRec******************************
//------------------------------------------------------
// data recieved by SoftwareSerial.
//------------------------------------------------------
void RX()
{  
  byte mask = 0;
  int i = 0;
  if (mySerial.available()>0)
  {    
    data = mySerial.read();
    //Serial.println(data,BIN);
    for (mask = 00000001; mask>0; mask <<= 1,i++)
    {      
      switch(i)
      {
        case 0: if(bitRead(data,0)==1)  {slideRight = true; slideLeft = false;break;} else{slideRight = false; break;}
        case 1: if(bitRead(data,1)==1)  {slideLeft = true;slideRight=false;break;} else{slideLeft = false; break;}
        case 2: if(bitRead(data,2)==1)  {slideDown = true;break;} else{slideDown = false; break;}
        case 3: if(bitRead(data,3)==1)  {slideUp = true;break;} else{slideUp = false; break;}
        case 4: if(bitRead(data,4)==1)  {left = true;break;} else{left = false; break;}
        case 5: if(bitRead(data,5)==1)  {right = true;break;} else{right = false; break;}
        case 6: if(bitRead(data,6)==1)  {forward = true;break;} else{forward = false; break;}
        case 7: if(bitRead(data,7)==1)  {backward = true;break;} else{backward= false; break;}
        default : slideLeft=false;slideDown=false;slideUp=false;left=false;right=false;forward=false;backward=false;break;
      }  
    }
  }  
}

//****************************RX()*******************************
