#include <ncurses.h>			/* ncurses.h includes stdio.h */
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <wiringPi.h>
#include <softPwm.h>

const int DE = 2;
const int DA = 3;
const int DB = 8;

const int SE = 5;
const int SA = 6;
const int SB = 4;

const int LED1 = 9;
const int LED2 = 10;

const int LED3 = 11;
const int LED4 = 12;
const int LED5 = 13;

int speed;  //valori ammessi: -1, 0, 1
int turn;   //valori ammessi: -1, 0, 1
int led;    //valori ammessi: 0, 1
int fari;   //valori ammessi: 0, 1

int dPWM, sPWM; //valori ammessi: -100, -50, 0, 50, 100. Dopo il calcolo del verso se ne prendono i moduli



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


////////////////////////////////////////////////////*** END ***///////////////////////////////////////////////////




////////////////////////////////////////////PRIMO LIVELLO DI CONTROLLO////////////////////////////////////////////

void accelera(){
    turn = 0;
    
    switch (speed) {
        case -1:
            speed = 0;
            break;
            
        case 0:
            speed = 1;
            break;
            
        case 1:
            speed = 1;
            break;
            
        default:
            break;
    }
}

void rallenta(){
    turn = 0;
    
    switch (speed) {
        case -1:
            speed = -1;
            break;
            
        case 0:
            speed = -1;
            break;
            
        case 1:
            speed = 0;
            break;
            
        default:
            break;
    }
}

void brake(){
    //brake resetta anche lo sterzo, piuttosto che frenare e basta
    
    speed = 0;
    turn = 0;
}

void giraDx(){
    switch (turn) {
            
        case -1:
            turn = 0;
            break;
            
        case 0:
            turn = 1;
            break;
            
        case 1:
            turn = 1;
            break;
            
        default:
            break;
    }
}


void giraSx(){
    switch (turn) {
      
        case -1:
            turn = -1;
            break;
            
        case 0:
            turn = -1;
            break;
            
        case 1:
            turn = 0;
            break;
            
        default:
            break;
    }
}

////////////////////////////////////////////////////*** END ***///////////////////////////////////////////////////




////////////////////////////////////////////SECONDO LIVELLO DI CONTROLLO//////////////////////////////////////////

void initVerso(){
    
    if (dPWM >= 0){
        digitalWrite(DA, HIGH);
        digitalWrite(DB, LOW);
        
        digitalINFO(DA, C_HIGH);
        digitalINFO(DB, C_LOW);
    }
    else {
        dPWM = -1 * dPWM;
        
        digitalWrite(DA, LOW);
        digitalWrite(DB, HIGH);
        
        digitalINFO(DA, C_LOW);
        digitalINFO(DB, C_HIGH);
    }
    
    if (sPWM >= 0){
        digitalWrite(SA, HIGH);
        digitalWrite(SB, LOW);
        
        digitalINFO(SA, C_HIGH);
        digitalINFO(SB, C_LOW);
    }
    else {
        sPWM = -1 * sPWM;
        
        digitalWrite(SA, LOW);
        digitalWrite(SB, HIGH);
        
        digitalINFO(SA, C_LOW);
        digitalINFO(SB, C_HIGH);
    }
}

void setMotori(){
    
    if (speed == 0) {
        switch (turn) {
            case 0:
                dPWM = 0;
                sPWM = 0;
                break;
                
            case -1:
                dPWM = 100;
                sPWM = -100;
                break;
                
            case 1:
                dPWM = -100;
                sPWM = 100;
                break;
                
            default:
                break;
        }

    }
    else {
        switch (turn) {
            case 0:
                dPWM = speed*100;
                sPWM = speed*100;
                break;
                
            case -1:
                dPWM = speed*100;
                sPWM = 0;
                break;
                
            case 1:
                dPWM = 0;
                sPWM = speed*100;
                break;
                
            default:
                break;
        }
        
    }
    
    //initVerso() trasforma i valori PWM da negativi a positivi, cambiando il verso dei motori
    
    initVerso();

    softPwmWrite (DE, dPWM);
    softPwmWrite (SE, sPWM);
    
    pwmINFO(DE, dPWM);
    pwmINFO(SE, sPWM);
    
}

////////////////////////////////////////////////////*** END ***///////////////////////////////////////////////////





//////////////////////////////////////////////////GESTIONE LUCI///////////////////////////////////////////////////

void setFARI(){
    if (fari == 0){
        fari = 1;
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
        digitalWrite(LED5, HIGH);
    }
    else{
        fari = 0;
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
        digitalWrite(LED5, LOW);
    }
}

void ledswitch(){
    led = ((led) ? 0 : 1);
}

void ledHandler(int signo){
    ledswitch();
}

void codiceFiglio(){
    
    signal(SIGUSR1, ledHandler);
    while (1) {
        while (led) {
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, LOW);
            sleep(1);
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, HIGH);
            sleep(1);
        }
        
        if (!led){
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, LOW);
        }
    }
}




////////////////////////////////////////////////////*** END ***///////////////////////////////////////////////////






void printer(){
    move(9,1);
    printw("Turn: %d   ", turn);
    move(10,1);
    printw("Speed: %d  ", speed);
    move(11,1);
    printw("Led: %d  ", led);
    move(12,1);
    printw("Fari: %d  ", fari);
    
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
    move(21,1);
    printw("l per i lampeggianti");
    move(22,1);
    printw("f per i fari");
    move(23,0);
    printw("--------------------------------------------------------------------------------");
    
    move(0,0);
    printw("Inserisci un comando: ");
    refresh();
    
}







int main (void)
{
    initscr();
    
    int c, pid, status;
    
    if (wiringPiSetup() == -1){
        printw ("ATTENZIONE: wiringPI non inizializzata!\n");
        exit (1);
    }
    
    speed = 0;
    turn = 0;
    led = 0;
    fari = 0;
    
    dPWM = 0;
    sPWM = 0;
    
    softPwmCreate (DE, 0, 100);
    softPwmCreate (SE, 0, 100);


    pinMode (DA, OUTPUT);
    pinMode (DB, OUTPUT);
    pinMode (SA, OUTPUT);
    pinMode (SB, OUTPUT);
    
    pinMode (LED1, OUTPUT);
    pinMode (LED2, OUTPUT);
    
    pinMode (LED3, OUTPUT);
    pinMode (LED4, OUTPUT);
    pinMode (LED5, OUTPUT);
    
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    
    
    //inizializzazione gestore LED
    
    pid = fork();
    
    if (pid == 0){
        codiceFiglio();
        exit(0);
    }

    
    move(14,20);
    printw ("Variabili inizializzate correttamente");
    move(0,0);
    printw("Inserisci un comando: ");
    
    
    while ((c=getch()) !=27){
        
        
        switch (c) {
            case 'w':
                accelera();
                setMotori();
                printer();
                break;
                
            case 's':
                rallenta();
                setMotori();
                printer();
                break;
                
            case 'd':
                giraDx();
                setMotori();
                printer();
                break;
                
            case 'a':
                giraSx();
                setMotori();
                printer();
                break;
                
            case 'b':
                brake();
                setMotori();
                printer();
                break;
                
            case 'l':
                kill (pid, SIGUSR1);
                ledswitch();
                printer();
                break;
                
            case 'f':
                setFARI();
                printer();
                break;
                
                
            default:
                printer();
                break;
        }
        
        
        
    }
    
    //termino il gestore LED
    kill(pid, SIGKILL);
    wait(&status);
    
    endwin();
    return 0;
}