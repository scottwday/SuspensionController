
#include <SoftwareSerial.h>

SoftwareSerial softwareSerial(8, 9); //Rx, Tx

void setupComms()
{
  Serial.begin(115200);
  Serial.println("hi");

  softwareSerial.begin(9600);
}

void updateComms()
{
  static byte index = 0;
  static byte buffer[16];
  
  while (softwareSerial.available()) 
  {
    byte b = softwareSerial.read();
  
    if (index < sizeof(buffer))
      {
        buffer[index] = b;
        index++;
      }
    
    if (b == 0)
    {    
      onPacketReceived(buffer, index);
      index = 0;
    }
  }
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
}

void onPacketReceived(const uint8_t* buffer, size_t size)
{
  byte decoded[16];
  int len = CobsDecode(buffer, decoded, sizeof(decoded) );

  if (decoded[0] == 0xA0)
  {
    if (len == 10)
    {
      // Calculate Keksum
      byte checksum = 0xAA;
      for (byte i=0; i<len-1; i++)
        checksum += decoded[i];

      if (checksum == decoded[len-1])
      {
        for (int i=0; i<4; i++)
        {
          h_raw[i] = ((int)decoded[(i*2) + 1] << 8) | (decoded[(i*2) + 2] << 0);
          //Serial.print(h_raw[i]);
          //Serial.print("  ");
        }
        //Serial.println("");
      }
    }
  }
}

void sendRelayState(byte* relays)
{
  // Pack values into buffer
  byte buffer[3];
  buffer[0] = 0xB0; // Message id to set relays
  
  buffer[1] = (relays[0] << 0) |
              (relays[1] << 2) |
              (relays[2] << 4) |
              (relays[3] << 7);
  
  // Calculate Keksum
  byte checksum = 0xAA;
  for (byte i=0; i<3; i++)
    checksum += buffer[i];
  buffer[3] = checksum;

  // Send
  byte encoded[16];
  int len = CobsEncode(buffer,sizeof(buffer), encoded);
  softwareSerial.write(encoded, len);
}

void sendDispState(byte mode, byte deadband, short* filteredHeights, short* setpointHeights, byte* relays)
{
  // Pack values into buffer
  byte buffer[13];
  buffer[0] = 0xB0; // Message id

  StateMsgB0* p = (StateMsgB0*)(&buffer[1]);
  p->mode = (byte)mode;
  p->deadband = (byte)deadband / 10;
  for (byte i=0; i<4; i++)
  {
    p->filteredHeights[i] = filteredHeights[i] / 10;
    p->setpointHeights[i] = setpointHeights[i] / 10;
  }

  p->relays = ((relays[0] & 0x03) << 0) |
              ((relays[1] & 0x03) << 2) |
              ((relays[2] & 0x03) << 4) |
              ((relays[3] & 0x03) << 6);

  // Calculate Keksum
  byte checksum = 0xAA;
  for (byte i=0; i<sizeof(buffer)-1; i++)
    checksum += buffer[i];
  buffer[sizeof(buffer)-1] = checksum;

  // Send
  byte encoded[16];
  int len = CobsEncode(buffer,sizeof(buffer), encoded);
  softwareSerial.write(encoded, len);

}

