/*
 * piTankGo_1.h
 *
 *  Created on: 11 de enero de 2019
 *      Author: FFM
 */

#ifndef _PITANKGO_1_H_
#define _PITANKGO_1_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "fsm.h" // para poder crear y ejecutar la maquina de estados
#include "kbhit.h" // Para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas
#include "player.h"
#include "torreta.h"
#include "torreta.h"
#include "ruedas.h"
#include "teclado_TL04.h"

// Posibles estados de las FSMs
enum fsm_state {
	WAIT_START,
	WAIT_KEY,
	WAIT_PUSH,
	WAIT_NEXT,
	WAIT_MOVE,
	WAIT_END,
	JOYSTICK_UP,
	JOYSTICK_DOWN,
	JOYSTICK_LEFT,
	JOYSTICK_RIGHT,
	TRIGGER_BUTTON,
	PARADO,
	AVANZANDO,
	RETROCEDIENDO,
	GIRANDODCHA,
	GIRANDOIZQ
};

typedef struct {
	TipoTorreta torreta; // Objeto para el control de la torreta
	TipoRuedas ruedas; //Control de movimientos del tanque
	TipoPlayer player; // Reproductor de efectos
	TipoTeclado teclado; //Investigador del teclado
	char teclaPulsada;  // Variable que almacena la ultima tecla pulsada
	int debug; // Variable que habilita o deshabilita la impresion de mensajes por salida estandar

} TipoSistema;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------
int ConfiguraSistema (TipoSistema *p_sistema);
int InicializaSistema (TipoSistema *p_sistema);

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------
PI_THREAD(thread_xbox360);

#endif /* _PITANKGO_1_H_ */
