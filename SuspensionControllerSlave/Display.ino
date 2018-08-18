#include <U8g2lib.h>
#include <U8x8lib.h>
#include "MyTypes.h"

#define nmax 100
#define y0   12
#define y1   (12 + 2 + nmax)
#define x0   16
#define x1   48
#define r0   5

#define IS_RELAY(b,i,m) ( ( (b >> (i*2)) & 0x03 ) == m )

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R1, U8X8_PIN_NONE);

StateMsgB0 dispState = {0};

void setupDisplay()
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_12_mf);
}

void setDisplayState(void* state)
{
  dispState = *((StateMsgB0*)state);

  Serial.print("D");
  Serial.print(dispState.mode);
  Serial.print(",");
  Serial.print(dispState.deadband);
  Serial.println("");
  
}

void updateDisplay()
{
  u8g2.clearBuffer();

  u8g2.setCursor(3, 126);
  u8g2.print("fl");
  u8g2.setCursor(18, 126);
  u8g2.print("fr");

  u8g2.setCursor(35, 126);
  u8g2.print("rl");
  u8g2.setCursor(51, 126);
  u8g2.print("rr");


  u8g2.drawLine(x0, y0, x0, y1);
  u8g2.drawLine(x0-1, y0, x0-2, y0);
  u8g2.drawLine(x0-1, y1, x0-2, y1);

  u8g2.drawLine(x1, y0, x1, y1);
  u8g2.drawLine(x1+1, y0, x1+2, y0);
  u8g2.drawLine(x1+1, y1, x1+2, y1);

  if (dispState.mode != 0)
  {
    u8g2.drawLine(x0 - 2, y0+(nmax-dispState.setpointHeights[0]) - dispState.deadband, x0 - 2, y0 + (nmax-dispState.setpointHeights[0]) + dispState.deadband);
    u8g2.drawLine(x0 + 2, y0+(nmax-dispState.setpointHeights[1]) - dispState.deadband, x0 + 2, y0 + (nmax-dispState.setpointHeights[1]) + dispState.deadband);
    u8g2.drawLine(x0 - 2, y0+(nmax-dispState.setpointHeights[0]), x0 - 2 - 2, y0 + (nmax-dispState.setpointHeights[0]));
    u8g2.drawLine(x0 + 2, y0+(nmax-dispState.setpointHeights[1]), x0 + 2 + 2, y0 + (nmax-dispState.setpointHeights[1]));
  }
  drawArrow(x0 - 4, y0+nmax-dispState.filteredHeights[0], 4);
  drawArrow(x0 + 4, y0+nmax-dispState.filteredHeights[1], -4);
  
  if (dispState.mode != 0)
  {
    u8g2.drawLine(x1 - 2, y0+(nmax-dispState.setpointHeights[2]) - dispState.deadband, x1 - 2, y0 + (nmax-dispState.setpointHeights[2]) + dispState.deadband);
    u8g2.drawLine(x1 + 2, y0+(nmax-dispState.setpointHeights[3]) - dispState.deadband, x1 + 2, y0 + (nmax-dispState.setpointHeights[3]) + dispState.deadband);
    u8g2.drawLine(x1 - 2, y0+(nmax-dispState.setpointHeights[2]), x1 - 2 - 2, y0 + (nmax-dispState.setpointHeights[2]));
    u8g2.drawLine(x1 + 2, y0+(nmax-dispState.setpointHeights[3]), x1 + 2 + 2, y0 + (nmax-dispState.setpointHeights[3]));
  }
  
  drawArrow(x1 - 4, y0+nmax-dispState.filteredHeights[2], 4);
  drawArrow(x1 + 4, y0+nmax-dispState.filteredHeights[3], -4);

  drawArrowV(x0 - 3 - r0, 4, r0, IS_RELAY(dispState.relays,0,1) );
  drawArrowV(x0 - 3, 9, -r0, IS_RELAY(dispState.relays,0,2) );
  drawArrowV(x0 + 3, 4, r0,  IS_RELAY(dispState.relays,1,1) );
  drawArrowV(x0 + 3 + r0, 9, -r0, IS_RELAY(dispState.relays,1,2) );

  drawArrowV(x1 - 3 - r0, 4, r0,  IS_RELAY(dispState.relays,2,1) );
  drawArrowV(x1 - 3, 9, -r0, IS_RELAY(dispState.relays,2,2) );
  drawArrowV(x1 + 3, 4, r0, IS_RELAY(dispState.relays,3,1) );
  drawArrowV(x1 + 3 + r0, 9, -r0, IS_RELAY(dispState.relays,3,2) );

  u8g2.setCursor(30, 8);
  char str[3] = {0};
  str[0] = '0' + dispState.mode;
  u8g2.print(str);

  u8g2.sendBuffer();
}

void drawArrow(int x, int y, int a)
{
  u8g2.drawLine(x, y, x - a, y - a);
  u8g2.drawLine(x, y, x - a, y + a);
  u8g2.drawLine(x - a, y - a, x - a, y + a);
}

void drawArrowV(int x, int y, int a, bool f)
{
  if (f)
  {
  u8g2.drawTriangle(x, y, x-a,y+a, x+a,y+a);
  }
  else
  {
  u8g2.drawLine(x, y, x - a, y + a);
  u8g2.drawLine(x, y, x + a, y + a);
  u8g2.drawLine(x - a, y + a, x + a, y + a);
  }
}

