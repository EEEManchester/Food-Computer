//pin and sensors definitions (should be compatible with UNO/MEGA)
#define DHTTYPE DHT22

#define DHTPIN 2 //air humidity and temperature

#define DS18B20_pin 3 //water thermometer 

//Relay pins
#define R0 4
#define R1 5
#define R2 6
#define R3 7
#define R4 8
#define R5 9
#define R6 10
#define R7 11

//software UART
#define soft_rx 12 //for COZIR-AV-1 CO2 sensor
#define soft_tx 13

//water pH and electrical conductivity sensors
#define pH A0
#define EC A1

#define shellSwitch A2
#define windowSwitch A3

#define ONE_WIRE_BUS DS18B20_pin
