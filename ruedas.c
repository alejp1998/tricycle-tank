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

	//Creamos señales control pwm (hardware)
	pinMode(RUEDA1_PIN,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(CLOCK_DIVIDER);
	pwmSetRange(RUEDA_PWM_RANGE);
	pwmWrite(RUEDA1_PIN,p_ruedas->parado);

	pinMode(RUEDA2_PIN,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(CLOCK_DIVIDER);
	pwmSetRange(RUEDA_PWM_RANGE);
	pwmWrite(RUEDA2_PIN,p_ruedas->parado);
}


//FUNCIONES DE COMPROBACION
int CompruebaParado(fsm_t* this){
	int result = 0;
	result = (flags_player & FLAG_PARADO);
	return result;
}

int CompruebaMovimiento(fsm_t* this){
	int result = 0;
	result = (flags_player & FLAG_MOVIMIENTO);
	return result;
}

//FUNCIONES DE SALIDA O ACCION

void Parado(fsm_t* this){
	flags_player &= ~FLAG_PARADO;
	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	p_ruedas->rueda1 = STOPPED;
	p_ruedas->rueda2 = STOPPED;

	pwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	pwmWrite(RUEDA2_PIN,p_ruedas->rueda2);
}

void Movimiento(fsm_t* this){
	flags_player &= ~FLAG_MOVIMIENTO;

	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	//Normalizar el rango a uno
	if(posX > SENSIBILIDAD){
		posX = (posX-SENSIBILIDAD)/(1-SENSIBILIDAD);
	}
	if(posY > SENSIBILIDAD){
		posY = (posY-SENSIBILIDAD)/(1-SENSIBILIDAD);
	}

	//Calculo longitud pwm
	p_ruedas->rueda1 = (int) (STOPPED + RANGE*(posX - posY));
	p_ruedas->rueda2 = (int) (STOPPED + RANGE*(posX + posY));

	//Accionammiento de las ruedas
	pwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	pwmWrite(RUEDA2_PIN,p_ruedas->rueda2);
}
