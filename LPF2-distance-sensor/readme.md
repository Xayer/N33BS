WriteMessage(new byte[] { 0x41, 0x3c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01 }); // Activate voltage reports
- Port input format setup
- port id
- mode 0
- deltavalue - data update speedunit 4 bytes
- 1
- 2
- 3
- notificiation enabled

## roughly equates to:
- WriteMessage(new byte[] { messageType = 0x41 65, portId = , MOde = , deltaUpdateValue = 16 million, activateVoltage Reports = 0x01 });

first values in writMessage ex 0x41 is the message TYPE!
- 16 seconds