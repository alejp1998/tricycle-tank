/*
 * xbox360.c
 *
 *  Created on: 1 may. 2019
 *      Author: Alicia y Ale
 */

#include "xbox360.h"

double posX,posY = 0.0;
int nrebotes = 0;
int nsong = 1;

void InicializaXbox360(TipoXbox360* p_xbox360){
	p_xbox360->teclaXbox = 'N';
	p_xbox360->teclaTorreta = 'N';
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
	FILE* f1;
	f1 = fopen("xbox360.txt","r");//Opens the file in reading mode
	fscanf(f1, "%s %s %lf %lf" , &(p_xbox360->teclaXbox) , &(p_xbox360->teclaTorreta) , &(p_xbox360->posX) , &(p_xbox360->posY) );
	fclose(f1);

	if(p_xbox360->posX>=0.1 || p_xbox360->posX<=-0.1 || p_xbox360->posY>=0.1 || p_xbox360->posY<=-0.1){
		posX = p_xbox360->posX;
		posY = p_xbox360->posY;
		flags_player |= FLAG_MOVIMIENTO;
	}else{
		posX = 0.0;
		posY = 0.0;
		flags_player |= FLAG_PARADO;
	}

	//Keys reading
	if(nrebotes>0){
		nrebotes--;
	}else{
		//Switch para teclas normales
		switch(p_xbox360->teclaXbox){
			case 'T': //disparo
				//Si nos quedamos sin balas no se activa el flag(no se dispara)
				if(disparos>0){
					piLock (SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_TRIGGER_BUTTON;
					piUnlock (SYSTEM_FLAGS_KEY);
				}

				nrebotes = NREBOTES;
				break;

			case 'X': //recargar
				if(disparos<10){
					disparos++;
				}
				nrebotes = NREBOTES;
				break;

			case 'A': //reproducir efecto libre
				piLock (PLAYER_FLAGS_KEY);
				flags_player |= FLAG_START_EFECTO;
				piUnlock (PLAYER_FLAGS_KEY);
				nrebotes = NREBOTES;
				break;

			case 'B': //finalizar efecto en reproduccion
				piLock (PLAYER_FLAGS_KEY);
				flags_player |= FLAG_PLAYER_END;
				piUnlock (PLAYER_FLAGS_KEY);
				nrebotes = NREBOTES;
				break;

			case 'E': //finalizar juego
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_SYSTEM_END;
				piUnlock (SYSTEM_FLAGS_KEY);
				nrebotes = NREBOTES;
				break;

			case 'Y': //comenzar juego
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_SYSTEM_START;
				piUnlock (SYSTEM_FLAGS_KEY);
				nrebotes = NREBOTES;
				break;

			case 'l': //selecciona despacito
				nsong = 1;

				nrebotes = NREBOTES;
				break;

			case 'r': //selecciona GOT
				nsong = 2;

				nrebotes = NREBOTES;
				break;

			case 'u': //selecciona tetris
				nsong = 3;

				nrebotes = NREBOTES;
				break;

			case 'd': //selecciona star wars
				nsong = 4;

				nrebotes = NREBOTES;
				break;

			case 'N':
				break;

			default:
				break;
		}

		//Switch para joystick izquierdo
		switch(p_xbox360->teclaTorreta){
			case 'L': //mueve torreta izquierda
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_LEFT;
				piUnlock (SYSTEM_FLAGS_KEY);

				nrebotes = NREBOTES;
				break;

			case 'R': //mueve torreta derecha
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_RIGHT;
				piUnlock (SYSTEM_FLAGS_KEY);

				nrebotes = NREBOTES;
				break;

			case 'U': ////mueve torreta arriba
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_UP;
				piUnlock (SYSTEM_FLAGS_KEY);

				nrebotes = NREBOTES;
				break;

			case 'D': //mueve torreta abajo
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_DOWN;
				piUnlock (SYSTEM_FLAGS_KEY);

				nrebotes = NREBOTES;
				break;

			case 'N':
				break;

			default:
				break;
		}
	}
}

