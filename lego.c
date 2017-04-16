#include <ncurses.h>			/* ncurses.h includes stdio.h */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <wiringPi.h>



const int DE = 1;
const int DA = 2;
const int DB = 3;

const int SE = 4;
const int SA = 5;
const int SB = 6;

int speed;  //valori ammessi: -2, -1, 0, 1, 2
int turn;   //valori ammessi: -2, -1, 0, 1, 2



////////////////////////////////////////////////PINOUT COMUNICATION///////////////////////////////////////////////
#define C_HIGH 1
#define C_LOW 0




void digitalINFO(int pin, int value){
    
    if (pin == DA) {
        move(17,40);
        printw("PIN: %d ", pin);
        move(18,40);
        printw("DigitalOut: %d ", value);
    }
    
    if (pin == DB) {
        move(19,40);
        printw("PIN: %d ", pin);
        move(20,40);
        printw("DigitalOut: %d ", value);
    }
    
    if (pin == SA) {
        move(17,65);
        printw("PIN: %d ", pin);
        move(18,65);
        printw("DigitalOut: %d ", value);
    }

    if (pin == SB) {
        move(19,65);
        printw("PIN: %d ", pin);
        move(20,65);
        printw("DigitalOut: %d ", value);
    }
    
}

void pwmINFO(int pin, int PWM){
    
    if (pin == DE) {
        move(15,40);
        printw("PIN: %d ", pin);
        move(16,40);
        printw("PWMOut: %d     ", PWM);
    }
    
    if (pin == SE) {
        move(15,65);
        printw("PIN: %d   ", pin);
        move(16,65);
        printw("PWMOut: %d     ", PWM);
    }

}


////////////////////////////////////////////////PINOUT COMUNICATION///////////////////////////////////////////////


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
        /*digitalWrite(DA, HIGH);
        digitalWrite(DB, LOW);
        
        digitalWrite(SA, HIGH);
        digitalWrite(SB, LOW);*/
        
        digitalINFO(DA, C_HIGH);
        digitalINFO(DB, C_LOW);
        
        digitalINFO(SA, C_HIGH);
        digitalINFO(SB, C_LOW);
    }
    
    else if (vel == -1 || vel == -2){
        /*digitalWrite(DA, LOW);
        digitalWrite(DB, HIGH);
        
        digitalWrite(SA, LOW);
        digitalWrite(SB, HIGH);*/
        
        digitalINFO(DA, C_LOW);
        digitalINFO(DB, C_HIGH);
        
        digitalINFO(SA, C_LOW);
        digitalINFO(SB, C_HIGH);
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
    
    
    /*pwmWrite(DE, dPWM);
    pwmWrite(SE, sPWM);*/
    
    pwmINFO(DE, dPWM);
    pwmINFO(SE, sPWM);
    
}


void brake(){
    //brake resetta anche lo sterzo, piuttosto che frenare e basta
    
    speed = 0;
    turn = 0;
}


void printer(){
    move(10,1);
    printw("Turn: %d   ", turn);
    move(11,1);
    printw("Speed: %d  ", speed);
    
    move(14,0);
    printw("--------------------------------------------------------------------------------");
    move(15,1);
    printw("ESC per uscire");
    move(16,1);
    printw("b per frenare");
    move(17,1);
    printw("w per accelerare");
    move(18,1);
    printw("s per rallentare");
    move(19,1);
    printw("d per sterzare a destra");
    move(20,1);
    printw("a per sterzare a sinistra");
    move(21,0);
    printw("--------------------------------------------------------------------------------");
    
    move(0,0);
    printw("Inserisci un comando: ");
    refresh();
    
}


int main (void)
{
    initscr();
    
    int c;
    
    /*if (wiringPiSetup() == -1){
        printw ("ATTENZIONE: wiringPI non inizializzata!\n");
        exit (1);
    }*/
    
    speed = 0;
    turn = 0;
    
    /*pinMode (DE, PWM_OUTPUT);
    pinMode (DA, OUTPUT);
    pinMode (DB, OUTPUT);
    
    pinMode (SE, PWM_OUTPUT);
    pinMode (SA, OUTPUT);
    pinMode (SB, OUTPUT);*/
    
    move(14,20);
    printw ("Variabili inizializzate correttamente");
    move(0,0);
    printw("Inserisci un comando: ");
    
    
    while ((c=getch()) !=27){
        
        
        switch (c) {
            case 'w':
                accelera();
                aggiornaPWM(turn, speed);
                printer();
                break;
                
            case 's':
                rallenta();
                aggiornaPWM(turn, speed);
                printer();
                break;
                
            case 'd':
                giraDx();
                aggiornaPWM(turn, speed);
                printer();
                break;
                
            case 'a':
                giraSx();
                aggiornaPWM(turn, speed);
                printer();
                break;
                
            case 'b':
                brake();
                aggiornaPWM(turn, speed);
                printer();
                break;
                
                
            default:
                printer();
                break;
        }
        
        
        
    }
    
    endwin();
    return 0;
}