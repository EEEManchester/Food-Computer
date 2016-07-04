#include "Arduino.h"
#include "Relays.h"

Relays::Relays(int r0, int r1, int r2, int r3, int r4, int r5, int r6, int r7)
{  
  r0_ = r0;
  r1_ = r1;
  r2_ = r2;
  r3_ = r3;
  r4_ = r4;
  r5_ = r5;
  r6_ = r6;
  r7_ = r7;   
}

void Relays::begin(void)
{
  //set all relay pins to output
  pinMode(r0_, OUTPUT); pinMode(r1_, OUTPUT); pinMode(r2_, OUTPUT); pinMode(r3_, OUTPUT); pinMode(r4_, OUTPUT); pinMode(r5_, OUTPUT); pinMode(r6_, OUTPUT); pinMode(r7_, OUTPUT);
  //the relay is active low, so this line should turn it off
  digitalWrite(r0_, HIGH); digitalWrite(r1_, HIGH); digitalWrite(r2_, HIGH); digitalWrite(r3_, HIGH); digitalWrite(r4_, HIGH); digitalWrite(r5_, HIGH); digitalWrite(r6_, HIGH); digitalWrite(r7_, HIGH);
}

void Relays::setRelays(char c)
{
  switch (c) //simple switch for controlling relays 
  {
    case 'A': digitalWrite(r0_, LOW); break; //uses one character command for faster response
    case 'B': digitalWrite(r0_, HIGH); break;
    case 'C': digitalWrite(r1_, LOW); break;
    case 'D': digitalWrite(r1_, HIGH); break;
    case 'E': digitalWrite(r2_, LOW); break;
    case 'F': digitalWrite(r2_, HIGH); break;
    case 'G': digitalWrite(r3_, LOW); break;
    case 'H': digitalWrite(r3_, HIGH); break;
    case 'I': digitalWrite(r4_, LOW); break;
    case 'J': digitalWrite(r4_, HIGH); break;
    case 'K': digitalWrite(r5_, LOW); break;
    case 'L': digitalWrite(r5_, HIGH); break;
    case 'M': digitalWrite(r6_, LOW); break;
    case 'N': digitalWrite(r6_, HIGH); break;
    case 'O': digitalWrite(r7_, LOW); break;
    case 'P': digitalWrite(r7_, HIGH); break;
  }
}
