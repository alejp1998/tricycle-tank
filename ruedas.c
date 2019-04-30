/*
 * ruedas.c
 *
 *  Created on: 18 abr. 2019
 *      Author: Ale
 */

#include "ruedas.h"

void InicializaRuedas(TipoRuedas *p_ruedas){
	//Ruedas inicialmente paradas
	p_ruedas->parado = STOPPED;
	p_ruedas->rueda1 = p_ruedas->parado;
	p_ruedas->rueda2 = p_ruedas->parado;

	//Creamos señales control pwm
	softPwmCreate(RUEDA1_PIN,p_ruedas->parado,RUEDA_PWM_RANGE);
	softPwmWrite(RUEDA1_PIN,p_ruedas->parado);
	softPwmCreate(RUEDA2_PIN,p_ruedas->parado,RUEDA_PWM_RANGE);
	softPwmWrite(RUEDA2_PIN,p_ruedas->parado);

	//Inicializamos temporizador de movimientos
	p_ruedas->p_timer = tmr_new(timer_rueda_isr);
}


//FUNCIONES ATENCION INTERRUPCIONES
void timer_rueda_isr(union sigval value){
	piLock (PLAYER_FLAGS_KEY);
	flags_player |= FLAG_RUEDA_TIMEOUT;
	piUnlock(PLAYER_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("ACABA MOVIMIENTO");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
}


//FUNCIONES DE COMPROBACION
int CompruebaParado(fsm_t* this){
	int result = 0;
	result = (flags_player & FLAG_RUEDA_TIMEOUT);
	return result;
}

int CompruebaAvanzar(fsm_t* this){
	int result = 0;
	result = (flags_player & FLAG_AVANZAR);
	return result;
}

int CompruebaRetroceder(fsm_t* this){
	int result = 0;
	result = (flags_player & FLAG_RETROCEDER);
	return result;
}

int CompruebaDerecha(fsm_t* this){
	int result = 0;
	result = (flags_player & FLAG_DERECHA);
	return result;
}

int CompruebaIzquierda(fsm_t* this){
	int result = 0;
	result = (flags_player & FLAG_IZQUIERDA);
	return result;
}

//FUNCIONES DE SALIDA O ACCION

void Parado(fsm_t* this){
	flags_player &= ~FLAG_RUEDA_TIMEOUT;
	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	p_ruedas->rueda1 = STOPPED;
	p_ruedas->rueda2 = STOPPED;
	softPwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	softPwmWrite(RUEDA2_PIN,p_ruedas->rueda2);
}

void Avanzar(fsm_t* this){
	flags_player &= ~FLAG_AVANZAR;
	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	p_ruedas->rueda1 = CLOCKWISE;
	p_ruedas->rueda2 = COUNTERCLOCKWISE;
	softPwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	softPwmWrite(RUEDA2_PIN,p_ruedas->rueda2);

	tmr_startms(p_ruedas->p_timer,TIEMPOMOV);

	printf("RUEDAS AVANZAN");
	fflush(stdout);
}

void Retroceder(fsm_t* this){
	flags_player &= ~FLAG_RETROCEDER;
	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	p_ruedas->rueda1 = COUNTERCLOCKWISE;
	p_ruedas->rueda2 = CLOCKWISE;
	softPwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	softPwmWrite(RUEDA2_PIN,p_ruedas->rueda2);

	tmr_startms(p_ruedas->p_timer,TIEMPOMOV);

	printf("RUEDAS RETROCEDEN");
	fflush(stdout);
}

void GirarDerecha(fsm_t* this){
	flags_player &= ~FLAG_DERECHA;
	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	p_ruedas->rueda1 = COUNTERCLOCKWISE;
	p_ruedas->rueda2 = COUNTERCLOCKWISE;
	softPwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	softPwmWrite(RUEDA2_PIN,p_ruedas->rueda2);

	tmr_startms(p_ruedas->p_timer,TIEMPOMOV);

	printf("RUEDAS GIRAN DERECHA");
	fflush(stdout);
}

void GirarIzquierda(fsm_t* this){
	flags_player &= ~FLAG_IZQUIERDA;
	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	p_ruedas->rueda1 = CLOCKWISE;
	p_ruedas->rueda2 = CLOCKWISE;
	softPwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	softPwmWrite(RUEDA2_PIN,p_ruedas->rueda2);

	tmr_startms(p_ruedas->p_timer,TIEMPOMOV);

	printf("RUEDAS GIRAN IZQUIERDA");
	fflush(stdout);
}
