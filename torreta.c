
#include "torreta.h"

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTorreta (TipoTorreta *p_torreta) {

	//Inicializamos servo horizontal(x)
	p_torreta->servo_x.incremento = SERVO_INCREMENTO;
	p_torreta->servo_x.minimo 	= SERVO_MINIMO;
	p_torreta->servo_x.maximo 	= SERVO_MAXIMO;

	p_torreta->servo_x.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->servo_x.posicion 	= p_torreta->servo_x.inicio;

	if(p_torreta->servo_x.posicion > p_torreta->servo_x.maximo)
		p_torreta->servo_x.posicion = p_torreta->servo_x.maximo;

	if(p_torreta->servo_x.posicion < p_torreta->servo_x.minimo)
		p_torreta->servo_x.posicion = p_torreta->servo_x.minimo;

	softPwmCreate (SERVOX_PIN, p_torreta->servo_y.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVOX_PIN, p_torreta->servo_y.posicion);

	//Inicializamos servo vertical(y)
	p_torreta->servo_y.incremento = SERVO_INCREMENTO;
	p_torreta->servo_y.minimo 	= SERVO_MINIMO;
	p_torreta->servo_y.maximo 	= SERVO_MAXIMO;

	p_torreta->servo_y.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->servo_y.posicion 	= p_torreta->servo_y.inicio;

	if(p_torreta->servo_y.posicion > p_torreta->servo_y.maximo)
		p_torreta->servo_y.posicion = p_torreta->servo_y.maximo;

	if(p_torreta->servo_y.posicion < p_torreta->servo_y.minimo)
		p_torreta->servo_y.posicion = p_torreta->servo_y.minimo;

	softPwmCreate (SERVOY_PIN, p_torreta->servo_y.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVOY_PIN, p_torreta->servo_y.posicion);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...

	return result;
}

int VolverMove (fsm_t* this) {
	return 1;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;
	result = (flags_juego & FLAG_JOYSTICK_UP);
	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;
	result = (flags_juego & FLAG_JOYSTICK_DOWN);
	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;
	result = (flags_juego & FLAG_JOYSTICK_LEFT);
	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;
	result = (flags_juego & FLAG_JOYSTICK_RIGHT);
	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...

	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...

	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...

	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	flags_juego &= (~FLAG_JOYSTICK_UP);

	if(p_torreta->servo_y.posicion - p_torreta->servo_y.incremento >= p_torreta->servo_y.minimo) {
		p_torreta->servo_y.posicion = p_torreta->servo_y.posicion - p_torreta->servo_y.incremento;

		softPwmWrite(SERVOY_PIN, p_torreta->servo_y.posicion);

		printf("[SERVOY][POSICION]=[%d]\n", p_torreta->servo_y.posicion);
		fflush(stdout);
	}
}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	flags_juego &= (~FLAG_JOYSTICK_DOWN);

	if(p_torreta->servo_y.posicion + p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo) {
		p_torreta->servo_y.posicion = p_torreta->servo_y.posicion + p_torreta->servo_y.incremento;

		softPwmWrite(SERVOY_PIN, p_torreta->servo_y.posicion);

		printf("[SERVOY][POSICION]=[%d]\n", p_torreta->servo_y.posicion);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	flags_juego &= (~FLAG_JOYSTICK_LEFT);

	if(p_torreta->servo_x.posicion - p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo) {
		p_torreta->servo_x.posicion = p_torreta->servo_x.posicion - p_torreta->servo_x.incremento;

		softPwmWrite(SERVOX_PIN, p_torreta->servo_x.posicion);

		printf("[SERVOX][POSICION]=[%d]\n", p_torreta->servo_x.posicion);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	flags_juego &= (~FLAG_JOYSTICK_RIGHT);

	if(p_torreta->servo_x.posicion + p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo) {
		p_torreta->servo_x.posicion = p_torreta->servo_x.posicion + p_torreta->servo_x.incremento;

		softPwmWrite(SERVOX_PIN, p_torreta->servo_x.posicion);

		printf("[SERVOX][POSICION]=[%d]\n", p_torreta->servo_x.posicion);
		fflush(stdout);
	}
}

void DisparoIR (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void FinalDisparoIR (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void ImpactoDetectado (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void FinalizaJuego (fsm_t* this) {
	// A completar por el alumno
	// ...
}
