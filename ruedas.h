/*
 * ruedas.h
 *
 *  Created on: 18 abr. 2019
 *      Author: Alicia y Ale
 */

#ifndef RUEDAS_H_
#define RUEDAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "xbox360.h"
#include "piTankGoLib.h"

extern int flags_player;
extern double posX, posY;

#define	RUEDA1_PIN			18 //Salidas a configurar como PWM
#define	RUEDA2_PIN			19

#define RUEDA_PWM_RANGE		200  //Rango del PWM
#define CLOCK_DIVIDER       1920 //Divisor del reloj

#define STOPPED 15 //Ruedas paradas
#define RANGE 8 //MAXIMO 8
#define MAXIMO 23 	//Maxima longitud permitida por el servo
#define MINIMO 7	//Minima longitud permitida por el servo
#define SENSIBILIDAD 0.3 //Inclinacion a partir de la cual se tiene en cuenta el joystick

//Almacena la velocidad de las ruedas en cada momento
typedef struct {
	int rueda1; //Velocidad de la rueda 1
	int rueda2;	//Velocidad de la rueda 2
	int parado; //Valor para el que las ruedas estan paradas
}TipoRuedas;

void InicializaRuedas(TipoRuedas* p_ruedas);

int CompruebaParado(fsm_t* this);
int CompruebaMovimiento(fsm_t* this);

void Movimiento(fsm_t* this);
void Parado(fsm_t* this);

#endif /* RUEDAS_H_ */
