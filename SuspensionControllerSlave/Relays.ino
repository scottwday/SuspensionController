
const byte pinMapping[4][2] = {{11,2},{12,10},{6,7},{3,4}};
const byte relayTimeoutValue = 100;

byte relayTimeout = {0}; 
byte relaysState;

void setupRelays()
{
  pinMode(pinMapping[0][0], OUTPUT);
  pinMode(pinMapping[0][1], OUTPUT);
  pinMode(pinMapping[1][0], OUTPUT);
  pinMode(pinMapping[1][1], OUTPUT);
  pinMode(pinMapping[2][0], OUTPUT);
  pinMode(pinMapping[2][1], OUTPUT);
  pinMode(pinMapping[3][0], OUTPUT);
  pinMode(pinMapping[3][1], OUTPUT);
}

void setRelaysState(byte state)
{
  relaysState = state;
  relayTimeout = relayTimeoutValue;

  Serial.print("R");
  Serial.print((char)('0' + ((state >> 0) & 3)));
  Serial.print((char)('0' + ((state >> 2) & 3)));
  Serial.print((char)('0' + ((state >> 4) & 3)));
  Serial.print((char)('0' + ((state >> 6) & 3)));
  Serial.println("");
}

void updateRelays()
{
  // Decrement relay timeouts, if timeout then set relays off
  if (relayTimeout > 0)
    relayTimeout--;
  else
    relaysState = 0;
  
  setRelayState(0, (relaysState >> 0) & 0x03);
  setRelayState(1, (relaysState >> 2) & 0x03);
  setRelayState(2, (relaysState >> 4) & 0x03);
  setRelayState(3, (relaysState >> 6) & 0x03);
}

/// Set relay for given channel and state
void setRelayState(byte channel, byte state)
{
  if ( channel > (sizeof(pinMapping) / sizeof(pinMapping[0])) )
    return;
  
  auto upPin = pinMapping[channel][0];
  auto downPin = pinMapping[channel][1];

  if (state == 0)
  {
    digitalWrite(upPin, HIGH);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
    digitalWrite(downPin, HIGH);
  }

  if (state == 1)
  {
    digitalWrite(upPin, HIGH);
    digitalWrite(downPin, LOW);
  }

  if (state == 2)
  {
    digitalWrite(downPin, HIGH);
    digitalWrite(upPin, LOW);
  }
  
}

