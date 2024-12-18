//used ChatGPT to assist with writing some of this code

const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const fs = require('fs');
const path = require('path');

// Initialize serial port
const port = new SerialPort({ path: '/dev/cu.usbserial-0246DDC2', baudRate: 115200 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

// File path for storing the sensor data
const filePath = path.join(__dirname, 'sensor_data.csv');

// Function to append sensor data to a CSV file
function appendSensorData(data) {
  const currentTime = new Date().toISOString(); // Get current time in ISO format
  const csvLine = `${currentTime}, ${data}\n`; // Format as 'timestamp,sensor-value'

  // Append the data to the file
  fs.appendFile(filePath, csvLine, (err) => {
    if (err) {
      console.error('Error writing to file', err);
    } 
    // else {
    //   console.log('Sensor data written to file');
    // }
  });
}

// Handle serial port open event
port.on('open', () => {
  console.log('Serial port now open');
});

// Handle incoming data from ESP32
parser.on('data', (data) => {
  console.log('The word from ESP32:', data);
  appendSensorData(data); // Write data to CSV file
});
