#include "pin_definitions.h"
#include "COZIR_CO2.h"
#include "Relays.h"
#include <Wire.h> //standard library
#include "DHT.h" //from https://github.com/adafruit/DHT-sensor-library
#include "OneWire.h" //from https://github.com/PaulStoffregen/OneWire
#include "DallasTemperature.h" //from https://github.com/milesburton/Arduino-Temperature-Control-Library
#include "Probe_pH.h" //from http://www.dfrobot.com/wiki/index.php/PH_meter%28SKU:_SEN0161%29 (PH meter(SKU: SEN0161))
#include "Probe_EC.h" //from http://www.dfrobot.com/wiki/index.php/Analog_EC_Meter_SKU:DFR0300 (Analog EC Meter SKU:DFR0300)
#include "Adafruit_Sensor.h" //from https://github.com/adafruit/Adafruit_Sensor
#include "Adafruit_TSL2561_U.h" //from https://github.com/adafruit/Adafruit_TSL2561
