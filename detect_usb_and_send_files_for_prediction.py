import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import os
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

image_path = "../../media/robynbernabe/"
destination = "/home/robynbernabe/CoralHealthProject/captureimages/"

class USBHandler(FileSystemEventHandler):
    def on_created(self, event):
        if event.is_directory:
            # Wait to ensure the directory is fully mounted and available
            time.sleep(1)
            print(f"Directory created: {event.src_path}")
            list_files_on_usb(event.src_path)

def list_files_on_usb(mount_point):
    files_list = []
    for root, dirs, files in os.walk(mount_point):
        for file in files:
            file_path = os.path.join(root, file)
            files_list.append(file_path)
            with open(image_path + file, "rb") as image:
              payload = base64.b64encode(image.read())
            response = get_prediction(payload)
            timestamp = int(time.time())
            df.loc[len(df)+1] = [timestamp, destination+file, response]
            df.to_csv(filepath, index = False)

            # Move the file to a folder in Raspberry pi
            os.rename(image_path + file, destination)

    print("Files detected in the USB drive:")
    for file in files_list:
        print(file)

def monitor_mount_point(mount_point='/media/robynbernabe/'):
    event_handler = USBHandler()
    observer = Observer()
    observer.schedule(event_handler, path=mount_point, recursive=True)
    observer.start()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == "__main__":
    monitor_mount_point()

