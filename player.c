
#include "player.h"
#include "fsm.h"
#include "string.h"
#include "tmr.h"
//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//Inicializa el puntero del efecto que recibe
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {
	strcpy(p_efecto->nombre,nombre); //Copia string nombre en su atributo
	int i;//Introduce arrays de frecuencias y efectos, además del numero de notas
	for(i=0; i < num_notas; i++){
		p_efecto->frecuencias[i] = array_frecuencias[i];
		p_efecto->duraciones[i] = array_duraciones[i];
	}
	p_efecto->num_notas = num_notas;
	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
void InicializaPlayer (TipoPlayer *p_player) {

	p_player->posicion_nota_actual = 0;
	p_player->p_efecto = &p_player->efecto_impacto; //Asociamos inicialmente el objeto disparo
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	//Creamos temporizador de notas y apuntamos a el
	p_player->p_timer = tmr_new(timer_player_duracion_nota_actual_isr);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
// Se encargan únicamente de comprobar el estado de los flags deseados
// para saltar o no al siguiente estado
// Bloquean el flag a modificar mientras lo leen para modificar el resultado
// Para saltar al siguiente estado result debera ser distinto de necesario
// result = (flags & FLAG_X) devolvera resultado != 0 cuando el bit x de flags
// tenga un valor distinto de cero
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
// Se encarga de llevar a cabo las funciones deseadas al llegar a cada estado
// que pueden ser la activación de otros flags o la manipulacion de los objetos
// asociados a la fsm (fsm_t* this) con this->user_data
// Además deben dar por atendidos a los flags de llegada poniendo su valor a cero en
// la variable flags mediante flags &= ~FLAG_X
//------------------------------------------------------

//Comenzamos reproduccion efectos
void InicializaPlayDisparo (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	flags_player &= ~FLAG_START_IMPACTO;
	piUnlock (PLAYER_FLAGS_KEY);

	//Leemos el player
	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);
	p_player->p_efecto = &p_player->efecto_disparo;
	p_player->posicion_nota_actual = 0;
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	//Reproducimos la nota en el pin 23 y iniciamos su temporizador asociado
	softToneWrite (23, p_player->frecuencia_nota_actual);//Reproduce la nota
	tmr_startms(p_player->p_timer,p_player->duracion_nota_actual); //Comienza a contar

	piLock (STD_IO_BUFFER_KEY);
	printf("Inicializa Play Disparo\n");
	printf("Comienza Nueva Nota: 0\n");
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
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	softToneWrite (23, p_player->frecuencia_nota_actual);//Reproduce la nota
	tmr_startms(p_player->p_timer,p_player->duracion_nota_actual); //Comienza a contar

	piLock (STD_IO_BUFFER_KEY);
	printf("Inicializa Play Impacto\n");
	printf("Comienza Nueva Nota: 0\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);
}

//Si la nota anterior no era la ultima, se reproduce la siguiente
void ComienzaNuevaNota (fsm_t* this) {
	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);
	softToneWrite (23, p_player->frecuencia_nota_actual);
	tmr_startms(p_player->p_timer,p_player->duracion_nota_actual);

	piLock (STD_IO_BUFFER_KEY);
	printf("Comienza Nueva Nota: ");
	printf("%d\n",p_player->posicion_nota_actual);
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);
}

//Cuando acaba una nota(TIMEOUT) incrementa el indice y commprueba si el la ultima
void ActualizaPlayer (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);
	p_player->posicion_nota_actual++;
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
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
	//Finaliza la reproduccion de frecuencias
	softToneWrite (23, 0);

	piLock (STD_IO_BUFFER_KEY);
	printf("Final Efecto\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
// Cuando el temporizador de cada nota llega a su valor de referencia
// se ejecuta esta funcion de interrrupcion que activa el FLAG_NOTA_TIMEOUT
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
