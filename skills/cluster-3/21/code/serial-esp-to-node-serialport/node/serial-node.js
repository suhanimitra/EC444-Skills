const express = require('express');
const http = require('http');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const fs = require('fs');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = require('socket.io')(server);

// Initialize serial port (modify the path to your ESP32 port)
const serialPort = new SerialPort({ path: '/dev/cu.usbserial-0246DDC2', baudRate: 115200 });
const parser = serialPort.pipe(new ReadlineParser({ delimiter: '\n' }));

const port = 3000;  // Set your preferred port

// Path to the CSV file for storing sensor data
const filePath = path.join(__dirname, 'sensor_data.csv');

// Function to append sensor data to CSV file
function appendSensorData(temp, lux) {
    const currentTime = new Date().toISOString();
    const csvLine = `${currentTime}, ${temp}, ${lux}\n`;
    
    fs.appendFile(filePath, csvLine, (err) => {
        if (err) {
            console.error('Error writing to file:', err);
        } else {
            console.log('Sensor data written to file');
        }
    });
}

// Serve the HTML page
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

// WebSocket handling
io.on('connection', (socket) => {
    console.log('A user connected');

    // Listen for data from the serial port
    parser.on('data', (data) => {
        console.log('Data from ESP32:', data);

        // Parse the data from the ESP32
        const match = data.match(/Temp:\s*(-?\d+\.\d+)\s*C,\s*Light:\s*(\d+\.\d+)\s*Lux/);
        if (match) {
            const temp = parseFloat(match[1]);
            const lux = parseFloat(match[2]);

            // Append data to CSV file
            appendSensorData(temp, lux);

            // Emit the parsed data to the client via WebSocket
            socket.emit('data', { temp, lux });
        } else {
            console.error('Data format not recognized:', data);
        }
    });

    // Handle disconnection
    socket.on('disconnect', () => {
        console.log('A user disconnected');
    });
});

// Start the server
server.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
