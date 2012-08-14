//
//  LedDisplay.h
//  
//
//  Created by Jay Brockman on 8/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef LedDisplay_h
#define LedDisplay_h

class LedDisplay
{
public:
    LedDisplay(int p3, int p2, int p1, int p0);
    void cycle();
protected:
    int d3, d2, d1, d0;
};

#endif
