
#include "servo_basico.h"

int InicializaServo (TipoServo *p_servo) {
	int result = 0;

	wiringPiSetupGpio();

	p_servo->incremento = SERVO_INCREMENTO;
	p_servo->minimo 	= SERVO_MINIMO;
	p_servo->maximo 	= SERVO_MAXIMO;

	p_servo->inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_servo->posicion 	= p_servo->inicio;

	if(p_servo->posicion > p_servo->maximo)
		p_servo->posicion = p_servo->maximo;

	if(p_servo->posicion < p_servo->minimo)
		p_servo->posicion = p_servo->minimo;

	softPwmCreate (SERVO_PIN, p_servo->inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVO_PIN, p_servo->posicion);

	return result;
}

int CompruebaIzquierda (fsm_t* this) {
	int result = 0;
	result = (flags_juego & FLAG_JOYSTICK_LEFT);
	return result;
}

int CompruebaDerecha (fsm_t* this) {
	int result = 0;
	result = (flags_juego & FLAG_JOYSTICK_RIGHT);
	return result;
}

void MueveServoIzquierda (fsm_t* this) {
	TipoServo *p_servo;
	p_servo = (TipoServo*)(this->user_data);

	flags_player &= (~FLAG_JOYSTICK_LEFT);

	if(p_servo->posicion - p_servo->incremento >= p_servo->minimo) {
		p_servo->posicion = p_servo->posicion - p_servo->incremento;

		softPwmWrite(SERVO_PIN, p_servo->posicion);

		printf("[SERVO][POSICION]=[%d]\n", p_servo->posicion);
		fflush(stdout);
	}
}

void MueveServoDerecha (fsm_t* this) {
	TipoServo *p_servo;
	p_servo = (TipoServo*)(this->user_data);

	flags_player &= (~FLAG_JOYSTICK_RIGHT);

	if(p_servo->posicion + p_servo->incremento <= p_servo->maximo) {
		p_servo->posicion = p_servo->posicion + p_servo->incremento;

		softPwmWrite(SERVO_PIN, p_servo->posicion);

		printf("[SERVO][POSICION]=[%d]\n", p_servo->posicion);
		fflush(stdout);
	}
}

int comprueba_teclado_PC (fsm_t* this) {
	if(kbhit())
		return 1;

	return 0;
}
