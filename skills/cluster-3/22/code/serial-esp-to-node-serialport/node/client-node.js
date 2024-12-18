const dgram = require('dgram');
const client = dgram.createSocket('udp4');

// Replace with your ESP32's IP and Port
const ESP32_IP = '192.168.1.126'; // IP of ESP32
const ESP32_PORT = 3335;

let state = 0; // Initialize state

// Function to send a message based on the state
function sendMessage() {
    let message;

    if (state === 0) {
        message = Buffer.from('TURN ON LED');
        state = 1;
    } else {
        message = Buffer.from('TURN OFF LED');
        state = 0;
    }

    // Send the message to ESP32
    client.send(message, ESP32_PORT, ESP32_IP, (error) => {
        if (error) {
            console.error('Error sending message:', error);
            client.close();
        } else {
            console.log('Message sent to ESP32:', message.toString());
        }
    });
}

// Send the message every 2 seconds (2000ms)
setInterval(sendMessage, 2000);

// Listen for messages from the ESP32
client.on('message', (msg, rinfo) => {
    console.log(`Received from ESP32: ${msg.toString()}`);
});