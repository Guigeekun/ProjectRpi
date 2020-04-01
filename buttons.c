/*objectif du script:
détecter lorsque l'un des bouttons de la raspberry est pressé et
lancer une fonction associée, qui sera à terme remplacée par une 
fonction de portaudio pour émettre un son*/

#include <stdio.h>
#include <wiringPi.h>

//on utilisera les 16 bouttons de la malette joyPI comme touche pour les sons
//et les 4 boutons à part pour des commandes spéciales (enregister une composition, etc)

//scanButton test la valeur envoyé par le l'input sélectionné

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

int main (void)
{
  int i, a ; //a servira à savoir si le boutton d'arret a été pressé ou non

  printf ("Test boutton\n") ;

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