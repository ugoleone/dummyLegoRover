//
//  fakePi.c
//  legor
//
//  Created by Ugo Cavalcanti on 15/04/17.
//  Copyright © 2017 Ugo Cavalcanti. All rights reserved.
//

#include "fakePi.h"


void digitalWrite(int pin, int HIGH){
    
    printf("PIN: %d\nDigitalOut: %d\n", pin, HIGH);
    
}

void pwmWrite(int pin, int dPWM){
    printf("PIN: %d\nPWMOut: %d\n", pin, dPWM);
}

int wiringPiSetup(){
    return 0;
}
