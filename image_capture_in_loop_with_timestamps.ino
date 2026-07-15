#include "esp_camera.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include "time.h"

// Pin definition for CAMERA_MODEL_AI_THINKER
// Change pin definition if you're using another ESP32 camera module
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

//# define BUTTON_PIN        0   // Pin connected to the button
#define FLASH_LED_PIN      4   // Flash LED Pin (usually GPIO 4)

// Camera configuration
camera_config_t config;

// Function to initialize the camera
void configInitCamera() {
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

// Manually set the time
void setupManualTime() {
  struct tm timeinfo;
  timeinfo.tm_year = 2024 - 1900; // Year since 1900
  timeinfo.tm_mon = 9;             // Month (0-11, where 0 = January)
  timeinfo.tm_mday = 19;           // Day of the month
  timeinfo.tm_hour = 15;           // Hour (0-23)
  timeinfo.tm_min = 30;            // Minute (0-59)
  timeinfo.tm_sec = 0;             // Second (0-59)

  time_t t = mktime(&timeinfo);
  if (t == -1) {
    Serial.println("Failed to set time using mktime");
    return;
  }

  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}

// Get the picture filename based on the current time
String getPictureFilename() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d_%H-%M-%S", &timeinfo);
  Serial.println(timeString);
  String filename = "/picture_" + String(timeString) + ".jpg";
  return filename;
}

// Initialize the micro SD card
void initMicroSDCard() {
  // Start Micro SD card
  Serial.println("Starting SD Card");
  if (!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD Card attached");
    return;
  }
}

// Capture image and save to SD card
void captureAndSaveImage() {
  // Turn on the flash
  digitalWrite(FLASH_LED_PIN, HIGH);
  delay(100); // Give a little time for illumination

  // Take Picture with Camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    digitalWrite(FLASH_LED_PIN, LOW); // Turn off the flash
    delay(1000);
    return;
  }

  // Path where new picture will be saved in SD Card
  String path = getPictureFilename();
  Serial.printf("Picture file name: %s\n", path.c_str());

  // Save picture to microSD card
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.printf("Failed to open file in writing mode");
  } else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved: %s\n", path.c_str());
  }
  file.close();
  esp_camera_fb_return(fb);

  // Turn off the flash
  digitalWrite(FLASH_LED_PIN, LOW);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  Serial.begin(115200);
  delay(2000);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(FLASH_LED_PIN, OUTPUT); // Set the flash LED pin as an output
  digitalWrite(FLASH_LED_PIN, LOW); // Turn off the flash initially

  // Manually set the initial time
  setupManualTime();

  // Initialize the camera
  Serial.print("Initializing the camera module...");
  configInitCamera();
  Serial.println("Ok!");

  // Initialize MicroSD
  Serial.print("Initializing the MicroSD card module... ");
  initMicroSDCard();
}

void loop() {
  // Check if button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200); // Debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) {
      captureAndSaveImage();
    }
  }
}