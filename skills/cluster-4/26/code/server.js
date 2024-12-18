const fs = require('fs');
const moment = require('moment-timezone'); 
const Engine = require('tingodb')();
const db = new Engine.Db('./mydb', {});

// Database collection
const sensorDataCollection = db.collection('sensorData');

// Function to load data from smoke.txt, insert it into the database, and perform a query
function loadDataAndQuery() {
    fs.readFile('smoke.txt', 'utf8', (err, data) => {
        if (err) throw err;

        const lines = data.split('\n').filter(line => line.trim().length > 0);
        
        let insertions = []; // Track insertions to ensure completion before query
        
        lines.forEach(line => {
            const [time, id, smoke, temp] = line.split('\t');
            
            // Insert each entry into the database
            insertions.push(sensorDataCollection.insert({
                time: parseInt(time),          // Unix time in seconds
                sensor_id: parseInt(id),       // Sensor ID
                smoke: parseInt(smoke) === 1,  // Convert smoke to Boolean
                temperature: parseFloat(temp)  // Temperature as float
            }));
        });

        // Perform the query only after all data is inserted
        Promise.all(insertions).then(() => {
            console.log('Data from smoke.txt inserted into database.');

            // Query for sensor ID 1 with smoke detected
            sensorDataCollection.find({ sensor_id: 1, smoke: true }, { time: 1, temperature: 1 }).toArray((err, results) => {
                if (err) throw err;
                console.log("Entries for Sensor ID 1 with smoke detected:", results);
            });
        }).catch(err => {
            console.error("Error inserting data:", err);
        });
    });
}

// Call the function to load data and perform the query
loadDataAndQuery();
