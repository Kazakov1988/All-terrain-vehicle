// Машинка на ESP32-CAM 

const char* ssid = "Car";
const char* password = "123456789";

#include "esp_wifi.h"
#include "esp_camera.h"
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define CAMERA_MODEL_AI_THINKER

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

void startCameraServer();

const int MotPin0 = 12; // Правый мотор
const int MotPin1 = 13; // Правый мотор
const int MotPin2 = 14; // Левый мотор
const int MotPin3 = 15; // Левый мотор

void initMotors()
{
  ledcSetup(3, 2000, 8); // 2000 Гц ШИМ, разрешение 8-бит
  ledcSetup(4, 2000, 8); // 2000 Гц ШИМ, разрешение 8-бит
  ledcSetup(5, 2000, 8); // 2000 Гц ШИМ, разрешение 8-бит
  ledcSetup(6, 2000, 8); // 2000 Гц ШИМ, разрешение 8-бит
  ledcAttachPin(MotPin0, 3); // Правый мотор
  ledcAttachPin(MotPin1, 4); // Правый мотор
  ledcAttachPin(MotPin2, 5); // Левый мотор
  ledcAttachPin(MotPin3, 6); // Левый мотор
}

const int ServoPin = 2;
void initServo(){
  ledcSetup(8, 50, 16); // 50 Гц ШИМ, разрешение 16-бит, диаппазон от 3250 до 6500.
  ledcAttachPin(ServoPin, 8);
}

void setup(){
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  if (psramFound()){
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Инициализация камеры завершилась ошибкой 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);

  initMotors();
  initServo();

  ledcSetup(7, 5000, 8);
  ledcAttachPin(4, 7);  // Пин 4 - светодиодная вспышка.

  WiFi.softAP(ssid, password);
  IPAddress miIP = WiFi.softAPIP();
  Serial.print("AP IP адрес: ");
  Serial.println(miIP);   //пробар 192.168.4.1

  startCameraServer();

  for (int i = 0; i < 5; i++) {
    ledcWrite(7, 10); // Светодиодная вспышка
    delay(50);
    ledcWrite(7, 0);
    delay(50);
  }
}

void loop() {
  delay(1000);
  Serial.printf("RSSi: %ld dBm\n", WiFi.RSSI());
}
