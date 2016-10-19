#include "inet.h"
void sigproc(){
        signal(SIGINT,sigproc);
        printf("\tyou have pressed ctrl-c if you want force to quit press, ctrl-\\\n");
}
void quitproc(){
        printf("ctrl-\\ pressed to terminate the server\n");
        exit(0);
}

