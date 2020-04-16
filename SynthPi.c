/*objectif du script:
détecter lorsque l'un des bouttons de la raspberry est pressé et
lancer une fonction associée, qui sera à terme remplacée par une 
fonction de portaudio pour émettre un son*/

#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define Harm 10 //nombre d'harmoniques qui vont etre présents dans le son produit

void tune(int);
int octSwap(int);
int harmSwap();
int waveSwap();

//on utilisera les 16 bouttons de la malette joyPI comme touche pour les sons


int a;
int buttons[3][4]= {{1,2,3,4},
                    {5,6,7,8},
                    {9,10,11,12}};
int rowPins[4] = {2,3,21,22};
int columnPins[4] = {23,24,25,6};
char boutton; //valeur bu boutton pressé
int freq[12] ={262,277,293,311,330,349,370,392,415,440,466,493};
int globalVol=-5;

int oct=4;
int harmMode=1;
int waveForm=1;



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
        printf("\n signal carré \n");
        globalVol=-5; //la waveForm square sonne plus fort que le sinus on adapte donc le volume
        return(waveForm);
    }else{
        waveForm=1;
        globalVol=+5;
        printf("\n signal sinusoïdal \n");
        return(waveForm);
    }
    
}

int main (void)
{
  wiringPiSetup () ;

  // Setup des inputs et outputs

  for (int i = 0 ; i < 4 ; i++)
  {
    const int row=rowPins[i];
    const int column=columnPins[i];
    pinMode         (row, INPUT) ;
    pinMode (column, OUTPUT);
    pullUpDnControl (row, PUD_UP) ;
    digitalWrite(column,1);
  }

  while (a!=1)
  {
    for (int i = 0 ; i < 4 ; i++) //on parcourt les colonnes
    {
      digitalWrite (columnPins[i],0);
      for (int j = 0 ; j < 4 ; j++) //on parcourt les lignes
      {
        if (digitalRead (rowPins[j]) == 0)
        {
          if (j == 3)
          {
            if (i == 0)
            {
              octSwap(0);
              printf("\n on descend d'un octave \n");
            }
            if (i == 1)
            {
              octSwap(1);
              printf("\n on monte d'un octave \n");
            }
            if (i == 2)
            {
              harmSwap();
              printf("\n changement d'harmoniques \n");
            }
            if (i == 3)
            {
              waveSwap();
            }
          }
          else
          {
            int fre = buttons[j][i] - 1;
            tune(freq[fre]); //joue la fréquence associée au boutton pressé
            printf("\n \n \n");
          }
          
          while (digitalRead (rowPins[j]) == 0)
          {
            delay(10);
          }
        }
      }
      digitalWrite(columnPins[i],1);
    }
  }
}


