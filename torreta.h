
#ifndef _TORRETA_H_
#define _TORRETA_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "piTankGoLib.h"

#define	SERVOX_PIN			17	//Pin correspondiente al servo horizontal
#define	SERVOY_PIN			27	//Pin correspondiente al servo vertical

#define SERVO_PWM_RANGE		400 // 100 * 400 = 40,000 µS = 40 ms // 25 Hz

#define DEBOUNCE_TIME		200 //Prevencion de rebotes en interrupcion impacto

#define SERVO_INCREMENTO	1 //Incremento de la posicion de los servos
#define SERVO_MINIMO		9 //Valor minimo del servo
#define SERVO_MAXIMO		22//Valor maximo del servo
#define SHOOT_TIMEOUT		200 //Tiempo desde que disparamos para detectar un impacto
#define PIN_DISPARO 9	//Pin donde se escribira high para disparar
#define PIN_IMPACTO 11	//Pin donde atenderemos una interrupcion de subida para el impacto

typedef struct {
	int inicio; // Valor correspondiente a la posicion inicial del servo
	int incremento; // Cuantía en que se incrementa el valor de la posicion con cada movimiento del servo
	int minimo; // Valor mínimo correspondiente a la posicion del servo
	int maximo; // Valor maximo correspondiente a la posicion del servo
	int posicion; // Posicion actual del servo
}TipoServo;

typedef struct {
	TipoServo servo_x; 	//Servo horizontal	
	TipoServo servo_y; 	//Servo vertical
	int impactos;		//Numero impactos recibidos
	tmr_t* p_timer;		//Temporizador del disparo
} TipoTorreta;

extern int flags_juego;
extern int flags_player;


// Prototipos de procedimientos de inicializacion de los objetos especificos
void InicializaTorreta (TipoTorreta *p_torreta);

// Prototipos de funciones de entrada
int CompruebaComienzo (fsm_t* this);
int VolverMove (fsm_t* this);
int CompruebaJoystickUp (fsm_t* this);
int CompruebaJoystickDown (fsm_t* fsm_player);
int CompruebaJoystickLeft (fsm_t* this);
int CompruebaJoystickRight (fsm_t* this);
int CompruebaTimeoutDisparo (fsm_t* this);
int CompruebaImpacto (fsm_t* this);
int CompruebaTriggerButton (fsm_t* this);
int CompruebaFinalJuego (fsm_t* this);

// Prototipos de funciones de salida
void ComienzaSistema (fsm_t* this);
void MueveTorretaArriba (fsm_t* this);
void MueveTorretaAbajo (fsm_t* this);
void MueveTorretaIzquierda (fsm_t* this);
void MueveTorretaDerecha (fsm_t* this);
void DisparoIR (fsm_t* this);
void FinalDisparoIR (fsm_t* this);
void ImpactoDetectado (fsm_t* this);
void FinalizaJuego (fsm_t* this);
void impacto_recibido_isr (void);

// Prototipos de procedimientos de atencion a las interrupciones
void timer_disparo_isr (union sigval value);

#endif /* _TORRETA_H_ */
