/*
 * Simple Library for controlling 8 relays via ascii characters
*/

#ifndef Relays_h //this prevents problems if someone accidently #include this library twice.
#define Relays_h

#include "Arduino.h"

class Relays
{
  public:
    Relays(int r0, int r1, int r2, int r3, int r4, int r5, int r6, int r7);
    void begin(void);
    void setRelays(char c);
    
  private:
    int r0_;
    int r1_;
    int r2_;
    int r3_;
    int r4_;
    int r5_;
    int r6_;
    int r7_;
};

#endif
