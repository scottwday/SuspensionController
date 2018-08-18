#include <SoftwareSerial.h>

#include "MyTypes.h"
#define PIN_LED 13

byte txBuffer[16];

SoftwareSerial softwareSerial(8, 9); //Rx, Tx

void setupComms()
{
  Serial.begin(115200);
  softwareSerial.begin(9600);
  pinMode(13, OUTPUT);
}

static byte FindNextZero(const byte* srcPtr, byte start, byte end)
{
  for (int i = start; i < end; i++)
  {
    if (srcPtr[i] == 0)
      return i;
  }
  return end;
}

int CobsEncode(const byte srcPtr[], byte srcLen, byte destPtr[])
{
  byte nextZeroInd = FindNextZero(srcPtr, 0, srcLen);

  // First byte is number of bytes to next zero or end of frame
  destPtr[0] = nextZeroInd + 1;

  for (byte i = 0; i < srcLen; i++)
  {
    if (i == nextZeroInd)
    {
      nextZeroInd = FindNextZero(srcPtr, i+1, srcLen);
      destPtr[i+1] = nextZeroInd - i;
    }
    else
    {
      destPtr[i+1] = srcPtr[i];
    }
  }

  destPtr[srcLen + 1] = 0;
  return srcLen + 2;
}

int CobsDecode(const byte srcPtr[], byte destPtr[], byte destMaxLen)
{
  byte nextZeroInd = srcPtr[0] - 1;
  srcPtr++;

  for (byte i = 0; i < destMaxLen; i++)
  {
    // Frame must always end with 0, even if malformed
    if (srcPtr[i] == 0)
      return i;
      
    if (i == nextZeroInd)
    {
      nextZeroInd = i + srcPtr[i];
      destPtr[i] = 0;
    }
    else
    {
      destPtr[i] = srcPtr[i];
    }
  }

  return 0;
}

void updateComms()
{
  static byte index = 0;
  static byte remaining = 0;
  static byte buffer[20];

  //while (Serial.available()) 
  while (softwareSerial.available()) 
  {
    byte b = softwareSerial.read();
    if (b == 0)
    {
      int len = CobsDecode(buffer, buffer, sizeof(buffer));
      if (len > 0)
      {
        // Calculate checksum
        byte sum = 0xAA;
        for (int i=0; i<len-1; i++)
          sum += buffer[i];
  
        if (sum == buffer[len-1])
        {
          onPacketReceived(buffer, len-1);
          digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        }
      }
      index = 0;
    }
    else if (index < sizeof(buffer))
    {
      digitalWrite(PIN_LED, HIGH);
      buffer[index] = b;
      index++;
    }
    else // we ran out of buffer space
    {
      index = 0;
    }
  }
}

// Appends checksum then sends cobs encoded packet
void sendPacket(uint8_t* buffer, size_t size)
{
  byte sum = 0xAA;
  for (byte i=0; i<size; i++)
      sum += buffer[i];
  buffer[size] = sum;

  int len = CobsEncode(buffer, size+1, txBuffer);

  softwareSerial.write(txBuffer, len);
}

void onPacketReceived(const uint8_t* buffer, size_t size)
{
  if (size > 20)
    return;

  switch(buffer[0])
  {
    case 0xB0:
      if ((size - 1) == sizeof(StateMsgB0))
      {
        setDisplayState((StateMsgB0*)(&buffer[1]));
        setRelaysState(((StateMsgB0*)(&buffer[1]))->relays);
      }
      break;
    case 0xB1:
      if ((size - 1) == 1)
        setRelaysState(buffer[1]);
      break; 
  }



}

