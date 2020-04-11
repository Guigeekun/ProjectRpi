
// SynthPi Work in Progress

//make sure that sox is installed before running, see readme.md


//========================Lib==========================================
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
//=====================================================================

//================================Prototype============================
void tune(int);
int octSwap(int);
int harmSwap();
void scanButton(int);
void function(int);
//=====================================================================

//==============Valeur par défaut (inutile de les modifier)============
int Freq[12] ={262,277,293,311,330,349,370,392,415,440,466,493};
//les freq correspondent à l'octave 4 (définie vulgairement comme le centre du piano) voici les notes dans l'ordre :
//Do4 (C4), Do#4 (C#4), Ré4 (D4), Ré#4 (D#4), Mi4 (E4), Fa4 (F4), Fa#4 (F#4), Sol4 (G4), Sol#4 (G#4), La4 (A4), La#4 (A#4), Si4 (B4)
int HarmMode=1;
int oct=4;
//====================================================================

int globalVol=-5; // in dB, change that at your own risk, may harm your speaker and/or your hears
int Harm=10;

void scanButton (int boutton) 
{
  if (digitalRead (boutton) == HIGH)	// le bouton est pressé
    return ;

  fonction(boutton);

  while (digitalRead (boutton) == LOW)	// on attend que le bouton soit relaché
    delay (10) ;
}

void fonction(int boutton)
{
  printf("boutton" + boutton);
}

void tune(int freq){ 
//the way this function works is that it create a string which contains the sox command and then use it via the system() call

  
    pid_t  pid;
    if(oct==5){freq=freq*2;}
    if(oct==3){freq=freq*0.5;}

    int volu=globalVol; 

    for(int i;i<Harm;i++){
        char command[100];
        
        strcpy(command,"play -n -c1 synth 3 sine "); // sox command
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
        if(HarmMode){
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

int harmSwap(){
    if(HarmMode){
        HarmMode=0;
        return(HarmMode);
    }else{
        HarmMode=1;
        return(HarmMode);
    }
}


int main (void)
{
  int i, a ; //a servira à savoir si le boutton d'arret a été pressé ou non

    tune(Freq[6]);

  wiringPiSetup () ;

// Setup des inputs

  for (i = 0 ; i < 20 ; ++i)
  {
    pinMode         (i, INPUT) ;
    pullUpDnControl (i, PUD_UP) ;
    //boutton [i] = 0 ;
  }

  while (a!=0)
  {
    for (i = 0 ; i < 20 ; ++i)
      scanButton (i) ;
    delay (1) ;
  }
}