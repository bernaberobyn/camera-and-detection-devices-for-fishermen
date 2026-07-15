import requests
import base64
import os
import time
import pandas as pd

filepath = 'responses.csv'

# Check if the file exists
if os.path.exists(filepath):
   # Load the CSV file into a DataFrame
   df = pd.read_csv(filepath)
   print("File loaded successfully.")
else:
  # Create an empty DataFrame with specified columns
  df = pd.DataFrame(columns = ['timestamp', 'filename', 'response'])
  # Save the empty DataFrame as a CSV file
  df.to_csv(filepath, index=False)

categories = ["Bleached Corals", "Healthy Corals"]
def get_prediction(image_data):
  url = 'https://askai.aiclub.world/cecc4b02-43c5-4bc2-8906-2e8c7120d43e'
  r = requests.post(url, data=image_data)
  response = r.json()['predicted_label']
  print(r.json())
  print(categories[response])
  return categories[response]
image_path = "/home/robynbernabe/HealthyandBleachedCorals/Test/healthy_corals/"
file_names = []
for root, dirs, files in os.walk(image_path):
  for file_name in files:
    file_names.append(file_name)
    with open(image_path + file_name, "rb") as image:
      payload = base64.b64encode(image.read())
    response = get_prediction(payload)
    timestamp = int(time.time())
    df.loc[len(df)+1] = [timestamp, image_path + file_name, response]
    df.to_csv(filepath, index = False)