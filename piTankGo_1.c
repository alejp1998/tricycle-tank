
#include "piTankGo_1.h"

//Arrays de frecuencias y duraciones para el player
//Efectos libres
int frecuenciaDespacito[160] = {0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,0,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318,659,659,659,659,659,659,659,659,554,587,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318};
int tiempoDespacito[160] = {1200,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,800,300,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,450,1800,150,150,150,150,300,150,300,150,150,150,300,150,300,450,450,300,150,150,225,75,150,150,300,450,800,150,150,300,150,150,300,450,150,150,150,150,150,150,150,150,300,300,150,150,150,150,150,150,450,150,150,150,300,150,300,450,450,300,150,150,150,300,150,300,450,800,150,150,300,150,150,300,450};
int frecuenciaGOT[518] = {1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,1175,0,1397,0,932,0,1244,0,1175,0,1397,0,932,0,1244,0,1175,0,1046,0,831,0,698,0,523,0,349,0,784,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,587,0,622,0,587,0,523,0,587,0,784,0,880,0,932,0,1046,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1046,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,880,0,784,0,932,0,1244,0,0,1397,0,0,932,0,0,1175,0,0,1244,0,0,1175,0,0,932,0,0,1046,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,0,0,0,2794,0,0,0,0,3136,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,1397,0,0,0,2350,0,0,0,2489,0,0,0,2350,0,0,0,0,2093,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865};
int tiempoGOT[518] = {900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1400,1400,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,116,267,28,267,28,267,28,900,89,900,89,1400,89,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,69,7,69,7,69,7,69,7,267,28,400,45,133,13,267,28,267,28,267,28,300,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,900,89,900,133,13,150,133,13,150,1200,1800,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,267,28,1200,400,133,13,133,13,150,900,89,900,900,89,900,600,59,600,267,28,300,600,59,600,267,28,300,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,133,13,267,28,267,28,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,900,89,900,900,900,900,89,900,900,900,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,600,212,133,13,150,150,267,28,300,300,400,45,450,450,133,13,150,150,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400,116,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400};
int frecuenciaTetris[55] = {1319,988,1047,1175,1047,988,880,880,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,1175,1397,1760,1568,1397,1319,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,659,523,587,494,523,440,415,659,523,587,494,523,659,880,831};
int tiempoTetris[55] = {450,225,225,450,225,225,450,225,225,450,225,225,450,225,225,450,450,450,450,450,675,450,225,450,225,225,675,225,450,225,225,450,225,225,450,450,450,450,450,450,900,900,900,900,900,900,1800,900,900,900,900,450,450,900,1800};
int frecuenciaStarwars[59] = {523,0,523,0,523,0,698,0,1046,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,1760,0,0,784,0,523,0,0,523,0,0,523,0};
int tiempoStarwars[59] = {134,134,134,134,134,134,536,134,536,134,134,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,429,357,1071,268,67,67,268,67,67,67,67,67};
//Efectos torreta
int frecuenciasDisparo[16] = {2500,2400,2300,2200,2100,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000};
int tiemposDisparo[16] = {75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75};
int frecuenciasImpacto[55] = {1319,988,1047,1175,1047,988,880,880,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,1175,1397,1760,1568,1397,1319,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,659,523,587,494,523,440,415,659,523,587,494,523,659,880,831};
int tiemposImpacto[55] = {450,225,225,450,225,225,450,225,225,450,225,225,450,225,225,450,450,450,450,450,675,450,225,450,225,225,675,225,450,225,225,450,225,225,450,450,450,450,450,450,900,900,900,900,900,900,1800,900,900,900,900,450,450,900,1800};

//int frecuenciasImpacto[32] = {97,109,79,121,80,127,123,75,119,96,71,101,98,113,92,70,114,75,86,103,126,118,128,77,114,119,72};
//int tiemposImpacto[32] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};

//Variables para los flags
int flags_juego = 0;
int flags_player = 0;
int nsongant = 0;

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

	//Inicializar wiringPi
	if (wiringPiSetupGpio () < 0) {
			piLock (STD_IO_BUFFER_KEY);
			printf ("Unable to setup wiringPi\n");
			piUnlock (STD_IO_BUFFER_KEY);

			return -1;
	}
	return result;
}

// int InicializaSistema (TipoSistema *p_sistema): procedimiento de inicializacion del sistema.
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// la torreta, los efectos, etc.
int InicializaSistema (TipoSistema *p_sistema) { //Inicializamos el sistema y sus atributos(player...)
	int result = 0;
	//Inicializamos los efectos dentro del objeto player
	InicializaEfecto (&p_sistema->player.efecto_disparo, "DISPARO" , frecuenciasDisparo, tiemposDisparo, 16);
	InicializaEfecto (&p_sistema->player.efecto_impacto, "IMPACTO" , frecuenciasImpacto, tiemposImpacto, 55);

	//Inicializamos el objeto player p_sistema->player es el objeto player bajo el puntero al sistema
	InicializaPlayer(&(p_sistema->player));

	//Inicializamos el objeto torreta del sistema
	InicializaTorreta (&p_sistema->torreta);

	//Inicializamos el objeto ruedas del sistema
	InicializaRuedas (&p_sistema->ruedas);

	//Inicializamos mando xbox360
	InicializaXbox360 (&p_sistema->mando);

	return result;
}

//Funcion de retardo que se encarga de sincronizar el automata
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

//Conjunto de acciones a ejecutar
int main ()
{
	//Declaramos el sistema a usar
	TipoSistema sistema;
	unsigned int next;

	//Configuracion e inicializacion del sistema
	ConfiguraSistema (&sistema);

	//Inicializamos todos los componentes del sistema
	InicializaSistema (&sistema);

	//TRANSICIONES MAQUINAS DE ESTADOS
	//ESTADO DE PARTIDA , COMPROBACION DE FLAG, ESTADO FINAL(si se cumple comprobacion), FUNCION A EJECUTAR

	//Control teclas pulsadas
	fsm_trans_t xbox360[] = {
				{ ESPERAPULS, CompruebaPulsada, ESPERAPULS, Pulsada},
				{-1, NULL, -1, NULL },
	};

	//Reproductor de efectos
	fsm_trans_t reproductor[] = {
			{ WAIT_START, CompruebaStartDisparo, WAIT_NEXT, InicializaPlayDisparo },
			{ WAIT_START, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
			{ WAIT_START, CompruebaStartEfecto, WAIT_NEXT, InicializaPlayEfecto },
			{ WAIT_NEXT, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
			{ WAIT_NEXT, CompruebaNotaTimeout, WAIT_END, ActualizaPlayer },
			{ WAIT_END, CompruebaFinalEfecto, WAIT_START, FinalEfecto },
			{ WAIT_END, CompruebaNuevaNota, WAIT_NEXT, ComienzaNuevaNota},
			{-1, NULL, -1, NULL },
	};

	//Control ruedas
	fsm_trans_t ruedas[] = {
			{PARADO, CompruebaMovimiento, MOVIMIENTO, Movimiento },
			{MOVIMIENTO, CompruebaMovimiento, MOVIMIENTO, Movimiento },
			{MOVIMIENTO, CompruebaParado, PARADO, Parado },
			{-1,NULL,-1,NULL},
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
	fsm_t* xbox360_fsm = fsm_new (ESPERAPULS, xbox360, &(sistema.mando));
	fsm_t* player_fsm = fsm_new (WAIT_START, reproductor, &(sistema.player));
	fsm_t* ruedas_fsm = fsm_new (PARADO, ruedas, &(sistema.ruedas));
	fsm_t* torreta_fsm = fsm_new (WAIT_START, torreta, &(sistema.torreta));

	//Valor inicial de next
	next = millis();

	//Print inicial, espera a que pulsemos start para comenzar juego
	piLock(STD_IO_BUFFER_KEY);
	printf("SYSTEM INITIALIZATION COMPLETE!\n");
	printf("WELCOME! PRESS START\n");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);

	//Bucle infinito que maneja las maquinas de estado del sistema
	while (1) {

		fsm_fire (xbox360_fsm);
		//No ejecuta el player ni las ruedas hasta que no comience
		if((flags_juego & FLAG_SYSTEM_START) != 0){
			fsm_fire (player_fsm);
			fsm_fire (ruedas_fsm);
		}
		fsm_fire (torreta_fsm);

		//Seleccion de efecto personalizado con cruceta, no inicializamos dos veces seguidas el mismo
		if(nsong == 1 && nsongant != nsong){
			InicializaEfecto (&(sistema.player.efecto_libre), "DESPACITO" , frecuenciaDespacito, tiempoDespacito, 160);
			nsongant = 1;//Numero de cancion anterior
		}else if(nsong == 2 && nsongant != nsong){
			InicializaEfecto (&(sistema.player.efecto_libre), "GOT" , frecuenciaGOT, tiempoGOT, 518);
			nsongant = 2;
		}else if(nsong == 3 && nsongant != nsong){
			InicializaEfecto (&(sistema.player.efecto_libre), "TETRIS" , frecuenciaTetris, tiempoTetris, 55);
			nsongant = 3;
		}else if(nsong == 4 && nsongant != nsong){
			InicializaEfecto (&(sistema.player.efecto_libre), "STAR WARS" , frecuenciaStarwars, tiempoStarwars, 59);
			nsongant = 4;
		}

		//Imprimir estado del tanque cuando empieze el juego (si no estamos reproduciendo un efecto)
		if((flags_player & FLAG_PLAYER_ACTIVO)==0 && (flags_juego & FLAG_SYSTEM_START) != 0){
			piLock(STD_IO_BUFFER_KEY);
			printf("MOV:     RUEDA1 ( %d ) RUEDA2 ( %d ) \n", sistema.ruedas.rueda1 , sistema.ruedas.rueda2);
			printf("TORRETA: SERVOX ( %d ) SERVOY ( %d ) \n", sistema.torreta.servo_x.posicion , sistema.torreta.servo_y.posicion);
			printf("ESTADISTICAS: IMPACTOS LOGRADOS: ( %d ) BALAS RESTANTES: ( %d ) \n", sistema.torreta.impactos, disparos);
			printf("EFECTO: ( %s )\n", sistema.player.efecto_libre.nombre);
			
			//Cuando nos quedemos sin balas mostrar mensaje de recarga
			if(disparos<=0){
				printf("TE HAS QUEDADO SIN BALAS!!! PULSA X PARA RECARGAR MUNICION \n");
			}

			//Cuando acertemos 10 impactos, finalizar el juego
			if(sistema.torreta.impactos>=10){
				printf("WINNER \n HAS ACERTADO 10 IMPACTOS!!!\n OTRA PARTIDA?\n");
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_SYSTEM_END;
				piUnlock(SYSTEM_FLAGS_KEY);
			}

			//Retornos de carro para separar estados del robot en consola
			printf("\n\n\n");
			fflush(stdout);
			piUnlock(STD_IO_BUFFER_KEY);
		}

		//Se actualizan las maquinas de estados cada CLK_MS
		next += CLK_MS;
		delay_until (next);
	}

	//Final del main, no se debe ejecutar nunca
	return 0;
}
