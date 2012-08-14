//
//  LedDisplay.cpp
//  
//
//  Created by Jay Brockman on 8/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "LedDisplay.h"

LedDisplay::LedDisplay(int p3, int p2, int p1, int p0)
{
    d3 = p3;
    d2 = p2;
    d1 = p1;
    d0 = p0;
    
    digitalWrite(d3, LOW); 
    digitalWrite(d2, LOW); 
    digitalWrite(d1, LOW); 
    digitalWrite(d0, LOW); 
}

void LedDisplay::cycle()
{
    digitalWrite(d3, LOW); 
    digitalWrite(d2, LOW); 
    digitalWrite(d1, LOW); 
    digitalWrite(d0, LOW); 
    digitalWrite(d3, HIGH);
    delay(100);
    digitalWrite(d3, LOW);
    digitalWrite(d2, HIGH);
    delay(100);
    digitalWrite(d2, LOW);
    digitalWrite(d1, HIGH);
    delay(100);
    digitalWrite(d1, LOW);
    digitalWrite(d0, HIGH);
    delay(100);
}
