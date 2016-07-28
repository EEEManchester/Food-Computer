//should be compatible with arduino sketch v2

//leds for debug purposes
DigitalOut led1(PA_13); 
DigitalOut led2(PA_14);
DigitalOut led3(PA_15);

DHT dht22(D2,DHT22);

DS1820 water_temp(D3);

DigitalOut R0(D4);
DigitalOut R1(D5);
DigitalOut R2(D6);
DigitalOut R3(D7);
DigitalOut R4(D8);
DigitalOut R5(D9);
DigitalOut R6(D10);
DigitalOut R7(D11);

SoftSerial CO2sensor(D13, D12);

TSL2561_I2C lux_sensor(D14, D15);

AnalogIn water_pH(A0);
AnalogIn water_EC(A1);

DigitalIn shell_switch(A2);
DigitalIn window_switch(A3);