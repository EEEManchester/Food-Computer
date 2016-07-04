#include "Arduino.h"
#include "COZIR_CO2.h"


COZIR_CO2::COZIR_CO2(int rx, int tx)
{  
  rx_ = rx;
  tx_ = tx;    
}

void COZIR_CO2::begin(void)
{
  if (mySerial_ != NULL) {delete mySerial_;}
  mySerial_ = new SoftwareSerial(rx_, tx_); //uses software uart to keep backward compatibility with uno  
  mySerial_ -> begin(9600); 
  delay(100); //allow some time for initiation
  mySerial_ -> println("K 2\r"); //set the sensor into polling mode (need to sent twice, not sure why)
  dummy  = mySerial_ -> readStringUntil('\n');
  mySerial_ -> println("K 2\r"); //Mode {0,1,2} is {command,streaming,polling}
  dummy  = mySerial_ -> readStringUntil('\n');
}

long COZIR_CO2::getCO2()
{
  mySerial_ -> println("Z\r"); //request CO2 reading in ppm
  dummy  = mySerial_ -> readStringUntil('\n'); //the sensor sends something like " Z 00400\r\n"
  dummy = getValue(dummy, ' ', 2); //to extract integer part of the string
  return dummy.toInt(); //CO2     
}

String COZIR_CO2::getValue(String data, char separator, int index) //found here: http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string/1237
{
  int found = 0;
  int strIndex[] = {0, -1  };
  int maxIndex = data.length()-1;
  
  for(int i=0; i<=maxIndex && found<=index; i++)
  {
    if(data.charAt(i)==separator || i==maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
