#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main () {
    int freq;
    
    pid_t  pid;

    freq=440;
    int volu=-10;

    for(int i;i<10;i++){
        char command[100];
        
        
        strcpy(command,"play -n -c1 synth 1 sine "); // sox command
        char buffer[50];
        sprintf(buffer,"%d", freq );
        strcat(command,buffer);
        strcat(command," vol ");
        char buffer1[50];
        sprintf(buffer1,"%d",volu );
        strcat(command,buffer1);
        strcat(command,"db");
        printf("\n%s\n",command);
        pid=fork();
        if(pid==0){
            system(command);
            exit(-1);
        }
        freq=freq*2;
        volu=volu-10;
    }
    
    for(int i;i<10;i++){
        wait(0);
    }
}