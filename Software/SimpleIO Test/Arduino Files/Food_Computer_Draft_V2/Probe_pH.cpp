#include "Arduino.h"
#include "Probe_pH.h"

Probe_pH::Probe_pH(int pin)
{  
  pin_ = pin; 
}

void Probe_pH::begin(void)
{
  pinMode(pin_, INPUT);
}

float Probe_pH::getpH()
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
  avgValue_ = 0;
  for(int i=2;i<8;i++)       //take the average value of 6 center sample
    avgValue_ += buf_[i];
    
  float phValue_= (float)avgValue_*5.0/1024/6; //convert the analog into millivolt
  return 3.5*phValue_;  
}
