/*
 *  Suspension Controller
 *  
 *  Mode 1 Rear Setpoint
 *  Mode 1 Front Setpoint
 *  Mode 2 Rear Setpoint
 *  Mode 2 Front Setpoint
 *  Mode 3 Rear Setpoint
 *  Mode 3 Front Setpoint
 *  
 *  Rear Left Trim
 *  Rear Right Trim
 *  Front Left Trim
 *  Front Right Trim
 *  
 *  Deadband
 *  
 */
 
#include "MyTypes.h"

#define SW0 5
#define SW1 4
#define LED_RED 3
#define LED_GRN 2
#define nmax 1024

uint8_t mode_switch;
short h_raw[4] = { 50, 50, 50, 50 };
byte r[4] = {0};

void readModeSwitch()
{
  mode_switch = (digitalRead(SW0) | (digitalRead(SW1) << 1)) ^ 0x03;
}

void setLed(uint8_t led)
{
  digitalWrite( LED_GRN, (led & 1) ? 0 : 1 );
  digitalWrite( LED_RED, (led & 2) ? 0 : 1 );
}

void setup() {

  setupComms();

  setupPots();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GRN, OUTPUT);
  
  pinMode(SW0, INPUT);
  pinMode(SW1, INPUT);
  digitalWrite(SW0, HIGH);
  digitalWrite(SW1, HIGH);

  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  
  digitalWrite(A6, LOW);
  digitalWrite(A7, HIGH);
}

void loop() {

  readPots();

  readModeSwitch();
  
  short scale = (getPot(POT_SCALE) + POT_MID) * 2;
  
  short h[4];
  h[0] = (short)( (long)(h_raw[0] + getPot(POT_TRIM_LB)) * scale / nmax);
  h[1] = (short)( (long)(h_raw[1] + getPot(POT_TRIM_RB)) * scale / nmax);
  h[2] = (short)( (long)(h_raw[2] + getPot(POT_TRIM_LF)) * scale / nmax);
  h[3] = (short)( (long)(h_raw[3] + getPot(POT_TRIM_RF)) * scale / nmax);
  for (auto& k: h)
  {
    if (k > nmax) k = nmax;
    if (k < 0) k = 0;
  }

  //deadband
  short d = (getPot(POT_DEADBAND) + POT_MID) / 2;
  if (d > POT_MID) d = POT_MID;
  if (d < 0) d = 0;

  // Read setpoints
  short s0 = 0;
  short s1 = 0;
  switch (mode_switch)
  {
    case 1:
      s0 = getPot(POT_SETPOINT1_F) + POT_MID;
      s1 = getPot(POT_SETPOINT1_R) + POT_MID;
      break;
    case 2:
      s0 = getPot(POT_SETPOINT2_F) + POT_MID;
      s1 = getPot(POT_SETPOINT2_R) + POT_MID;
      break;
    case 3:
      s0 = getPot(POT_SETPOINT3_F) + POT_MID;
      s1 = getPot(POT_SETPOINT3_R) + POT_MID;
      break;
  }

  short s[4];
  s[0] = (s0);
  s[1] = (s0);
  s[2] = (s1);
  s[3] = (s1);

  byte relays_on = 0;
  // Calculate relay states, r
  for (short i=0; i<4; i++)
  {
    short e = h[i] - s[i];

    if (e > d)
    {
      r[i] = 2;
      relays_on = 1;
    }
    else if (e < -d)
    {
      r[i] = 1;
      relays_on = 1;
    }
    else if ((r[i] == 1) && (e > 0))
    {
      r[i] = 0; //stop
    }
    else if ((r[i] == 2) && (e < 0))
    {
      r[i] = 0; //stop
    }

    if (mode_switch == 0)
    {
      r[i] = 0;
    }
  }


  Serial.print("m");
  Serial.print(mode_switch);

  Serial.print(" ;");
  Serial.print(h_raw[0]);
  Serial.print(";");
  Serial.print(h_raw[1]);
  Serial.print(";");
  Serial.print(h_raw[2]);
  Serial.print(";");
  Serial.print(h_raw[3]);

  Serial.print(" r");
  Serial.print(r[0]);
  Serial.print(r[1]);
  Serial.print(r[2]);
  Serial.print(r[3]);
  Serial.println("");


/*
  for (int i=0; i<16; i++)
  {
    int p = getPot(i);

    if (p < 0)
    {
      Serial.print('-');
      p=-p;
    }
    else
    {
      Serial.print('+');
    }
    
    Serial.print((char)('0' + ((p / 100) % 10) ));
    Serial.print((char)('0' + ((p / 10) % 10) ));
    Serial.print((char)('0' + ((p / 1) % 10) ));
    Serial.print(' ');
  }
  Serial.println("");
  */
  
  //sendRelayState(r);
  
  if (mode_switch == 0)
  {
    setLed(0);
  }
  else if (relays_on)
  {
    setLed(1);
  }
  else
  {
    setLed(2);
  }
  
  sendDispState(mode_switch, d, h, s, r);

  updateComms();

  //~20Hz
  delay(50); 
}
