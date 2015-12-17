// include the library header
#include <glcd.h>
// include the Fonts
#include <fonts/allFonts.h>
#include "fonts/allFonts.h"         // system and arial14 fonts are used
#include <SoftwareSerial.h>
//--------------------------------------------------------------
//variables:
int keyDownClicksCounts = 0; // the total number of clicks on the push-button
int menueStringQuantity =4; //how many items ther is in the menue
int keySelectCount=0; // the status of the push-button
int horizentalSlide=0;
String s2 ="normal",s1="manual",s3="access", s4="read";
char RXdata; // data that is being received over mySerial
boolean right = false; //indicating right sliding
boolean left = false;//indicating left sliding
boolean key = false; // left joystick key status.
boolean Menu=false; // if true then show the menu
boolean sdAccess=false;
boolean configAccess=false;
boolean forward = false, backward=false, rightward=false, leftward=false;
gText textArea;
SoftwareSerial mySerial(12,13); // RX, TX
boolean emptyScreen = true; //if screen is empty=>flag=true=>no need for GLCD.ClearScreen
boolean alreadyCleared = true;
boolean toggle; //reading toggle status
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600); 
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600); 
  pinMode(13,OUTPUT); // tx
  pinMode(12,INPUT); // rx
  // Initialize the GLCD 
  GLCD.Init();
 // Select the font for the default text area
  GLCD.SelectFont(SystemFont5x7);    
}

void loop()
{     
  if(digitalRead(3)==HIGH)
  {
    GLCD.ClearScreen();  
    emptyScreen =true;
    alreadyCleared = true;
  }
  if(digitalRead(2)==HIGH)//toggle on
  {
    drive();
    if(digitalRead(2)==LOW) GLCD.ClearScreen();
  } 
  if(digitalRead(2)==LOW)//toggle off
  {
    if(emptyScreen == false) alreadyCleared=false;
    Menu=true; 
    mySerial.write('0');/*toggle off*/     
  } 
  if(Menu) 
  {     
    showMenu();
    if(digitalRead(2)==HIGH)
    {
      textArea.SetFontColor(BLACK);
      GLCD.ClearScreen(); 
      emptyScreen = true;
      Menu = false;
    } 
  }
  if(sdAccess) 
  {   
    sdRead();    
  }
}


void drive()
{
  mySerial.write('1');//toggle on
  receiveData();           
  if(emptyScreen) 
  {  
    GLCD.println("drive test");
    emptyScreen =false;
  } 
  if(forward)       {GLCD.println("Forward"); forward = false; emptyScreen =false;}
  else if(backward) {GLCD.println("Backward");backward = false; emptyScreen =false;}
  else if(leftward) {GLCD.println("Left");leftward = false; emptyScreen =false;}
  else if(rightward){GLCD.println("Right");rightward = false; emptyScreen =false;}
}
//showing the main menu, when the menu push button is pressed.
void showMenu()
{
  if(sdAccess) return;
  /*GLCD.DrawHLine(1,7,128); // top
  GLCD.DrawVLine(0,7,56); //at center*/
  showArea(textAreaRIGHT, "Main Menu");
  receiveData();
  if(right)
  {
    horizentalSlide++;
    if(horizentalSlide>=3) horizentalSlide=0;
  }
  else if(left)
  {
    horizentalSlide --;
    if(horizentalSlide<0) horizentalSlide=2;
  } 
  int joystickValue = analogRead(5);
  delay(150);
  scroll(joystickValue) ;
  menuRight(textAreaLEFT,keyDownClicksCounts,keySelectCount,horizentalSlide,key);  
}
//scrolling text up or down according to joystick value
void scroll(int joystickValue)
{
  if(joystickValue<400) //left joystick down
  {     
    keyDownClicksCounts= keyDownClicksCounts +1;
    textArea.DefineArea(textAreaLEFT);
    textArea.CursorToXY(2,8); 
    textArea.EraseTextLine();
    textArea.SetFontColor(BLACK);    
    textArea.DrawString(s1, 2,8);    

    textArea.CursorToXY(2,24); 
    textArea.EraseTextLine();
    textArea.SetFontColor(BLACK); 
    textArea.DrawString(s2, 2,8); 
    
    if(keyDownClicksCounts > menueStringQuantity) keyDownClicksCounts=1;       
  } 
  else if(joystickValue>900) //left joystick up
  {
    keyDownClicksCounts= keyDownClicksCounts -1;
    textArea.DefineArea(textAreaLEFT);
    textArea.CursorToXY(2,8); 
    textArea.EraseTextLine();
    textArea.SetFontColor(BLACK); 
    textArea.DrawString(s1, 2,8);    
    if(keyDownClicksCounts <=0) keyDownClicksCounts=menueStringQuantity;
  }
  emptyScreen =false;
}
void showArea(predefinedArea area, char *description)
{
  textArea.DefineArea(area);
  textArea.SelectFont(SystemFont5x7);
  textArea.SetFontColor(WHITE); 
  textArea.DrawString(description, 0,0);
  textArea.SetFontColor(BLACK);   
  menuLeft("-control","-speed adj","-config", "-map");  
}

void menuLeft(String s1, String s2, String s3, String s4)
{ 
  textArea.SetFontColor(BLACK);      
  textArea.DrawString(s1, 2,8);
  textArea.DrawString(s2, 2,24);     
  textArea.DrawString(s3, 2,40);
  textArea.DrawString(s4, 2,56);
  emptyScreen =false;
}

void menuRight(predefinedArea area,int keyDownClicksCounts,int keySelectCount, int horizentalSlide,boolean key)
{
   
  textArea.DefineArea(area);  
  switch(keyDownClicksCounts)
  {
    case 0: 
             textArea.SetFontColor(BLACK); 
             textArea.DrawString(s1, 2,8);
             textArea.DrawString(s2, 2,24);     
             textArea.DrawString(s3, 2,40);
             textArea.DrawString(s4, 2,56);
             break;
    case 1:                                        
             textArea.CursorToXY(2,8);              
             textArea.EraseTextLine();
             textArea.SetFontColor(WHITE);
             switch(horizentalSlide)
             {
               case 0:                       
                      if(key) {s1="manual"; horizentalSlide=0;}
                      else textArea.DrawString("manual", 2,8);                                                                  
                      break;
               case 1:
                      if(key) {s1="autonomus"; horizentalSlide=1;}
                      else {textArea.DrawString("autonomus", 2,8);}
                      break;  
               case 2:
                      if(key) {s1="manual"; horizentalSlide=0;}
                      else textArea.DrawString("manual", 2,8);                                                                  
                      break;               
             } 
                                                      
             textArea.SetFontColor(BLACK);  textArea.DrawString(s2, 2,24);              
             textArea.SetFontColor(BLACK);  textArea.DrawString(s3, 2,40);    
             textArea.SetFontColor(BLACK);  textArea.DrawString(s4, 2,56);    
             
             break;                          
    case 2: 
             
             textArea.SetFontColor(BLACK);   
             textArea.DrawString(s1, 2,8);                          
             textArea.CursorToXY(2,24);              
             textArea.EraseTextLine();
             textArea.SetFontColor(WHITE);
             switch(horizentalSlide)
             {
               case 0:                       
                      if(key) {s2="normal"; horizentalSlide=0;}
                      else textArea.DrawString("normal", 2,24);                                                                  
                      break;
               case 1:
                      if(key) {s2="low"; horizentalSlide=1;}
                      else textArea.DrawString("low", 2,24);                      
                      break;
               case 2:
                      if(key) {s2="high"; horizentalSlide=2;}
                      else textArea.DrawString("high", 2,24);                       
                      break;                       
             }                 
             textArea.SetFontColor(BLACK);  textArea.DrawString(s3, 2,40);                       
             textArea.SetFontColor(BLACK);  textArea.DrawString(s4, 2,56);                        
             break;
    case 3:             
             textArea.SetFontColor(BLACK);   textArea.DrawString(s1, 2,8);            
             textArea.SetFontColor(BLACK);   textArea.DrawString(s2, 2,24);             
             textArea.SetFontColor(WHITE);   textArea.DrawString(s3, 2,40);                        
             textArea.SetFontColor(BLACK);   textArea.DrawString(s4, 2,56);             
             break;   
    case 4:             
             textArea.SetFontColor(BLACK);   textArea.DrawString(s1, 2,8);             
             textArea.SetFontColor(BLACK);   textArea.DrawString(s2, 2,24);             
             textArea.SetFontColor(BLACK);   textArea.DrawString(s3, 2,40);             
             textArea.CursorToXY(2,56);
             textArea.EraseTextLine();
             textArea.SetFontColor(WHITE);              
             switch(horizentalSlide)
             {
               case 0:                       
                      if(key) 
                      {
                        s4="read"; 
                        horizentalSlide=0; 
                        sdAccess = true; 
                        textArea.ClearArea(); 
                        emptyScreen = true;
                        GLCD.ClearScreen();
                        Menu = false;
                        return;
                      }
                      else textArea.DrawString("read", 2,56);                                                                  
                      break;
               case 1:
                      if(key) {s4="record"; horizentalSlide=1;}
                      else textArea.DrawString("record", 2,56);                      
                      break;  
               case 2:
                      if(key) 
                      {
                        s4="read"; 
                        horizentalSlide=0; 
                        sdAccess = true; 
                        textArea.ClearArea(); 
                        emptyScreen = true;
                        GLCD.ClearScreen();
                        Menu = false;
                        return;
                      }
                      else textArea.DrawString("read", 2,56);                                                                  
                      break;               
             }         
             break;              
  }  
}
void sdRead()
{
  if(digitalRead(3)==HIGH) 
  {
    GLCD.ClearScreen();
    sdAccess = false;
    Menu = true;
    return;
  }   
  textArea.DefineArea(textAreaFULL);
  textArea.SelectFont(SystemFont5x7);
  textArea.SetFontColor(WHITE); 
  textArea.DrawString("Reading Map Data",64,0);
  delay(200);
  textArea.DrawString("Access SD", 64,15);
  if(digitalRead(2)==HIGH)
  {
    textArea.SetFontColor(BLACK);
    GLCD.ClearScreen(); 
    emptyScreen = true;
    Menu = false;
    sdAccess = false;
  }
}
//receiving data via software serial from the second arduino board.
void receiveData()
{  
  mySerial.listen();
  if (mySerial.available()>0)
  {
    RXdata = mySerial.read();   
    switch(RXdata)
    {
      case 'r':
                right = true;
                left = false;
                break;
      case 'l':
                right = false;
                left = true;
                break;
      case 't':
                key = true;
                right = false;
                left = false;
                break;
      case 'f':
                key = false;
                right = false;
                left = false;
                break;
      case 'F':                
                forward = true;
                backward = false;
                leftward = false;
                rightward = false;
                break;
      case 'B':               
                forward = false;
                backward = true;
                leftward = false;
                rightward = false;
                break;
      case 'L':                
                forward = false;
                backward = false;
                leftward = true;
                rightward = false;
                break;
      case 'R':                
                forward = false;
                backward = false;
                leftward = false;
                rightward = true;
                break;           
      default:
                right = false;
                left = false;
                key = false;
                forward = false;
                backward = false;
                leftward = false;
                rightward = false;
                break;
    }    
  }
}
