/************************************************************************/
/*				                        		*/
/*	  LCDPDemoProject 		                                */
/*						                  	*/
/*					                        	*/
/************************************************************************/
/*	Author: Cristian Fatu						*/
/*	Copyright 2011, Digilent Inc.					*/
/************************************************************************/
/*  File Description: 			             		        */
/*					                        	*/
/* This file implements a simple demo application that demonstrates     */
/* how to use the LCDP library.				                */
/*									*/
/*	Functionality:							*/
/*									*/
/* In the setup() function, the application instantiates and initializes*/
/* the LCDP library object.                                             */
/* In the loop() function, the application implements demo steps        */
/*					                        	*/
/*	Required Hardware:						*/
/*	  1. Cerebot 32MX4cK    					*/
  /*	  2. PmodCLP - plugged connected on JA, JB              	*/
/************************************************************************/
/*  Revision History:			        			*/
/*					                        	*/
/*	12/10/2011 (CristianF): created		       			*/
/*						                     	*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions  	*/
/* ------------------------------------------------------------ */
#include <LCDP.h>
#include <Bounce.h>
/* ------------------------------------------------------------ */
/*				Local Type Definitions		*/
/* ------------------------------------------------------------ */
#define btnPin1           42
#define btnPin2           43
/* ------------------------------------------------------------ */
/*				Global Variables		*/
/* ------------------------------------------------------------ */
LCDP         MyLCDP;
char         szInfo1[0x27]; 
char         szInfo2[0x27];
//custom characters definition
byte         defChar[] = {0, 0x4, 0x2, 0x1F, 0x02, 0x4, 0, 0};
byte         defChar1[] = {14, 31, 21, 31, 23, 16, 31, 14};
byte         defChar2[] = {0x00, 0x1F, 0x11, 0x00, 0x00, 0x11, 0x1F, 0x00};
byte         defChar3[] = {0x00, 0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00};
//bytes array representing the position of the user defined characters in the memory
byte         charsToDisp[] = {0, 1, 2, 3};
//definitions for display and cursor settings flags
boolean      fDisplay     = true;
boolean      fBackLight   = true;
boolean      fCursor      = false;
boolean      fBlink       = false;
//flags indicating whether a button has been pressed or not
boolean      fBtn1Process;
boolean      fBtn2Process;
//Bounce class objects instantiation
Bounce       debBtn2      = Bounce(btnPin2, 50);
Bounce       debBtn1      = Bounce(btnPin1, 50);
/* ------------------------------------------------------------ */
/*				Forward Declarations		*/
/* ------------------------------------------------------------ */
//function that detects if a button or both have been pressed
boolean WaitUntilBtnPressed(boolean *pfBtn1Process, boolean *pfBtn2Process);


void setup() {
  // initialize LCDP
    MyLCDP.begin(LCDP_32MX4CK_JA_JB_ARGS); // corresponds to Pmod connected on JA, JB
    MyLCDP.SetBacklight(true);
    //set the pin buttons as inputs
    pinMode(btnPin1, INPUT);
    pinMode(btnPin2, INPUT);
}

void loop() {

  // 1. Welcome message displayed
    MyLCDP.DisplayClear(); // clear display, return cursor home
    strcpy(szInfo1, "LCDP Arduino");
    strcpy(szInfo2, "Press any button");
    MyLCDP.WriteStringAtPos(szInfo1, 0, 0);
    MyLCDP.WriteStringAtPos(szInfo2, 1, 0);
    fBtn1Process = false;
    fBtn2Process = false;
    //wait until any button pressed
    while((fBtn1Process == false && fBtn2Process == false)) {
       WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
    }
    MyLCDP.DisplayClear(); // clear display, return cursor home
   
    // 2. Toggle Backlight 
    strcpy(szInfo1, "BTN2: Backlight");
    strcpy(szInfo2, "BTN1: Continue");
    MyLCDP.WriteStringAtPos(szInfo1, 0, 0);
    MyLCDP.WriteStringAtPos(szInfo2, 1, 0);
    fBtn1Process = false;
    fBtn2Process = false;
    //wait for button 2 pressed to continue
    while(fBtn1Process == false) {
        WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
	if(fBtn2Process == true) {
            // if only Btn2 is pressed
	    fBackLight =  1 - fBackLight; // toggle fBackLight
	    MyLCDP.SetBacklight(fBackLight);
	}
    }
        
    // 3. Display Scroll left/right
    // clear display, return cursor home
    MyLCDP.DisplayClear(); 
    strcpy(szInfo1, "Btns - L/R scroll long text");
    strcpy(szInfo2, "BTN1&BTN2: continue");
    MyLCDP.WriteStringAtPos(szInfo1, 0, 0);
    MyLCDP.WriteStringAtPos(szInfo2, 1, 0);
    while(fBtn1Process == false || fBtn2Process == false) {
	WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
	if(fBtn1Process != fBtn2Process) {
            // if only one button is pressed
            //scroll one position to left or right
	    MyLCDP.DisplayShift(fBtn1Process);
	 }
    }
    MyLCDP.DisplayClear(); // clear display, return cursor home

    // 4. Toggle cursor 
    strcpy(szInfo1, "BTN2: Cursor");
    strcpy(szInfo2, "BTN1: Continue");
    MyLCDP.WriteStringAtPos(szInfo1, 0, 0);
    MyLCDP.WriteStringAtPos(szInfo2, 1, 0);
    fBtn1Process = false;
    fBtn2Process = false;
    while(fBtn1Process == false) {
	WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
	if(fBtn2Process == true) {
	    // if only Btn2 is pressed
	    fCursor =  1 - fCursor; // toggle fCursor
	    MyLCDP.SetCursor(fCursor);
	}
    }
  
    // 5. Toggle blink
    strcpy(szInfo1, "BTN2: Blink ");
    strcpy(szInfo2, "BTN1: Continue");
    MyLCDP.WriteStringAtPos(szInfo1, 0, 0);
    MyLCDP.WriteStringAtPos(szInfo2, 1, 0);
    fBtn1Process = false;
    fBtn2Process = false;
    while(fBtn1Process == false){
	WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
	if(fBtn2Process == true){
	    // if only Btn2 is pressed
	    fBlink =  1 - fBlink; // toggle fBlink
            MyLCDP.SetBlink(fBlink);
        }
    }
    // restore cursor off
    MyLCDP.SetCursor(false);
    // restore blink off
    MyLCDP.SetBlink(false);
    
    
    // 6. User char.
    // define characters in RAM at positions 0, 1, 2, 3 
    MyLCDP.DefineUserChar(defChar, 0);
    MyLCDP.DefineUserChar(defChar1, 1);
    MyLCDP.DefineUserChar(defChar2, 2);
    MyLCDP.DefineUserChar(defChar3, 3);

    MyLCDP.DisplayClear(); // clear display, return cursor home
    strcpy(szInfo1, "User char:");
    MyLCDP.WriteStringAtPos(szInfo1, 0, 0);
    // write user defined character	
    MyLCDP.WriteUserCharsAtPos(charsToDisp, 4, 0, 10);
    strcpy(szInfo2, "Btn to continue");
    MyLCDP.WriteStringAtPos(szInfo2, 1, 0);
    fBtn1Process = false;
    fBtn2Process = false;
    //move forward if any button pressed
    while(fBtn1Process == false && fBtn2Process == false) {
        WaitUntilBtnPressed(&fBtn1Process, &fBtn2Process);
    }
} 
/* ------------------------------------------------------------------- */
/** void  WaitUntilBtnPressed(boolean *pfBtn1Process, boolean *pfBtn2Process)
**
**	Parameters:
**		pfBtn1Process - true for button1 pressed
**		pfBtn2Process - true for button2 pressed
**
**	Return Value:
**		boolean
**
**	Errors:
**		none
**
**	Description:
**		This function returns the state of the two buttons, if they are pressed or not
**
-----------------------------------------------------------------------*/
boolean WaitUntilBtnPressed(boolean *pfBtn1Process, boolean *pfBtn2Process)
{
    *pfBtn1Process = false;
    *pfBtn2Process = false;
    while ((*pfBtn1Process == false) && (*pfBtn2Process == false)) {
	//check the state of the buttons
        debBtn1.update();
        debBtn2.update();
        //if either one of them is pressed while the other is released, set the flags for buttons pressed
	if ((debBtn2.read() && debBtn1.risingEdge())||
           (debBtn2.risingEdge() && debBtn1.read())) 
	    {
	    *pfBtn1Process = true;
	    *pfBtn2Process = true;
  	}
	else {
            //if only button 1 is pressed
	    if (debBtn1.read()&&debBtn1.risingEdge()) {
		*pfBtn1Process = true;
	    }
	    else {
                //if only button 2 is pressed
		if (debBtn2.read()&&debBtn2.risingEdge()) {
		    *pfBtn2Process = true;
		}
	    }	
	}
    }
}



