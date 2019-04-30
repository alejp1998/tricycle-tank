/*
 * ruedas.h
 *
 *  Created on: 18 abr. 2019
 *      Author: Ale
 */

#ifndef RUEDAS_H_
#define RUEDAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "piTankGoLib.h"

extern int flags_player;

#define	RUEDA1_PIN			4 //Salidas a configurar como PWM
#define	RUEDA2_PIN			7

#define RUEDA_PWM_RANGE		400 //Rango del PWM

#define CLOCKWISE 8 //Maxima velocidad hacia clockwise
#define COUNTERCLOCKWISE 22 //Maxima hacia counterclockwise
#define STOPPED 15 //Ruedas paradas
#define TIEMPOMOV 300 //0.3 segundos por mov

//Almacena la velocidad de las ruedas en cada momento
typedef struct {
	int rueda1;
	int rueda2;
	int parado;
	tmr_t* p_timer;
}TipoRuedas;

void InicializaRuedas(TipoRuedas* p_ruedas);

void timer_rueda_isr(union sigval value);

int CompruebaParado(fsm_t* this);
int CompruebaAvanzar(fsm_t* this);
int CompruebaRetroceder(fsm_t* this);
int CompruebaDerecha(fsm_t* this);
int CompruebaIzquierda(fsm_t* this);

void Avanzar(fsm_t* this);
void Retroceder(fsm_t* this);
void GirarDerecha(fsm_t* this);
void GirarIzquierda(fsm_t* this);
void Parado(fsm_t* this);

#endif /* RUEDAS_H_ */
