#include "inet.h"
void sigproc(){
        signal(SIGINT,sigproc);
        printf("\tyou have pressed ctrl-c, please type <menu> option to terminate program\n");
}
void quitproc(){
        exit(0);
}

