/*
 * tests/stubs/wiringPi.h — host-side stub of the wiringPi API.
 *
 * Lets the project be syntax-checked (and the FSM tests run) on any
 * machine without a Raspberry Pi. The stub functions return sane
 * defaults and are only used by `make check` / `make test`.
 */
#ifndef STUB_WIRINGPI_H
#define STUB_WIRINGPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Pin modes */
#define INPUT 0
#define OUTPUT 1
#define PWM_OUTPUT 2
#define SOFT_PWM_OUTPUT 3
#define SOFT_TONE_OUTPUT 4
#define PWM_TONE_OUTPUT 5

/* Digital levels */
#define LOW 0
#define HIGH 1

/* Pull-up / pull-down resistors */
#define PUD_OFF 0
#define PUD_DOWN 1
#define PUD_UP 2

/* Interrupt edge types */
#define INT_EDGE_SETUP 0
#define INT_EDGE_FALLING 1
#define INT_EDGE_RISING 2
#define INT_EDGE_BOTH 3

extern int wiringPiSetupGpio(void);
extern void pinMode(int pin, int mode);
extern void pullUpDnControl(int pin, int pud);
extern void digitalWrite(int pin, int value);
extern int digitalRead(int pin);
extern unsigned int millis(void);
extern void delay(unsigned int howLong);
extern int wiringPiISR(int pin, int mode, void (*function)(void));
extern int piLock(int key);
extern int piUnlock(int key);
extern int piHiPri(int pri);

/* Hardware PWM (used by ruedas.c) */
#define PWM_MODE_MS 0
#define PWM_MODE_BAL 1
extern void pwmSetMode(int mode);
extern void pwmSetClock(int divisor);
extern void pwmSetRange(unsigned int range);
extern void pwmWrite(int pin, int value);

#ifdef __cplusplus
}
#endif

#endif /* STUB_WIRINGPI_H */
