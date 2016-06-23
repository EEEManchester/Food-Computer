/*
 * Simple sketch to test all communication, sensors and actuators on arduino uno/mega.
 * Not optimised yet.
*/

#include <SoftwareSerial.h> //uses software uart to keep backward compatibility with uno
#include <DHT.h> //https://github.com/adafruit/DHT-sensor-library
#include <Wire.h> //standard i2c
#include <Digital_Light_TSL2561.h> //https://github.com/Seeed-Studio/Grove_Digital_Light_Sensor

//pin definitions
#define DHTTYPE DHT22

#define window_switch A3
#define shell_switch A2
#define EC A1
#define pH A0

#define DHTPIN 2

#define DC1 3
#define DC2 4
#define DC3 5
#define DC4 6

#define AC1 7
#define AC2 8
#define AC3 9
#define AC4 10

#define soft_rx 11 //for COZIR-AV-1 CO2 sensor
#define soft_tx 12

#define water_temp 13 //not featured yet (waiting for shipment) DS18B20 


int inByte = 0, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, j = 0; ;
String sendString,A,B,C,D,E,F,G,H,I,J;
float airHumidity, airTemperature;

SoftwareSerial mySerial(soft_rx, soft_tx); // RX, TX
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  Wire.begin(); //for water temp
  Serial.begin(9600); //for GUI
  mySerial.begin(9600);  //for CO2 sensor
  dht.begin(); //for humidiry sensor
  TSL2561.init(); //for light sensor

  pinMode(EC,INPUT); //water Electrical Conductivity
  pinMode(pH,INPUT); //water pH
  pinMode(DHTPIN,INPUT); //air humidity

  pinMode(AC1,OUTPUT); pinMode(AC2,OUTPUT); pinMode(AC3,OUTPUT); pinMode(AC4,OUTPUT); //ac relays  
  pinMode(DC1,OUTPUT); pinMode(DC2,OUTPUT); pinMode(DC3,OUTPUT); pinMode(DC4,OUTPUT); //dc relays

  pinMode(window_switch,INPUT_PULLUP); //for window switch
  pinMode(shell_switch,INPUT_PULLUP); //for shell switch  

  digitalWrite(AC1,HIGH);digitalWrite(AC2,HIGH);digitalWrite(AC3,HIGH);digitalWrite(AC4,HIGH); //the relay module is active low, this line will turn it off upon power up
  digitalWrite(DC1,HIGH);digitalWrite(DC2,HIGH);digitalWrite(DC3,HIGH);digitalWrite(DC4,HIGH);
    
  mySerial.print("K 2\r\n"); //set the CO2 sensor into polling mode (the command "K 2\r\n" need to be sent twice, idk why.. :/ )
  String dummy  = mySerial.readStringUntil('\n'); 

  mySerial.print("K 2\r\n"); //set the CO2 sensor into polling mode (the command "K 2\r\n" need to be sent twice, idk why.. :/ )
  dummy  = mySerial.readStringUntil('\n');
}


void loop()
{  
  if (Serial.available() > 0) 
  {  
    inByte = Serial.read();
    switch (inByte) //simple switch for controlling relays 
    {
      case 'A': digitalWrite(AC1, LOW); break; //uses one character command for faster response
      case 'B': digitalWrite(AC1, HIGH); break;
      case 'C': digitalWrite(AC2, LOW); break;
      case 'D': digitalWrite(AC2, HIGH); break;
      case 'E': digitalWrite(AC3, LOW); break;
      case 'F': digitalWrite(AC3, HIGH); break;
      case 'G': digitalWrite(AC4, LOW); break;
      case 'H': digitalWrite(AC4, HIGH); break;
      case 'I': digitalWrite(DC1, LOW); break;
      case 'J': digitalWrite(DC1, HIGH); break;
      case 'K': digitalWrite(DC2, LOW); break;
      case 'L': digitalWrite(DC2, HIGH); break;
      case 'M': digitalWrite(DC3, LOW); break;
      case 'N': digitalWrite(DC3, HIGH); break;
      case 'O': digitalWrite(DC4, LOW); break;
      case 'P': digitalWrite(DC4, HIGH); break;
    }
        
    if (inByte == '#') //if '#' is received, send all sensor readings
    {                   
      a = digitalRead(window_switch); //'1' means the switched is open. (uses pullup resistor and the switch as the pull down) 
      b = digitalRead(shell_switch); 
      c = 0; //water temp (uses one wire library) waiting for the sensor to arrive
      d = analogRead(pH); //ph (needs post processing)
      e = analogRead(EC); //ec (needs post processing)
      
      float lux_ = TSL2561.readVisibleLux(), //get raw lux data from the sensor (might overflow?)
      lux = lux_*0.78, par = lux_*0.02*0.86;     //not done yet, not too sure how MIT calculate the lux and par from the sensor raw data
      f = int(lux); //lux
      g = int(par); //par
      
      h = dht.readTemperature(); //air temp
      i = dht.readHumidity();  //air humidity

      mySerial.print("Z\r\n");  //just get the CO2 data (tbc: humidity and temperature index)
      String first  = mySerial.readStringUntil('\n'); //the sensor sends something like " Z 00400\r\n"
      String second = getValue(first, ' ', 2); //to extract integer part of the string
      j = second.toInt(); //CO2    

      //format string for GUI
      A = String(a); B = String(b); C = String(c); D = String(d); E = String(e); F = String(f); G = String(g); H = String(h); I = String(i); J = String(j); 
      sendString = A + ',' + B + ',' + C + ',' + D + ',' + E + ',' + F + ',' + G + ',' + H + ',' + I + ',' + J; 
      Serial.println(sendString);
    }
  }
}

String getValue(String data, char separator, int index) //found here: http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string/1237
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
