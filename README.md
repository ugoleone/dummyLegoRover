# dummyLegoRover
Simple C program for a Raspberry based LEGO rover

------------------------------------------------------------------------------------------------------------------------------

                           Ho realizzato la libreria fakePi per simulare le funzioni relative alla wiringPi.
                           Le quali comunque sono state, per il momento, commentate.
                       
                                     ! Ricordarsi l'argomento -l ncurses per il compilatore !

------------------------------------------------------------------------------------------------------------------------------


                                                INTRODUZIONE HARDWARE

Il rover è composto da due motori controllati da un L293D, l'integrato riceve 3 bit per motore. 1 di enable (E) e due per codificare il verso in cui il motore dere ruotare (A e B). Nel programma sono stati identificati come: 

DE (enable del motore destro), 
DA (primo bit direzione del motore destro),
DB (secondo bit direzione del motore destro),
per il motore sinistro vale la stessa codifica solo che è stato usato il prefisso S (ad es, SE per l'enable del motore sinistro).


                                                INTRODUZIONE SOFTWARE

Il software si articola su due livelli:

vi sono due variabili globali "speed" e "turn" che possono assumere i valori -2, -1, 0, 1, 2 a seconda dei comandi ricevuti dalla consolle.

Il secondo livello software si occupa di "leggere" i valori delle due variabili globali e definire il verso di rotazione e la velocità (tramite PWM) di rotazione dei motori.



                                            COME SI GESTISCE IL ROVER

entrambi i motori sono settati inizialmente alla stessa velocità data dalla variabile speed.

A seconda del grado di curvatura viene decrementata di 250 o di 500 la velocità del motore corrispondente al lato verso cui si vuole curvare.

Vi è poi una funzione brake() che azzera le variabili globali.

se speed = 0, il rover non gira.


                                         INTERFACCIA GRAFICA DA CONSOLLE

Tramite l'uso della libreria ncurses, è possibile "catturare" i tasti premuti sulla tastiera senza attendere la premuta del tasto "invio", e ricevere in tempo reale informazioni circa lo stato dei pin del raspberry
