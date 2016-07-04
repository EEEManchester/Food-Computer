#include "includes.h"

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345); //For Lux sensor
COZIR_CO2 CO2(soft_rx, soft_tx); //for COZIR CO2 sensor
Relays relays(R0, R1, R2, R3, R4, R5, R6, R7); //for relay modules
DHT dht(DHTPIN, DHTTYPE); //for air humidity/temp sensor
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature thermometer(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
Probe_pH probe_pH(pH);
Probe_EC probe_EC(EC);

float air_co2, air_temp, air_humidity, water_temp, water_ec, water_ph, light_lux, window_switch, shell_switch; //global variables
int inByte;
void getSensors();

void setup() 
{
  Serial.begin(9600); //for data transfer
  CO2.begin(); //for air CO2 content 
  relays.begin(); //simple ascii based relay control
  thermometer.begin(); //water thermometer
  probe_pH.begin(); //water pH level
  probe_EC.begin(); //water electrical conductivity
  tsl.enableAutoRange(true); tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS); //Lux sensor
  pinMode(windowSwitch, INPUT_PULLUP); pinMode(shellSwitch, INPUT_PULLUP); //safety switches
}

char c = 'A';
void loop()
{  
  if (Serial.available() > 0) //if command received
  {
    inByte = Serial.read();    
        
    if (inByte == '#') //if '#' is received, send all sensor readings
    {                         
      getSensors(); //fetch latest value

      //format and send string for GUI
      Serial.println(String(air_co2)       + "," +                 
                     String(air_humidity)  + "," +
                     String(air_temp)      + "," +
                     String(water_temp)    + "," +
                     String(water_ec)      + "," +
                     String(water_ph)      + "," +
                     String(light_lux)     + "," +
                     String(window_switch) + "," +
                     String(shell_switch));
    }
    else {relays.setRelays(inByte);} //else, control relays
  } 
}

void getSensors() //fetch all sensor data and store it in global variables
{
    thermometer.requestTemperatures(); //water temperature
    sensors_event_t event; tsl.getEvent(&event); //Get a new (Lux) sensor event
    
    air_co2 = CO2.getCO2(); //air CO2 concentration
    air_humidity = dht.readHumidity(); //air humidity
    air_temp = dht.readTemperature(); //air temperature
    water_temp = thermometer.getTempCByIndex(0); //water temperature in celcius
    water_ec = probe_EC.getEC(water_temp); //temperature compensated water electrical conductivity
    water_ph = probe_pH.getpH(); //water pH level
    light_lux = event.light; //light (Lux)
    window_switch = digitalRead(windowSwitch); //safety window switch 
    shell_switch = digitalRead(shellSwitch); //safety shell switch
}
