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

#define HIGH 1
#define LOW 2
#define OUTPUT 3
#define PWM_OUTPUT 4


void digitalWrite(int pin, int value);

void pwmWrite(int pin, int PWM);

int wiringPiSetup();

void pinMode (int pin, int mode);

#endif /* fakePi_h */
