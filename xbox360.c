/*
 * xbox360.c
 *
 *  Created on: 1 may. 2019
 *      Author: Ale
 */

#include "xbox360.h"

double posX,posY = 0.0;

void InicializaXbox360(TipoXbox360* p_xbox360){
	p_xbox360->teclaXbox = 'N';
	p_xbox360->posX = 0.0;
	p_xbox360->posY = 0.0;
}

int CompruebaPulsada(fsm_t* this){
	return 1;
}

void Pulsada(fsm_t* this){
	TipoXbox360 *p_xbox360;
	p_xbox360 = (TipoXbox360*)(this->user_data);

	//Wheels speed reading
	FILE* f2;
	f2 = fopen("xbox360-2.txt","r");//Opens the file in reading mode
	fscanf(f2,"%lf", &(p_xbox360->posX));
	fclose(f2);

	FILE* f3;
	f3 = fopen("xbox360-3.txt","r");//Opens the file in reading mode
	fscanf(f3,"%lf", &(p_xbox360->posY));
	fclose(f3);

	if(p_xbox360->posX>=0.2 ||p_xbox360->posX<=-0.2 || p_xbox360->posY>=0.2 || p_xbox360->posY<=-0.2){
		flags_player |= FLAG_MOVIMIENTO;
	}else{
		flags_player |= FLAG_PARADO;
	}

	posX = p_xbox360->posX;
	posY = p_xbox360->posY;

	//Keys reading
	FILE* f1;
	f1 = fopen("xbox360-1.txt","r");//Opens the file in reading mode
	p_xbox360->teclaXbox = getc(f1);//Reads char from the file
	fclose(f1);
	switch(p_xbox360->teclaXbox){
		case 'A': //disparo
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_TRIGGER_BUTTON;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("TECLA A PULSADA - PIUUUUUUUUUUUUUM!\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'L':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_LEFT;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO LEFT!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'R':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_RIGHT;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO RIGHT!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'U':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_UP;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO UP!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;

		case 'D':
			piLock (SYSTEM_FLAGS_KEY);
			flags_juego |= FLAG_JOYSTICK_DOWN;
			piUnlock (SYSTEM_FLAGS_KEY);

			piLock (STD_IO_BUFFER_KEY);
			printf("\n[PULSACION][SERVO DOWN!!!!]\n");
			fflush(stdout);
			piUnlock (STD_IO_BUFFER_KEY);
			break;
		case 'N':
			break;
		default:
			break;
	}
}

