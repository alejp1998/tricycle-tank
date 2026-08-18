/*
 * tests/stubs/softTone.h — host-side stub of the wiringPi softTone API.
 */
#ifndef STUB_SOFTTONE_H
#define STUB_SOFTTONE_H

extern int softToneCreate(int pin);
extern void softToneWrite(int pin, int freq);
extern void softToneStop(int pin);

#endif /* STUB_SOFTTONE_H */
