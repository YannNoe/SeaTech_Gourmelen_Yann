#ifndef ROBOT_H
#define ROBOT_H

typedef struct robotStateBITS{

union{
struct{
unsigned char taskEnCours;

//MOTEUR GAUCHE VARIABLES
float vitesseGaucheConsigne;
float vitesseGaucheCommandeCourante;
//MOTEUR DROIT VARIABLES
float vitesseDroiteConsigne;
float vitesseDroiteCommandeCourante;

};
};
} 

ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */