import streamlit as st
import pandas as pd
from PIL import Image
import os

# Load the CSV file
csv_file = 'responses.csv'  # Replace with the path to your CSV file
df = pd.read_csv(csv_file)

st.title("Coral Image Predictions Dashboard")

# Count occurrences of each coral type
good_coral_count = df[df['response'] == 'Healthy Corals'].shape[0]
bleached_coral_count = df[df['response'] == 'Bleached Corals'].shape[0]

# Display coral counts at the top
st.write(f"### Coral Count Summary:")
st.write(f"**Good Coral**: {good_coral_count}")
st.write(f"**Bleached Coral**: {bleached_coral_count}")

# Show the dataframe as a table
st.write("### Predictions Table:")
st.dataframe(df)

# Add a dropdown to select file paths from the CSV
selected_image_path = st.selectbox("Select an image to view:", df['filename'])

# Display the selected image
if os.path.exists(selected_image_path):
    st.image(Image.open(selected_image_path), caption=selected_image_path)
else:
    st.write("Image not found!")