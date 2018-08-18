#ifndef MYTYPES_H
#define MYTYPES_H

#define POT_SETPOINT1_F 2
#define POT_SETPOINT1_R 3
#define POT_SETPOINT2_F 4
#define POT_SETPOINT2_R 5
#define POT_SETPOINT3_F 6
#define POT_SETPOINT3_R 7

#define POT_TRIM_LB 8
#define POT_TRIM_LF 9
#define POT_TRIM_RB 10
#define POT_TRIM_RF 11

#define POT_SCALE 12
#define POT_DEADBAND 13

#define POT_MID 512

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
