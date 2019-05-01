/*
 * xbox360.h
 *
 *  Created on: 1 may. 2019
 *      Author: Ale
 */

#ifndef XBOX360_H_
#define XBOX360_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "piTankGoLib.h"

extern int flags_juego;
extern int flags_player;

//Almacena tecla pulsada en cada momento
typedef struct {
	char teclaXbox;
}TipoXbox360;

void InicializaXbox360(TipoXbox360* p_xbox360);

int CompruebaPulsada(fsm_t* this);

void Pulsada(fsm_t* this);

#endif /* XBOX360_H_ */


