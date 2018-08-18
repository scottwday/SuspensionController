###Suspension Controller

Master:
- 16 Settings pots muxed via 2x CD4051 analog mux chips into A0,A1
- Height setting switch: binary from rotary switch to D4,D5 for [Off,1,2,3]
- LEDs red,green to D2,D3
- Software serial at 9600baud to slave, Slave TXD on D8, Slave RXD on D9

Slave:
- I2C 128x64 OLED module
- HW Serial wired to software serial on master
- Height sensor sampling on A0,A1,A2,A3
- Relay outputs on D3,4,6,7,8,9,10,11

Serial between Master and Slave uses PacketSerial library.
Packets start with a message id, and end with a Keksum (0xAA+sum(bytes[0..end-1]))

=================================================

Hall effect height sensor RQH100030

PIN 1 = GND
PIN 4 = SIGNAL
PIN 5 = 5v

/  3  2  1 \
\ 6  5  4  /

=================================================

-------------------------------------------------
Message A0: 8 bytes payload: Sampled Heights
2	Front left sampled height
2	Front right sampled height
2	Rear left sampled height
2	Rear right sampled height

-------------------------------------------------
Message B0: 1 byte payload: Relay Output
1	Relay bits

-------------------------------------------------
Message B1: 1 byte payload: State
1	Mode (0=off, 1,2,3=height settings)
1	Deadband
4	Filtered heights
4	Setpoint heights
1	Relay bits
