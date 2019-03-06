
#include "player.h"
#include "fsm.h"
#include "string.h"
#include "tmr.h"
//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
// void InicializaEfecto (TipoEfecto *p_efecto)
//
// Descripcion: inicializa los parametros del objeto efecto.
// Ejemplo de uso:
//
// ...
//
// TipoEfecto efecto_demo;
//
// if ( InicializaEfecto (&efecto_demo, "DISPARO", frecuenciasDisparo, tiemposDisparo, 16) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaEfecto]\n");
// 		fflush(stdout);
// 		}
//
// ...
//
//------------------------------------------------------
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {
	strcpy(p_efecto->nombre,nombre);
	int i;
	for(i=0; i < num_notas; i++){
		p_efecto->frecuencias[i] = array_frecuencias[i];
		p_efecto->duraciones[i] = array_duraciones[i];
	}
	p_efecto->num_notas = num_notas;
	return p_efecto->num_notas;
}


// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {

	p_player->posicion_nota_actual = 0;
	p_player->p_efecto = &p_player->efecto_disparo;
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	p_player->p_timer = tmr_new(timer_player_duracion_nota_actual_isr);
	//Creamos temporizador y apuntamos a el
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_DISPARO);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaNuevaNota (fsm_t* this){
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = ~(flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	flags_player &= ~FLAG_START_IMPACTO;
	//flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);
	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);
	p_player->p_efecto = &p_player->efecto_disparo;
	p_player->posicion_nota_actual = 0;
	softToneWrite (23, p_player->frecuencia_nota_actual);//Reproduce la nota
	tmr_startms(p_player->p_timer,p_player->duracion_nota_actual); //Comienza a contar
	// ...
	piLock (STD_IO_BUFFER_KEY);
	printf("Inicializa Play Disparo\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

void InicializaPlayImpacto (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	flags_player &= ~FLAG_START_IMPACTO;
	piUnlock (PLAYER_FLAGS_KEY);
	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);
	p_player->p_efecto = &p_player->efecto_impacto;
	p_player->posicion_nota_actual = 0;
	softToneWrite (23, p_player->frecuencia_nota_actual);//Reproduce la nota
	tmr_startms(p_player->p_timer,p_player->duracion_nota_actual); //Comienza a contar
	// ...
	piLock (STD_IO_BUFFER_KEY);
	printf("Inicializa Play Impacto\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

void ComienzaNuevaNota (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);
	softToneWrite (23, p_player->frecuencia_nota_actual);//Reproduce la nota
	tmr_startms(p_player->p_timer,p_player->duracion_nota_actual); //Comienza a contar

	piLock (STD_IO_BUFFER_KEY);
	printf("Comienza Nueva Nota: ");
	printf("%d\n",p_player->posicion_nota_actual);
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

void ActualizaPlayer (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);
	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);
	p_player->posicion_nota_actual++;

	if (p_player->posicion_nota_actual > (p_player->p_efecto->num_notas-1)){
		flags_player |= FLAG_PLAYER_END;
	}

	piLock (STD_IO_BUFFER_KEY);
	printf("Actualiza Player\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

void FinalEfecto (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	flags_player &= ~FLAG_START_IMPACTO;
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);
	softToneWrite (23, 0);
	// ...
	piLock (STD_IO_BUFFER_KEY);
	printf("Final Efecto\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock (STD_IO_BUFFER_KEY);
	printf("TIMEOUT\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	piLock (PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT; //Activa el flag de final de nota
	piUnlock (PLAYER_FLAGS_KEY);
}
