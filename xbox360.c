/*
 * xbox360.c
 *
 *  Created on: 1 may. 2019
 *      Author: Ale
 */

#include "xbox360.h"

void InicializaXbox360(TipoXbox360* p_xbox360){
	p_xbox360->teclaXbox = '0';
}

int CompruebaPulsada(fsm_t* this){
	return 1;
}

void Pulsada(fsm_t* this){
	TipoXbox360 *p_xbox360;
	p_xbox360 = (TipoXbox360*)(this->user_data);
	FILE* fp;
	fp = fopen("xbox360.txt","r");//Opens the file in reading mode
	p_xbox360->teclaXbox = getc(fp);//Reads char from the file
	fclose(fp);
	switch(p_xbox360->teclaXbox){
		case 'A': //disparo
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_TRIGGER_BUTTON;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("Tecla 9 pulsada!\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'l':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_LEFT;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO LEFT!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'r':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_RIGHT;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO RIGHT!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'u':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_UP;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO UP!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'd':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_DOWN;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO DOWN!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'U':
			piLock (SYSTEM_FLAGS_KEY);
			flags_player |= FLAG_AVANZAR;
			piUnlock (SYSTEM_FLAGS_KEY);
			break;

		case 'D':
			piLock (PLAYER_FLAGS_KEY);
			flags_player |= FLAG_RETROCEDER;
			piUnlock (PLAYER_FLAGS_KEY);
			break;

		case 'L':
			piLock (PLAYER_FLAGS_KEY);
			flags_player |= FLAG_IZQUIERDA;
			piUnlock (PLAYER_FLAGS_KEY);
			break;

		case 'R':
			piLock (PLAYER_FLAGS_KEY);
			flags_player |= FLAG_DERECHA;
			piUnlock (PLAYER_FLAGS_KEY);
			break;
		case 'N':
			break;
		default:
			break;
	}
}

