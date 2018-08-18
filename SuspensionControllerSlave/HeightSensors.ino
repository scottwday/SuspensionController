#include "MyTypes.h"

void setupHeightSensors()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void updateHeightSensors()
{
  // Sample height sensor values
  int a0 = analogRead(A0);
  int a1 = analogRead(A1);
  int a2 = analogRead(A2);
  int a3 = analogRead(A3);

  // Pack values into buffer
  byte buffer[12];
  buffer[0] = 0xA0; // Arbitrary message id for height samples
  buffer[1] = (a0 >> 8) & 0xFF;
  buffer[2] = (a0 >> 0) & 0xFF;
  buffer[3] = (a1 >> 8) & 0xFF;
  buffer[4] = (a1 >> 0) & 0xFF;
  buffer[5] = (a2 >> 8) & 0xFF;
  buffer[6] = (a2 >> 0) & 0xFF;
  buffer[7] = (a3 >> 8) & 0xFF;
  buffer[8] = (a3 >> 0) & 0xFF;
  buffer[9] = 0;  //checksum placeholder
  buffer[10] = 0;
  buffer[11] = 0;
  sendPacket(buffer, 9);
  
}
