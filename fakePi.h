//
//  fakePi.h
//  legor
//
//  Created by Ugo Cavalcanti on 15/04/17.
//  Copyright © 2017 Ugo Cavalcanti. All rights reserved.
//

#ifndef fakePi_h
#define fakePi_h

#include <stdio.h>


void digitalWrite(int pin, int HIGH);

void pwmWrite(int pin, int dPWM);

int wiringPiSetup();

#endif /* fakePi_h */
