/*
 * For water pH level. Taken from http://www.dfrobot.com/wiki/index.php?title=PH_meter(SKU:_SEN0161)
*/

#ifndef Probe_pH_h //this prevents problems if someone accidently #include this library twice.
#define Probe_pH_h

#include "Arduino.h"

class Probe_pH
{
  public:
    Probe_pH(int pin);
    void begin(void);
    float getpH();
    
  private:
    int pin_;
    unsigned long int avgValue_;  //Store the average value of the sensor feedback
    int buf_[10],temp_;
};

#endif
