/*
 * ruedas.c
 *
 *  Created on: 18 abr. 2019
 *      Author: Alicia y Ale
 */

#include "ruedas.h"

void InicializaRuedas(TipoRuedas *p_ruedas){
	//Ruedas inicialmente paradas
	p_ruedas->parado = STOPPED;
	p_ruedas->rueda1 = p_ruedas->parado;
	p_ruedas->rueda2 = p_ruedas->parado;

	//Creamos señales control pwm (hardware)
	//Para la rueda 1
	pinMode(RUEDA1_PIN,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(CLOCK_DIVIDER);
	pwmSetRange(RUEDA_PWM_RANGE);
	pwmWrite(RUEDA1_PIN,p_ruedas->parado);
	//Para la rueda 2
	pinMode(RUEDA2_PIN,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(CLOCK_DIVIDER);
	pwmSetRange(RUEDA_PWM_RANGE);
	pwmWrite(RUEDA2_PIN,p_ruedas->parado);
}


//FUNCIONES DE COMPROBACION
int CompruebaParado(fsm_t* this){
	int result = 0;
	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PARADO);
	piUnlock (PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaMovimiento(fsm_t* this){
	int result = 0;
	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_MOVIMIENTO);
	piUnlock (PLAYER_FLAGS_KEY);
	return result;
}

//FUNCIONES DE SALIDA O ACCION
void Parado(fsm_t* this){
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PARADO;
	piUnlock (PLAYER_FLAGS_KEY);

	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	//Paramos ambas ruedas
	p_ruedas->rueda1 = STOPPED;
	p_ruedas->rueda2 = STOPPED;

	pwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	pwmWrite(RUEDA2_PIN,p_ruedas->rueda2);
}

void Movimiento(fsm_t* this){
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_MOVIMIENTO;
	piUnlock (PLAYER_FLAGS_KEY);
	
	TipoRuedas *p_ruedas;
	p_ruedas = (TipoRuedas*)(this->user_data);

	//Normalizamos el valor de posicion segun la sensibilidad deseada
	if(posX > SENSIBILIDAD){
		posX = (posX-SENSIBILIDAD)/(1-SENSIBILIDAD);
	}else if(posX < (-SENSIBILIDAD)){
		posX = (posX+SENSIBILIDAD)/(1-SENSIBILIDAD);
	}else{
		posX = 0.0;
	}
	if(posY > SENSIBILIDAD){
		posY = (posY-SENSIBILIDAD)/(1-SENSIBILIDAD);
	}else if(posY < (-SENSIBILIDAD)){
		posY = (posY+SENSIBILIDAD)/(1-SENSIBILIDAD);
	}else{
		posY = 0.0;
	}

	//Calculo longitud pwm para cada rueda
	p_ruedas->rueda1 = (int) (STOPPED + RANGE*(posX - posY));
	p_ruedas->rueda2 = (int) (STOPPED + RANGE*(posX + posY));

	//Nos aseguramos de que los valores no sobrepasan el maximo ni el minimo permitidos por el servo
	if(p_ruedas->rueda1 > MAXIMO){
		p_ruedas->rueda1 = MAXIMO;
	}else if(p_ruedas->rueda1 < MINIMO){
		p_ruedas->rueda1 = MINIMO;
	}
	if(p_ruedas->rueda2 > MAXIMO){
		p_ruedas->rueda2 = MAXIMO;
	}else if(p_ruedas->rueda2 < MINIMO){
		p_ruedas->rueda2 = MINIMO;
	}

	//Accionammiento de las ruedas
	pwmWrite(RUEDA1_PIN,p_ruedas->rueda1);
	pwmWrite(RUEDA2_PIN,p_ruedas->rueda2);
}
