/*
 * xbox360.h
 *
 *  Created on: 1 may. 2019
 *      Author: Alicia y Ale
 */

#ifndef XBOX360_H_
#define XBOX360_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "piTankGoLib.h"

//Numero de intervalos CLK_MS a esperar después de cada tecla
#define NREBOTES 40

extern int flags_juego;
extern int flags_player;
extern int disparos;

//Almacena teclas pulsada en cada momento y posicion del joystick
typedef struct {
	char teclaXbox;
	char teclaTorreta;
	double posX;
	double posY;
}TipoXbox360;

void InicializaXbox360(TipoXbox360* p_xbox360);

int CompruebaPulsada(fsm_t* this);

void Pulsada(fsm_t* this);

#endif /* XBOX360_H_ */


