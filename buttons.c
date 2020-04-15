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

//on utilisera les 16 bouttons de la malette joyPI comme touche pour les sons
//et les 4 boutons à part pour des commandes spéciales (enregister une composition, etc)

int a;
int buttons[4][4]= {{'4','3','2','1'},
                    {'8','7','6','5'},
                    {'C','B','A','9'},
                    {'G','F','E','D'}};
int rowPins[4] = {2,3,21,22};
int columnPins[4] = {23,24,25,6};
char boutton; //valeur bu boutton pressé

void fonction(int boutton,int fre)
{
  int frequences[4]={392,440,523,698};
  char str[5];
  sprintf(str, "%d", boutton);
  printf(str);
  printf("\n");

  int freq=frequences[fre];

  tune(freq);
}


int main (void)
{
  printf ("Test boutton\n") ;

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
          fonction(buttons[j][i],i); //active la fonction associée au boutton situé ligne j colonne i
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


