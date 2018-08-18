#define POTS_ADDR0 10
#define POTS_ADDR1 11
#define POTS_ADDR2 12

#define POTS_IN0 A0
#define POTS_IN1 A1


short pots[16];

const byte potMap[] = { 4, 2, 6, 7, 5, 3, 0, 1, 14, 15, 13, 11, 8, 9, 10, 12 };

void setupPots()
{ 
  pinMode(POTS_ADDR0, OUTPUT);
  pinMode(POTS_ADDR1, OUTPUT);
  pinMode(POTS_ADDR2, OUTPUT);
  pinMode(POTS_IN0, INPUT);
  pinMode(POTS_IN1, INPUT);
}

void readPots()
{
  for (byte i=0; i<8; i++)
  {
    digitalWrite(POTS_ADDR0, (i & 1) ? 1 : 0);
    digitalWrite(POTS_ADDR1, (i & 2) ? 1 : 0);
    digitalWrite(POTS_ADDR2, (i & 4) ? 1 : 0);
    delay(1);
    short val0 = POT_MID - (short)analogRead(A0);
    short val1 = POT_MID - (short)analogRead(A1);
    pots[i] = val0;
    pots[i+8] = val1;
    delay(1);
  }
}

short getPot(byte i)
{
  if (i >= sizeof(potMap))
    return 0;
  
  return pots[potMap[i]];
}


