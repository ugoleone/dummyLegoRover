//
//  fakePi.c
//  legor
//
//  Created by Ugo Cavalcanti on 15/04/17.
//  Copyright © 2017 Ugo Cavalcanti. All rights reserved.
//

#include "fakePi.h"


void digitalWrite(int pin, int value){
    
    printf("PIN: %d\nDigitalOut: %d\n", pin, value);
    
}

void pwmWrite(int pin, int PWM){
    printf("PIN: %d\nPWMOut: %d\n", pin, PWM);
}

int wiringPiSetup(){
    return 0;
}

void pinMode (int pin, int mode){
    
}
