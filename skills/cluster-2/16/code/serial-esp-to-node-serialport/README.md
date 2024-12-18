# Serial ESP to Node Serialport Demo
2023-10-01

### ESP32 main.c
- Uses UART 0
- Baud rate 115200
- Sends string and integer counting up from 0

### Node.js serial-node
- In node directory
- Uses serial port
- Baud rate 115200
- Path: to your COM port device
- Receives data from ESP32 via serial port
- Prints message, then string and data

This is also in the design pattern for node.js