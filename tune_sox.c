#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define Harm 10 // number of harmonics to play, feel free to mess with that

void tune(int);
int octSwap(int);
int harmSwap();
int waveSwap();

//int Freq3[12] ={131,139,147,156,165,175,185,196,207,220,233,247}; à titre indicatif
int freq[12] ={262,277,293,311,330,349,370,392,415,440,466,493};
//Do4 (C4), Do#4 (C#4), Ré4 (D4), Ré#4 (D#4), Mi4 (E4), Fa4 (F4), Fa#4 (F#4), Sol4 (G4), Sol#4 (G#4), La4 (A4), La#4 (A#4), Si4 (B4)
//int Freq5[12] ={523,554,587,622,659,698,740,784,831,880,932,988}; à titre indicatif
int globalVol=-5; // in dB, change that at your own risk, may harm your speaker and/or your hears

int oct=4;
int harmMode=1;
int waveForm=1;

int main () {

    tune(freq[5]);
    sleep(1);
    printf("wtf");
    tune(freq[2]);

}

void tune(int freq){ //make sure that sox is installed before running
//the way this function works is that it create a string which contains the sox command and then use it via the system() call

  
    pid_t  pid;
    if(oct==5){freq=freq*2;}
    if(oct==3){freq=freq*0.5;}

    int volu=globalVol; 

    for(int i;i<Harm;i++){
        char command[100];
        
        strcpy(command,"play -n -c1 synth 0.1 "); // sox command
        if(waveForm){
            strcat(command,"sine "); 
        }else{
            strcat(command,"square ");
        }
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
        //maybe we can get some special sounds by messing with that
        if(harmMode){
            freq=freq*1.5;
            volu=globalVol*(i+1);
        }else{
            freq=freq*2;
            volu=globalVol-7.5*i; //here you can mess with the harmonic spectrum
        }
    }
    
    for(int i;i<Harm;i++){
        
        wait(0);
    }

}

int octSwap(int flag){ //le flag 0 indique que l'on veut descendre d'un octave / le flag 1 indique que l'on veut monter d'un octave
  //on limite les octaves accessible à 3-4-5 pour la sécurité de vos oreilles
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

int harmSwap(){
    if(harmMode){
        harmMode=0;
        return(harmMode);
    }else{
        harmMode=1;
        return(harmMode);
    }
}

int waveSwap(){
    if(waveForm){
        waveForm=0;
        globalVol=-5; //la waveForm square sonne plus fort que le sinus on adapte donc le volume
        return(waveForm);
    }else{
        waveForm=1;
        globalVol=+5;
        return(waveForm);
    }
    
}
