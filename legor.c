#include <stdio.h> 
#include <stdlib.h>
//#include <wiringPi.h>
#include "fakePi.h"


const int DE = 1;
const int DA = 2;
const int DB = 3;

const int SE = 4;
const int SA = 5;
const int SB = 6;

int speed;  //valori ammessi: -2, -1, 0, 1, 2
int turn;   //valori ammessi: -2, -1, 0, 1, 2


void accelera(){
    switch (speed) {
        case -2:
            speed = -1;
            break;
            
        case -1:
            speed = 0;
            break;
            
        case 0:
            speed = 1;
            break;
            
        case 1:
            speed = 2;
            break;
            
        case 2:
            speed = 2;
            break;
            
        default:
            break;
    }
}

void rallenta(){
    switch (speed) {
        case -2:
            speed = -2;
            break;
            
        case -1:
            speed = -2;
            break;
            
        case 0:
            speed = -1;
            break;
            
        case 1:
            speed = 0;
            break;
            
        case 2:
            speed = 1;
            break;
            
        default:
            break;
    }
}

void giraDx(){
    switch (turn) {
        case -2:
            turn = -1;
            break;
            
        case -1:
            turn = 0;
            break;
            
        case 0:
            turn = 1;
            break;
            
        case 1:
            turn = 2;
            break;
            
        case 2:
            turn = 2;
            break;
            
        default:
            break;
    }
}


void giraSx(){
    switch (turn) {
        case -2:
            turn = -2;
            break;
            
        case -1:
            turn = -2;
            break;
            
        case 0:
            turn = -1;
            break;
            
        case 1:
            turn = 0;
            break;
            
        case 2:
            turn = 1;
            break;
            
        default:
            break;
    }
}


int setPWM(int vel){
    //Ci sono 3 livelli di velocità: 0, 512, 1023
    
    if (vel == 2 || vel == -2)
         return 1023;
    
    else if (vel == 1 || vel == -1)
        return 512;
    
    else
        return 0;
}

void setVerso(int vel){
    
    if (vel == 1 || vel == 2){
        digitalWrite(DA, 1);
        digitalWrite(DB, 0);
        
        digitalWrite(SA, 1);
        digitalWrite(SB, 0);
    }

    else if (vel == -1 || vel == -2){
        digitalWrite(DA, 0);
        digitalWrite(DB, 1);
        
        digitalWrite(SA, 0);
        digitalWrite(SB, 1);
    }
    
}



void aggiornaPWM(int sterzo, int vel){
    
    setVerso(vel);
    
    int differenza = 0, dPWM, sPWM;
    
    dPWM = setPWM(vel);
    sPWM = dPWM;
    
    if (sterzo == 2 || sterzo == -2)
        differenza = 500;
    
    else if (sterzo == 1 || sterzo == -1)
        differenza = 250;

    else if (sterzo == 0)
        differenza = 0;
    
    if (sterzo == 1 || sterzo == 2)
        sPWM = dPWM - differenza;
    
    if (sterzo == -1 || sterzo == -2)
        dPWM = sPWM - differenza;
    
    //gestione semplificata valori PWM negativi, si perde la possibilità di girare se speed = 0
    
    if (dPWM < 0)
        dPWM = 0;
    
    if (sPWM < 0)
        sPWM = 0;
    
    
    pwmWrite(DE, dPWM);
    pwmWrite(SE, sPWM);
    
}


void brake(){
    //brake resetta anche lo sterzo, piuttosto che frenare e basta
    
    speed = 0;
    turn = 0;
}





void stampaStato(){
    printf("Turn: %d\nSpeed: %d\n", turn, speed);
}






int main (void)
{
    
    char provola;
    
    if (wiringPiSetup() == -1){
        printf ("ATTENZIONE: wiringPI non inizializzata!\n");
        exit (1);
    }
    
    speed = 0;
    turn = 0;
    
    /*pinMode (DE, PWM_OUTPUT);
    pinMode (DA, OUTPUT);
    pinMode (DB, OUTPUT);
    
    pinMode (SE, PWM_OUTPUT);
    pinMode (SA, OUTPUT);
    pinMode (SB, OUTPUT);*/
    
    printf ("Variabili inizializzate correttamente\n");
    
    
    
    for (;;)
    {
        provola = getchar();
        
        switch (provola) {
            case 'w':
                accelera();
                aggiornaPWM(turn, speed);
                break;
            
            case 's':
                rallenta();
                aggiornaPWM(turn, speed);
                break;
                
            case 'd':
                giraDx();
                aggiornaPWM(turn, speed);
                break;
                
            case 'a':
                giraSx();
                aggiornaPWM(turn, speed);
                break;
                
            case 'b':
                brake();
                aggiornaPWM(turn, speed);
                break;

                
            default:
                break;
        }
        
        stampaStato();
    
    }
    
    return 0;
}