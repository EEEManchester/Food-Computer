#include "Arduino.h"
#include "Probe_EC.h"

Probe_EC::Probe_EC(int pin)
{  
  pin_ = pin; 
}

void Probe_EC::begin(void)
{
  pinMode(pin_, INPUT);
}

float Probe_EC::getEC(float temperature) //'temperature' is in  celcius (for compensation)
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf_[i]=analogRead(pin_);
    delay(10);
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
  
  avgValue_ = 0; for(int i=2;i<8;i++) {avgValue_ += buf_[i];} //take the average value of 6 center sample (don't forget to divide!)
  
  float averageVoltage = avgValue_ * 5000 / 1024 / 6; //divide by 6 because to get average value   
  float TempCoefficient_ = 1.0 + 0.0185*(temperature - 25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
  float CoefficientVoltage_ = averageVoltage / TempCoefficient_;

  if      (CoefficientVoltage_ <= 448)   {ECcurrent_ = 6.84*CoefficientVoltage_ - 64.320;}    //1ms/cm<EC<=3ms/cm
  else if (CoefficientVoltage_ <= 1457)  {ECcurrent_ = 6.98*CoefficientVoltage_ - 127.00;}    //3ms/cm<EC<=10ms/cm
  else                                   {ECcurrent_ = 5.30*CoefficientVoltage_ + 2278.0;}    //10ms/cm<EC<20ms/cm
            
  return ECcurrent_/=1000;    //convert us/cm to ms/cm
}
