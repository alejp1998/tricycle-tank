
#include "piTankGo_1.h"

int frecuenciaDespacito[160] = {0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,0,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318,659,659,659,659,659,659,659,659,554,587,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318};
int tiempoDespacito[160] = {1200,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,800,300,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,450,1800,150,150,150,150,300,150,300,150,150,150,300,150,300,450,450,300,150,150,225,75,150,150,300,450,800,150,150,300,150,150,300,450,150,150,150,150,150,150,150,150,300,300,150,150,150,150,150,150,450,150,150,150,300,150,300,450,450,300,150,150,150,300,150,300,450,800,150,150,300,150,150,300,450};
int frecuenciaGOT[518] = {1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,1175,0,1397,0,932,0,1244,0,1175,0,1397,0,932,0,1244,0,1175,0,1046,0,831,0,698,0,523,0,349,0,784,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,587,0,622,0,587,0,523,0,587,0,784,0,880,0,932,0,1046,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1046,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,880,0,784,0,932,0,1244,0,0,1397,0,0,932,0,0,1175,0,0,1244,0,0,1175,0,0,932,0,0,1046,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,0,0,0,2794,0,0,0,0,3136,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,1397,0,0,0,2350,0,0,0,2489,0,0,0,2350,0,0,0,0,2093,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865};
int tiempoGOT[518] = {900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1400,1400,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,116,267,28,267,28,267,28,900,89,900,89,1400,89,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,69,7,69,7,69,7,69,7,267,28,400,45,133,13,267,28,267,28,267,28,300,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,900,89,900,133,13,150,133,13,150,1200,1800,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,267,28,1200,400,133,13,133,13,150,900,89,900,900,89,900,600,59,600,267,28,300,600,59,600,267,28,300,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,133,13,267,28,267,28,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,900,89,900,900,900,900,89,900,900,900,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,600,212,133,13,150,150,267,28,300,300,400,45,450,450,133,13,150,150,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400,116,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400};
int frecuenciaTetris[55] = {1319,988,1047,1175,1047,988,880,880,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,1175,1397,1760,1568,1397,1319,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,659,523,587,494,523,440,415,659,523,587,494,523,659,880,831};
int tiempoTetris[55] = {450,225,225,450,225,225,450,225,225,450,225,225,450,225,225,450,450,450,450,450,675,450,225,450,225,225,675,225,450,225,225,450,225,225,450,450,450,450,450,450,900,900,900,900,900,900,1800,900,900,900,900,450,450,900,1800};
int frecuenciaStarwars[59] = {523,0,523,0,523,0,698,0,1046,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,1760,0,0,784,0,523,0,0,523,0,0,523,0};
int tiempoStarwars[59] = {134,134,134,134,134,134,536,134,536,134,134,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,429,357,1071,268,67,67,268,67,67,67,67,67};

int frecuenciasDisparo[16] = {2500,2400,2300,2200,2100,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000};
int tiemposDisparo[16] = {75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75};
int frecuenciasImpacto[32] = {97,109,79,121,80,127,123,75,119,96,71,101,98,113,92,70,114,75,86,103,126,118,128,77,114,119,72};
int tiemposImpacto[32] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};

int flags_juego = 0;
int flags_player = 0;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion del sistema.
// configurar el uso de posibles librerías (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
int ConfiguraSistema (TipoSistema *p_sistema) { //Configuramos el sistema del puntero p_sistema
	int result = 0;

	//set wiringPi library
	if (wiringPiSetupGpio () < 0) {
			piLock (STD_IO_BUFFER_KEY);
			printf ("Unable to setup wiringPi\n");
			piUnlock (STD_IO_BUFFER_KEY);

			return -1;
	}


	//Establecemos pin 23 como salida y lo usamos para reproducir las notas de los efectos
	pinMode(23,OUTPUT);
	softToneCreate(23);
	softToneWrite (23,0);

	return result;
}

// int InicializaSistema (TipoSistema *p_sistema): procedimiento de inicializacion del sistema.
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// la torreta, los efectos, etc.
int InicializaSistema (TipoSistema *p_sistema) { //Inicializamos el sistema y sus atributos(player...)
	int result = 0;
	//Inicializamos los efectos dentro del objeto player
	InicializaEfecto (&p_sistema->player.efecto_disparo, "DISPARO" , frecuenciasDisparo, tiemposDisparo, 16);
	InicializaEfecto (&p_sistema->player.efecto_impacto, "IMPACTO" , frecuenciasImpacto, tiemposImpacto, 32);

	//Inicializamos el objeto player p_sistema->player es el objeto player bajo el puntero al sistema
	InicializaPlayer(&(p_sistema->player));

	//Inicializamos el objeto servo
	InicializaTorreta (&p_sistema->torreta);

	//Inicializamos el objeto ruedas
	InicializaRuedas (&p_sistema->ruedas);

	return result;
}

PI_THREAD (thread_xbox360) {
	char teclaXbox;
	while(1) {
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms
		*fp = FILE *fopen("xbox360",w);//Opens the file in writing mode
		teclaXbox = getc(*fp);//Reads char from the file
		fclose(*fp);//Closes the file

		switch(teclaXbox) {
			case 'A':
				printf("Boton A pulsado!\n");
				fflush(stdout);
				break;

			case 'q':
				exit(0);//When pressing start key
				break;

			default:
				//No keys pressed
				break;
		}
	
	}
}

//Funcion de retardo que se encarga de sincronizar el automata
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

int main ()
{
	//Declaramos el sistema a usar
	TipoSistema sistema;
	unsigned int next;

	// Configuracion e inicializacion del sistema
	ConfiguraSistema (&sistema);

	//Inicializamos todos los componentes del sistema
	InicializaSistema (&sistema);

	//TRANSICIONES MAQUINAS DE ESTADOS
	//ESTADO DE PARTIDA , COMPROBACION DE FLAG, ESTADO FINAL(si se cumple comprobacion), FUNCION A ejecutar

	//Reproductor de efectos
	fsm_trans_t reproductor[] = {
			{ WAIT_START, CompruebaStartDisparo, WAIT_NEXT, InicializaPlayDisparo },
			{ WAIT_START, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
			{ WAIT_NEXT, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
			{ WAIT_NEXT, CompruebaNotaTimeout, WAIT_END, ActualizaPlayer },
			{ WAIT_END, CompruebaFinalEfecto, WAIT_START, FinalEfecto },
			{ WAIT_END, CompruebaNuevaNota, WAIT_NEXT, ComienzaNuevaNota},
			{-1, NULL, -1, NULL },
	};

	//Control ruedas
	fsm_trans_t ruedas[] = {
			{PARADO, CompruebaAvanzar, AVANZANDO, Avanzar },
			{AVANZANDO, CompruebaParado, PARADO, Parado },
			{PARADO, CompruebaRetroceder, RETROCEDIENDO, Retroceder },
			{RETROCEDIENDO, CompruebaParado, PARADO, Parado },
			{PARADO, CompruebaDerecha, GIRANDODCHA, GirarDerecha },
			{GIRANDODCHA, CompruebaParado, PARADO, Parado },
			{PARADO, CompruebaIzquierda, GIRANDOIZQ, GirarIzquierda },
			{GIRANDOIZQ, CompruebaParado, PARADO, Parado },
			{-1,NULL,-1,NULL}
	};

	//Control torreta
	fsm_trans_t torreta[] = {
			{ WAIT_START, CompruebaComienzo, WAIT_MOVE, ComienzaSistema },
			{ WAIT_MOVE, CompruebaJoystickUp, JOYSTICK_UP, MueveTorretaArriba },
			{ JOYSTICK_UP, VolverMove, WAIT_MOVE, NULL },
			{ WAIT_MOVE, CompruebaJoystickDown, JOYSTICK_DOWN, MueveTorretaAbajo },
			{ JOYSTICK_DOWN, VolverMove, WAIT_MOVE, NULL },
			{ WAIT_MOVE, CompruebaJoystickLeft, JOYSTICK_LEFT, MueveTorretaIzquierda },
			{ JOYSTICK_LEFT, VolverMove, WAIT_MOVE, NULL },
			{ WAIT_MOVE, CompruebaJoystickRight, JOYSTICK_RIGHT, MueveTorretaDerecha },
			{ JOYSTICK_RIGHT, VolverMove, WAIT_MOVE, NULL },
			{ WAIT_MOVE, CompruebaTriggerButton, TRIGGER_BUTTON, DisparoIR },
			{ TRIGGER_BUTTON, CompruebaImpacto, WAIT_MOVE, ImpactoDetectado },
			{ TRIGGER_BUTTON, CompruebaTimeoutDisparo, WAIT_MOVE, FinalDisparoIR },
			{ WAIT_MOVE, CompruebaFinalJuego, WAIT_END, FinalizaJuego },
			{-1, NULL, -1, NULL },
		};

	//Creacion de las maquinas de estados
	//ESTADO INICIAL, LISTA TRANSICIONES, PUNTERO ASOCIADO A LA MAQUINA
	fsm_t* player_fsm = fsm_new (WAIT_START, reproductor, &(sistema.player));
	fsm_t* columns_fsm = fsm_new (KEY_COL_1, columns, &teclado);
	fsm_t* keypad_fsm = fsm_new (KEY_WAITING, keypad, &teclado);
	fsm_t* ruedas_fsm = fsm_new (PARADO, ruedas, &(sistema.ruedas));
	fsm_t* torreta_fsm = fsm_new (WAIT_MOVE, torreta, &(sistema.torreta));

	//Valor inicial de next
	next = millis();

	//Bucle infinito que maneja las maquinas de estado del sistema
	while (1) {
		fsm_fire (player_fsm);
		fsm_fire (ruedas_fsm);
		fsm_fire (torreta_fsm);

		//Se actualizan las maquinas de estados cada CLK_MS
		next += CLK_MS;
		delay_until (next);
	}

	//Final del main, no se debe ejecutar nunca
	return 0;
}
