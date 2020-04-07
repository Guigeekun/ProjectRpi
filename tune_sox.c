#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define Harm 15 // number of harmonics to play, feel free to mess with that

void tune(int);
void octSwap(int);

//int Freq3[12] ={131,139,147,156,165,175,185,196,207,220,233,247}; à titre indicatif
int Freq[12] ={262,277,293,311,330,349,370,392,415,440,466,493};
//int Freq5[12] ={523,554,587,622,659,698,740,784,831,880,932,988}; à titre indicatif

int oct=4;

int main () {

    octSwap(0);
    tune(Freq[0]);
}

void tune(int freq){ //make sure that sox is installed before running
//the way this function works is that it create a string which contains the sox command and then use it via the system() call

  
    pid_t  pid;
    if(oct==5){freq=freq*2;}
    if(oct==3){freq=freq*0.5;}

    int volu=-10; // change that at your own risk, may harm your speaker and/or your hears

    for(int i;i<Harm;i++){
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
        if(pid==0){ //to play all the harmonics at the same time we need to do some kind of multi threading
            system(command);
            sleep(1);
            exit(-1);
        }
        freq=freq*2; //maybe we can get some special sounds by messing with that
        volu=volu-10; //here you can mess with the harmonic spectrum
    }
    
    for(int i;i<Harm;i++){
        
        wait(0);
    }

}

void octSwap(int flag){ //le flag 0 indique que l'on veut descendre d'un octave / le flag 1 indique que l'on veut monter d'un octave
  //WARNING techniquement on peut utiliser cette commande une infinité de fois pour avoir des fréquence tres basses ou tres haute ... ne faites pas ca si vous tenez à votre matériel et vos oreilles
    if(flag==0){
        if(oct==3){
            return(oct); //sécurité pour eviter les fréquence trop basses
        }
        oct--;
    }else{
        if(oct==5){
            return(oct); //sécurité pour eviter les fréquence trop hautes
        }
        oct++;
    }
}
