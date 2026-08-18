/*
 * tests/stubs/softPwm.h — host-side stub of the wiringPi softPwm API.
 */
#ifndef STUB_SOFTPWM_H
#define STUB_SOFTPWM_H

extern int softPwmCreate(int pin, int initialValue, int pwmRange);
extern void softPwmWrite(int pin, int value);
extern void softPwmStop(int pin);

#endif /* STUB_SOFTPWM_H */
