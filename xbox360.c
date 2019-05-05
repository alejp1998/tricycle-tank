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

//Inicializamos sus valores a no pulsaciones
void InicializaXbox360(TipoXbox360* p_xbox360){
	p_xbox360->teclaXbox = 'N';
	p_xbox360->teclaTorreta = 'N';
	p_xbox360->posX = 0.0;
	p_xbox360->posY = 0.0;
}

//Cada CLK_MS se comprueban las pulsaciones del mando
int CompruebaPulsada(fsm_t* this){
	return 1;
}

//Funcion encargada de leer teclas pulsadas y realizar acciones oportunas con ellas
void Pulsada(fsm_t* this){
	TipoXbox360 *p_xbox360;
	p_xbox360 = (TipoXbox360*)(this->user_data);

	//Lectura de mando xbox
	FILE* f1;
	f1 = fopen("xbox360.txt","r");//Opens the file in reading mode
	fscanf(f1, "%s %s %lf %lf" , &(p_xbox360->teclaXbox) , &(p_xbox360->teclaTorreta) , &(p_xbox360->posX) , &(p_xbox360->posY) );
	fclose(f1);

	//Si la lectura del joystick izquierdo es mayor de 0.1, se activa el flag movimiento
	if(p_xbox360->posX>=SENSIBILIDAD || p_xbox360->posX<=-SENSIBILIDAD || p_xbox360->posY>=SENSIBILIDAD || p_xbox360->posY<=-SENSIBILIDAD){
		posX = p_xbox360->posX; //Se pasan valores leidos a la clase ruedas mediante dos 
		posY = p_xbox360->posY; //external doubles
		flags_player |= FLAG_MOVIMIENTO;
	}else{
		posX = 0.0; //Si son menores de 0.1 se pasa un 0.0 como posición leída
		posY = 0.0;
		flags_player |= FLAG_PARADO;
	}

	//Despues de que pase nrebotes*clk_ms tiempo se lee siguiente pulsacion
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

				nrebotes = NREBOTES; //Numero de clk_ms a esperar
				break;

			case 'X': //recargar
				//Permite recargar un maximo de 10 balas
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

			case 'Y': //finalizar juego
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_SYSTEM_END;
				piUnlock (SYSTEM_FLAGS_KEY);
				nrebotes = NREBOTES;
				break;

			case 'E': //comenzar juego
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

		//Switch para joystick derecho (control de torreta)
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

