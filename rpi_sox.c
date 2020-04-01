#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
    int freq;
    char command[100];

    for(int i;i<10;i++){
        int volu=-10-5*i;
        freq=freq*i;
        strcpy(command,"play -n -c1 synth 10 sine ");
        strcat(command,(char)freq);
        strcat(command," vol ");
        strcat(command,(char)volu);
        strcat(command,"db");
        printf(command);
        system(command);
    }
    

}