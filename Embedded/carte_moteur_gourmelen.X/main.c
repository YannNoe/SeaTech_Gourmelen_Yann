#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "adc.h"

unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;

int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();

/****************************************************************************************************/
// Configuration des entrées sorties
/****************************************************************************************************/
InitIO();

InitPWM();
InitADC1();

PWMSetSpeedConsigne(40,MOTEUR_GAUCHE);
PWMSetSpeedConsigne(40,MOTEUR_DROIT);

InitTimer1();
InitTimer23();

LED_BLANCHE = 1;
LED_BLEUE = 1;
LED_ORANGE = 1;

/****************************************************************************************************/
// Boucle Principale
/****************************************************************************************************/
while(1){
//    LED_BLANCHE=!LED_BLANCHE;
//    LED_BLEUE=!LED_BLEUE;
//    LED_ORANGE=!LED_ORANGE;

   if  (ADCIsConversionFinished()==1){
       
       ADCClearConversionFinishedFlag();
       unsigned int * result = ADCGetResult();
       ADCValue0=result[0];
       ADCValue1=result[1];
       ADCValue2=result[2];  
       
   }
} // fin main
}