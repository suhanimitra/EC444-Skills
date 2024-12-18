// Required module
var dgram = require('dgram');

// Port and IP
var PORT = 3333;
var HOST = '192.168.1.138';

// Create socket
var server = dgram.createSocket('udp4');

// Variable to alternate between 500ms and 1000ms
let toggleBlinkTime = true;

// Create server that listens on a port
server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

// Create server
server.on('message', function (message, remote) {
    console.log(remote.address + ':' + remote.port + ' - ' + message);

    // On receiving a request, alternate between 500ms and 1000ms
    if(message.toString() === "Request Blink Time") {
        let blinkTime = toggleBlinkTime ? "500" : "1000"; // Alternate between 500ms and 1000ms

        // Send the response with the blink time
        server.send(blinkTime, remote.port, remote.address, function(error){
            if(error){
                console.log('Error sending blink time');
            } else {
                console.log('Sent: ' + blinkTime);
            }
        });

        // Toggle blink time for the next request
        toggleBlinkTime = !toggleBlinkTime;
    }
});

// Bind server to port and IP
server.bind(PORT, HOST);
