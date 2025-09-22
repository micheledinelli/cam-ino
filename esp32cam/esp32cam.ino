
#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"

int lastState = LOW;
unsigned long lastTriggerTime = 0;
const unsigned long debounceDelay = 2000;

#define TRIGGER_PIN 12

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

camera_config_t config;

void configInitCamera() {
  camera_config_t config;
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_SVGA;
  config.pixel_format = PIXFORMAT_JPEG;
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  }

  sensor_t* s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);   
    s->set_brightness(s, 1);  
    s->set_saturation(s, -2);
  }

  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }
}

String getPictureFilename() {
  unsigned long randNum = random(100000, 999999);
  String filename = "/picture_" + String(randNum) + ".jpg";
  return filename;
}

void initMicroSDCard() {
  Serial.println("Starting SD Card");
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD Card attached");
    return;
  }
}

void takeSavePhoto() {
  camera_fb_t* fb = esp_camera_fb_get();

  //Uncomment the following lines if you're getting old pictures
  //esp_camera_fb_return(fb); // dispose the buffered image
  //fb = NULL; // reset to capture errors
  //fb = esp_camera_fb_get();

  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }

  String path = getPictureFilename();
  Serial.printf("Picture file name: %s\n", path.c_str());

  fs::FS& fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.printf("Failed to open file in writing mode");
  } else {
    file.write(fb->buf, fb->len);  // payload (image), payload length
    Serial.printf("Saved: %s\n", path.c_str());
  }
  file.close();
  esp_camera_fb_return(fb);
}


void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // disable brownout detector

  Serial.begin(115200);
  delay(2000);

  Serial.print("Initializing the camera module...");
  configInitCamera();
  Serial.println("Ok!");

  Serial.print("Initializing the MicroSD card module... ");
  initMicroSDCard();
  Serial.println("Ok!");

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  randomSeed(esp_random());

  Serial.println("ESP32CAM ready!");
}

void loop() {
  unsigned long now = millis();

  int state = digitalRead(TRIGGER_PIN);
  if (state == HIGH && lastState == LOW && (now - lastTriggerTime > debounceDelay)) {
    Serial.println("Signal received");
    takeSavePhoto();

    lastTriggerTime = now;
  }
}
