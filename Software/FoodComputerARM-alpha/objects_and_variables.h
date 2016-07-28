//for modbus
const unsigned char SLAVE = 1;
const long BAUD = 9600;            
const unsigned PARITY = 'n';
ModbusSlave232 mbs; // Create new mbs instance

Ticker modbus_updater;

// Slave registers
enum {        
  MB_0,   // Register 0 air_co2
  MB_1,   // Register 1 air_humidity
  MB_2,   // Register 2 air_temp
  MB_3,   // Register 3 water_temp 
  MB_4,   // Register 4 water_ec 
  MB_5,   // Register 5 water_ph
  MB_6,   // Register 6 light_lux
  MB_7,   // Register 7 window_switch
  MB_8,   // Register 8 shell_switch   
  MB_9,   // Register 9 relays    
  MB_REGS // Dummy register. using 0 offset to keep size of array
};

int regs[MB_REGS];

char co2string[32];
int CO2_PPM;
float water_temperature;