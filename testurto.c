#include <wiringPi.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

const int urtoPin = 14;

int main (void){
    
    if (wiringPiSetup() == -1){
        printf ("ATTENZIONE: wiringPI non inizializzata!\n");
        exit (1);
    }
    
    pinMode(urtoPin, INPUT);
    pullUpDnControl(urtoPin, PUD_UP);
    
    int urto = 0;
    
    while(1){
       
        if(!digitalRead(urtoPin))
        {
            printf("inizio la retro\n");
            sleep(4);
            printf("ho finito la retro\n");
        }
        else
        {
            printf("tutto regolare\n");
            sleep(1);
        }
    }
    
    
    
    return 0;
}