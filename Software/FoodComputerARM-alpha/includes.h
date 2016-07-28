#include "mbed.h"
#include "DHT.h" //humidity and air temperature
#include "ModbusSlave232.h" //see readme, for modbus
#include "millis.h" //see readme, for modbus
#include "SoftSerial.h" //for co2 sensor
#include "TSL2561_I2C.h" //for light sensor
#include "DS1820.h" //water temp sensor

#include "pin_definitions.h" //anythign related to physical pins
#include "objects_and_variables.h" //public objs and vars