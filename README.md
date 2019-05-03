# piTankGo_1
Proyecto SDG2

Este proyecto se encarga de manejar el control de una torreta y dos ruedas, ademas de la reproduccion de efectos, mediante los
pines de la raspberry pi(GPIO). 

------------------------------------------------------------------------------------------------------------------------------

PINES USADOS

PINES 18, 19 : HARDWARE PWM -> CONTROL RUEDAS
PINES 17, 27 : SOFTWARE PWM -> CONTROL SERVOS
PIN 23       : SOFTTONE -> REPRODUCCION EFECTOS
PIN 9        : HIGH     -> DISPARO
PIN 11       : DET FLANCO SUBIDA -> IMPACTOS

------------------------------------------------------------------------------------------------------------------------------

CONTROLES MANDO XBOX360

JOYSTICK IZQUIERDO -> MOVIMIENTO RUEDAS
JOYSTICK DERECHO   -> MOVIMIENTO SERVOS
CRUCETA DIGITAL    -> SELECCION DE EFECTO

TRIGGER DERECHO    -> DISPARAR

A -> REPRODUCE EFECTO LIBRE
B -> FINALIZA EFECTO EN REPRODUCCION
X -> RECARGAR BALAS (MAXIMO 10)
START -> FINALIZAR JUEGO

------------------------------------------------------------------------------------------------------------------------------

INSTRUCCIONES 

1. Comenzar escritura de mando en segundo plano: 
  
  Entrar en carpeta Xbox: cd Xbox
  Ejecutar: sudo python sample.py
 
2. Comenzar juego:

  Entrar en carpeta piTankEx: cd piTankEx
  Ejecutar: sudo ./piTankGo_1
  
3. Finalizar juego
  
  Pulsando la tecla START en mando Xbox360, o acertando 10 impactos.

