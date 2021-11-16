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
       
       
       float volts = ((float) result [2]) * 3.3 / 4096 * 3.2 ;
       robotState.distanceTelemetreDroit = 34 /volts - 5 ;
       volts = ((float) result [1])* 3.3 / 4096 * 3.2 ;
       robotState.distanceTelemetreCentre = 34 / volts - 5 ;
       volts = ((float) result [0]) *3.3 / 4096 *3.2;
       robotState.distanceTelemetreGauche = 34 / volts - 5 ;
       

       /* 
       //allumage led orange si + 30cm
       if(ADCValue0<=375)
           LED_ORANGE=1;
       else
           LED_ORANGE=0;
       
        //allumage led bleue si + 30cm
       if(ADCValue1<=375)
           LED_BLEUE=1;
       else
           LED_BLEUE=0;
       
        //allumage led blanche si + 30cm
       if(ADCValue2<=375)
           LED_BLANCHE=1;
       else
           LED_BLANCHE=0; */
   }
} // fin main
}