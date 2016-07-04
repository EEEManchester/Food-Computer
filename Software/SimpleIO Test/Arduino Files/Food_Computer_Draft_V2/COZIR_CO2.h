/*
 * Simple Library for COZIR CO2 sensors: http://www.co2meter.com/products/cozir-0-2-co2-sensor
*/

#ifndef COZIR_CO2_h //this prevents problems if someone accidently #include this library twice.
#define COZIR_CO2_h

#include "Arduino.h"
#include <SoftwareSerial.h> //uses software uart to keep backward compatibility with uno

class COZIR_CO2
{
  public:
    COZIR_CO2(int rx, int tx);
    void begin(void);
    long getCO2(); //to get CO2 concentration in PPM
    
  private:
    SoftwareSerial *mySerial_;
    String dummy;
    int rx_;
    int tx_;
    String getValue(String data, char separator, int index); //for String objects (used to extract integer from string)
};

#endif
