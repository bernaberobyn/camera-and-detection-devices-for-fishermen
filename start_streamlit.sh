#!/bin/bash

# Log file
LOGFILE=~/script_log.txt
echo "Starting script at $(date)" >> $LOGFILE

# Step 1: Start the Streamlit app in the background
echo "Starting Streamlit..." >> $LOGFILE
streamlit run /home/robynbernabe/CoralHealthProject/dashboard.py &
STREAMLIT_PID=$!

# Step 2: Wait for Streamlit to start
sleep 5
echo "Waiting for Streamlit to start..." >> $LOGFILE

# Check if Streamlit is running
if ps -p $STREAMLIT_PID > /dev/null
then
    echo "Streamlit is running..." >> $LOGFILE
else
    echo "Streamlit failed to start." >> $LOGFILE
    exit 1
fi

# Step 3: Open Chromium in full-screen mode
echo "Launching Chromium..." >> $LOGFILE
chromium-browser --start-fullscreen --app=http://localhost:8501 &
sleep 2

# Step 4: Send an F11 keypress
echo "Sending F11 keypress..." >> $LOGFILE
xte "key F11"

echo "Script completed at $(date)" >> $LOGFILE