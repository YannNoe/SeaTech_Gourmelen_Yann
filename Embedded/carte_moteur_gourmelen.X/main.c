#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "adc.h"
#include "Robot.h"
#include "main.h"
#include "ToolBox.h"

unsigned char nextStateRobot = 0;
unsigned char stateRobot;

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entr�es sorties
    /****************************************************************************************************/
    InitIO();

    InitPWM();
    InitADC1();

    PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
    PWMSetSpeedConsigne(20, MOTEUR_DROIT);

    InitTimer1();
    InitTimer23();
    InitTimer4();

    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        if (ADCIsConversionFinished() == 1) {

            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();

            float volts = ((float) result[3]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauchePlus = 34 / volts - 5;
            if (robotState.distanceTelemetreGauchePlus <= 30)
                LED_BLANCHE = 1;
            else
                LED_BLANCHE = 0;
            
            volts = ((float) result[4]) *3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            if (robotState.distanceTelemetreGauche <= 30)
                LED_BLANCHE = 1;
            else
                LED_BLANCHE = 0;
            
            volts = ((float) result[2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            if (robotState.distanceTelemetreCentre <= 30)
                LED_BLEUE = 1;
            else
                LED_BLEUE = 0;
            
            volts = ((float) result[1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            if(robotState.distanceTelemetreDroit<=30)
                LED_ORANGE=1;
            else
                LED_ORANGE=0;
            
            volts = ((float) result[0]) *3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroitPlus = 34 / volts - 5;
            if (robotState.distanceTelemetreDroitPlus <= 30)
                LED_ORANGE = 1;
            else
                LED_ORANGE = 0;
            

        }
    }
}

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;      
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
            case STATE_TOURNE_GAUCHE_PLUS:
            PWMSetSpeedConsigne(40, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;     
        case STATE_TOURNE_GAUCHE_PLUS_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
            case STATE_TOURNE_DROITE_PLUS:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(40, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_PLUS_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char OBSTACLE_A_GAUCHE=0;
unsigned char OBSTACLE_A_GAUCHE_PLUS=0;
unsigned char OBSTACLE_A_DROITE=0;
unsigned char OBSTACLE_A_DROITE_PLUS=0;
unsigned char OBSTACLE_EN_FACE=0;

void SetNextRobotStateInAutomaticMode() {
//    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //D�termination de la position des obstacles en fonction des t�l�m�tres
    
    // Detection obstacle gauche
    
    if(robotState.distanceTelemetreGauche < 25)
        OBSTACLE_A_GAUCHE = 1;
    else
        OBSTACLE_A_GAUCHE = 0;
    
    // Detection obstacle gauche plus
    
    if(robotState.distanceTelemetreGauchePlus < 15)
        OBSTACLE_A_GAUCHE_PLUS = 1;
    else
        OBSTACLE_A_GAUCHE_PLUS = 0;
    
      // Detection obstacle droite
    
    if(robotState.distanceTelemetreDroit < 25)
        OBSTACLE_A_DROITE = 1;
    else
        OBSTACLE_A_DROITE = 0;
    
      // Detection obstacle droite plus
    
    if(robotState.distanceTelemetreDroitPlus < 15)
        OBSTACLE_A_DROITE_PLUS = 1;
    else
        OBSTACLE_A_DROITE_PLUS = 0;
    
    // Detection obstacle centre
    
    if(robotState.distanceTelemetreCentre < 30
            )
        OBSTACLE_EN_FACE = 1;
    else
        OBSTACLE_EN_FACE = 0;
    
    unsigned char code = (OBSTACLE_A_GAUCHE_PLUS<<4) ||
            (OBSTACLE_A_GAUCHE<<3) ||
            (OBSTACLE_EN_FACE<<2) ||
            (OBSTACLE_A_DROITE<<1) ||
            (OBSTACLE_A_DROITE_PLUS << 0);
    
    //Machine � �tat
    
    switch(code)
    {
    /**********************************/
    /*CAS NUMERO 1 : OBSTACLES PARTOUT*/
    /**********************************/
        case 0b11111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
    /***************************************************/
    /*CAS NUMERO 2 : OBSTACLES PARTOUT SAUF GAUCHE PLUS*/
    /***************************************************/
        case 0b01111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
    /**********************************************/
    /*CAS NUMERO 3 : OBSTACLES PARTOUT SAUF GAUCHE*/
    /**********************************************/
        case 0b10111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
    /*************************************************************/
    /*CAS NUMERO 4 : OBSTACLES PARTOUT SAUF GAUCHE ET GAUCHE PLUS*/
    /*************************************************************/
        case 0b00111:
            nextStateRobot = STATE_TOURNE_GAUCHE_PLUS;
            break;
        case 0b11011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10011:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00011:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b11101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b00101:
            nextStateRobot = STATE_TOURNE_GAUCHE_PLUS;
            break;
        case 0b11001:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01001:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b10001:
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b00001:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10110:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b00110:
            nextStateRobot = STATE_TOURNE_GAUCHE_PLUS;
            break;
        case 0b11010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b01010:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10010:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00010:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b11100:
            nextStateRobot = STATE_TOURNE_DROITE_PLUS;
            break;
        case 0b01100:
            nextStateRobot = STATE_TOURNE_DROITE_PLUS;
            break;
        case 0b10100:
            nextStateRobot = STATE_TOURNE_DROITE_PLUS;
            break;
        case 0b00100:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b11000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b10000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b00000:
            nextStateRobot = STATE_AVANCE;
            break; 
    }

    //Si l?on n?est pas dans la transition de l?�tape en cours
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
    }
}
