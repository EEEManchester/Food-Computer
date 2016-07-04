/*
 * Water Electrical Conductivity Sensor. Taken from http://www.dfrobot.com/wiki/index.php/Analog_EC_Meter_SKU:DFR0300
*/

#ifndef Probe_EC_h //this prevents problems if someone accidently #include this library twice.
#define Probe_EC_h

#include "Arduino.h"

class Probe_EC
{
  public:
    Probe_EC(int pin);
    void begin(void);
    float getEC(float temperature);
    
  private:
    int pin_;    
    unsigned long int avgValue_;  //Store the average value of the sensor feedback
    int buf_[10], temp_;
    float ECcurrent_; 
};

#endif
