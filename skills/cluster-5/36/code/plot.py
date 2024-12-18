import pandas as pd
import json
import streamlit as st

# Function to load and parse JSON data
def load_data(json_file_path):
    with open(json_file_path, 'r') as file:
        data = json.load(file)  # Load JSON file
    # Flatten the list of dictionaries into a DataFrame
    parsed_data = [
        {
            "time": entry.get("time"),
            "sensor_id": entry.get("sensor_id"),
            "smoke": entry.get("smoke"),
            "temperature": entry.get("temperature")
        }
        for entry in data
        if "time" in entry and "sensor_id" in entry  # Include only relevant entries
    ]
    return pd.DataFrame(parsed_data)

# Load and process the data
file_path = 'sensorDataReformatted.json'
st.title("Sensor Data Visualization")

try:
    data = load_data(file_path)
    if data.empty:
        st.write("No data available for visualization.")
    else:
        # Display data in a table
        st.subheader("Sensor Data")
        st.dataframe(data)

        # Plot temperature over time for each sensor
        st.subheader("Temperature Over Time")
        for sensor_id in data["sensor_id"].unique():
            sensor_data = data[data["sensor_id"] == sensor_id]
            
            # Add a plot title
            st.write(f"### Temperature Trend for Sensor ID: {sensor_id}")
            st.line_chart(sensor_data.set_index("time")["temperature"])
except Exception as e:
    st.error(f"Error loading data: {e}")
