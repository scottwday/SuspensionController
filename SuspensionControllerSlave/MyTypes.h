#ifndef MYTYPES_H
#define MYTYPES_H

struct StateMsgB0
{
  byte mode;
  byte deadband;
  byte filteredHeights[4];
  byte setpointHeights[4];
  byte relays;
};
// 11 bytes

#endif
