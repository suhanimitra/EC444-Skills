# Serial to canvasJS demo
2023-10-05

### ESP32 main.c
- Uses UART 0
- Baud rate 115200
- Sends two values (count, time) counting up from 0
- Once every second

### Node serial-canvas.js
- In node directory
- Uses serial port
- Baud rate 115200
- Path: to your COM port device
- Receives data from ESP32 via serial port
- Sends data via socket.io to address (localhost:3000)
- Serves up index.html that consumes data sent from serial-node.js app
- Browser plots data
