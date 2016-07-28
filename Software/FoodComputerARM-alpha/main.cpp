#include "includes.h"

float get_EC(float temperature);
float get_pH();

void update_modbus() //check for request and update relays
{          
        led1 = !led1;  //for debug 
        if (mbs.update(regs, MB_REGS))
        {led3 = !led3;}

        R0 = (regs[MB_9] & (0x0001<<0)); 
        R1 = (regs[MB_9] & (0x0001<<1)); 
        R2 = (regs[MB_9] & (0x0001<<2)); 
        R3 = (regs[MB_9] & (0x0001<<3)); 
        R4 = (regs[MB_9] & (0x0001<<4)); 
        R5 = (regs[MB_9] & (0x0001<<5)); 
        R6 = (regs[MB_9] & (0x0001<<6)); 
        R7 = (regs[MB_9] & (0x0001<<7));
}

int main()
{
    regs[MB_9] = 0xFFFF; //force all relay off       
    
    startMillis(); // milliseconds (arduino like)
    mbs.configure(SLAVE, BAUD, PARITY);
    
    CO2sensor.baud(9600);
    CO2sensor.printf("\nK 2\r\n"); //set the sensor into polling mode
    CO2sensor.scanf(" %[^\n]", &co2string); //dummy - blocking call
            
    lux_sensor.enablePower();
    window_switch.mode(PullUp);
    shell_switch.mode(PullUp);
    water_temp.set_configuration_bits(10); //9bit of resolution so that it is faster
        
    modbus_updater.attach(&update_modbus, 0.1); //visit modbus every 100ms

    while (true) //main thread (updates sensors' status)
    {                                
        led2 = !led2; //for debug        
        
        dht22.readData();
        water_temp.convert_temperature(DS1820::all_devices);         //Start temperature conversion, wait until ready
        water_temperature = water_temp.temperature('c');
        
        CO2sensor.printf("Z\r\n"); //request CO2 reading
        //see: http://stackoverflow.com/questions/16447759/scanf-inside-while-loop-working-only-one-time
        CO2sensor.scanf(" %[^\n]", &co2string); //store it in a buffer (blocking call)                                      
        sscanf(co2string, "%*c %d", &CO2_PPM);   
                        
        regs[MB_0] = CO2_PPM; //air_co2
        regs[MB_1]=  (int)100.0*dht22.ReadHumidity(); //air_humidity
        regs[MB_2]=  (int)100.0*dht22.ReadTemperature(CELCIUS); //air_temp
        regs[MB_3] = (int)100.0*water_temperature;//water_temp in celcius
        regs[MB_4] = (int)100.0*get_EC(water_temperature); //water_ec 
        regs[MB_5] = (int)100.0*get_pH(); //water_ph
        regs[MB_6] = lux_sensor.getLux(); //light_lux
        regs[MB_7]=  window_switch.read(); //window_switch
        regs[MB_8] = shell_switch.read(); //shell_switch                      
    }
}

//For water pH level. Taken from http://www.dfrobot.com/wiki/index.php?title=PH_meter(SKU:_SEN0161)
float get_pH()
{
  float avgValue_;  //Store the average value of the sensor feedback
  float buf_[10],temp_;
  float phValue_;
    
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf_[i]=water_pH.read();
    wait_ms(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf_[i]>buf_[j])
      {
        temp_=buf_[i];
        buf_[i]=buf_[j];
        buf_[j]=temp_;
      }
    }
  }
  avgValue_ = 0.0f;
  for(int i=2;i<8;i++)       //take the average value of 6 center sample
    avgValue_ += buf_[i];
    
  phValue_= avgValue_*5.0f/6.0f; //convert the analog into millivolt
  return 3.5f*phValue_;  
}


//Water Electrical Conductivity Sensor. Taken from http://www.dfrobot.com/wiki/index.php/Analog_EC_Meter_SKU:DFR0300
float get_EC(float temperature)
{  
  float avgValue_;  //Store the average value of the sensor feedback
  float buf_[10], temp_;
  float ECcurrent_; 
    
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf_[i]=water_EC.read();
    wait_ms(10);
  }
  
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf_[i]>buf_[j])
      {
        temp_=buf_[i];
        buf_[i]=buf_[j];
        buf_[j]=temp_;
      }
    }
  }
  
  avgValue_ = 0.0f; for(int i=2;i<8;i++) {avgValue_ += buf_[i];} //take the average value of 6 center sample (don't forget to divide!)
  
  float averageVoltage = avgValue_ * 5000.0f / 6.0f; //divide by 6 because to get average value   
  float TempCoefficient_ = 1.0f + 0.0185f*(temperature - 25.0f);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
  float CoefficientVoltage_ = averageVoltage / TempCoefficient_;

  if      (CoefficientVoltage_ <= 448.0f)   {ECcurrent_ = 6.84f*CoefficientVoltage_ - 64.320f;}    //1ms/cm<EC<=3ms/cm
  else if (CoefficientVoltage_ <= 1457.0f)  {ECcurrent_ = 6.98f*CoefficientVoltage_ - 127.00f;}    //3ms/cm<EC<=10ms/cm
  else                                     {ECcurrent_ = 5.30f*CoefficientVoltage_ + 2278.0f;}    //10ms/cm<EC<20ms/cm
            
  return ECcurrent_/=1000.0f;    //convert us/cm to ms/cm
}