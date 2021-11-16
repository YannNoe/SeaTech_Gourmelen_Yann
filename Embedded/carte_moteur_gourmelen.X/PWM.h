/* 
 * File:   PWM.h
 * Author: TP-EO-5
 *
 * Created on 19 octobre 2021, 14:57
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_GAUCHE 1
#define MOTEUR_DROIT 2

void InitPWM();
//void PWMSetSpeed(float, int);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float, char);


#endif	/* PWM_H */

