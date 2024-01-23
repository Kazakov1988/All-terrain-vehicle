#include "esp_wifi.h"
#include "esp_camera.h"
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "Config.h"

void startCameraServer();

void initMotors() {
  /* typical: 2000 Hz PWM, 8-bit resolution */
  ledcSetup(LEDC_LEFT_MOTOR_IN1,  LEDC_MOTORS_PWM_FREQ, LEDC_MOTORS_PWM_RES);
  ledcSetup(LEDC_LEFT_MOTOR_IN2,  LEDC_MOTORS_PWM_FREQ, LEDC_MOTORS_PWM_RES);
  ledcSetup(LEDC_RIGHT_MOTOR_IN1, LEDC_MOTORS_PWM_FREQ, LEDC_MOTORS_PWM_RES);
  ledcSetup(LEDC_RIGHT_MOTOR_IN2, LEDC_MOTORS_PWM_FREQ, LEDC_MOTORS_PWM_RES);

  ledcAttachPin(PIN_LEFT_MOTOR_IN1, LEDC_LEFT_MOTOR_IN1);
  ledcAttachPin(PIN_LEFT_MOTOR_IN2, LEDC_LEFT_MOTOR_IN2);

  ledcAttachPin(PIN_RIGHT_MOTOR_IN1, LEDC_RIGHT_MOTOR_IN1);
  ledcAttachPin(PIN_RIGHT_MOTOR_IN2, LEDC_RIGHT_MOTOR_IN2);
}

void initServo() {
  // typical: 50 Hz PWM, 16-bit resolution
  // acceptable range - from 3250 to 6500.
  ledcSetup(LEDC_SERVO_CHAN, LEDC_SERVO_PWM_FREQ, LEDC_SERVO_PWM_RES);
  ledcAttachPin(PIN_SERVO, LEDC_SERVO_CHAN);
}

void initFlashLed() {
  // typical: 5 kHz, 8-bit resolution
  ledcSetup(LEDC_FLASH_CHAN, LEDC_FLASH_PWM_FREQ, LEDC_FLASH_PWM_RES);
  ledcAttachPin(PIN_FLASH_LED, LEDC_FLASH_CHAN);
}

void initCamera() {
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
    config.frame_size = CAMERA_DEFAULT_FRAMESIZE;
    config.jpeg_quality = CAMERA_DEFAULT_QUALITY;
    config.fb_count = 2;
  } else {
    config.frame_size = CAMERA_DEFAULT_FRAMESIZE;
    config.jpeg_quality = CAMERA_DEFAULT_QUALITY;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf(MSG_CAMERA_INIT_FAIL, err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, CAMERA_DEFAULT_FRAMESIZE);
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
}

void setup() {
  // bug workaround, see https://github.com/espressif/arduino-esp32/issues/863
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  Serial.begin(SERIAL_SPEED);
  Serial.setDebugOutput(SERIAL_DEBUG);
  Serial.println();

  initCamera();
  initMotors();
  initServo();
  initFlashLed();

#if WIFI_MODE_CLIENT
  Serial.println("SSID: " + (String) WIFI_CONNECT_SSID);
  Serial.println("Password: " + (String) WIFI_CONNECT_PASS);

  WiFi.begin(WIFI_CONNECT_SSID, WIFI_CONNECT_PASS);
  delay(500);

  long int StartTime=millis();
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if ((StartTime+10000) < millis()) break;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println(MSG_WIFI_CONNECTED);
    Serial.printf(MSG_CAMERA_READY, WiFi.localIP().toString());
  } else {
    Serial.println("");
    Serial.println(MSG_WIFI_DISCONNECTED);
    Serial.printf(MSG_CAMERA_READY, WiFi.softAPIP().toString());
    WiFi.softAP((WiFi.softAPIP().toString()+"_"+(String)WIFI_SOFTAP_SSID).c_str(), WIFI_SOFTAP_PASS);
  }
#else
  WiFi.softAP(WIFI_SOFTAP_SSID, WIFI_SOFTAP_PASS);
  IPAddress miIP = WiFi.softAPIP();
  Serial.printf(MSG_AP_IP_ADDRESS, miIP.toString());
#endif

  startCameraServer();

  for (int i = 0; i < 5; i++) {
    ledcWrite(LEDC_FLASH_CHAN, 10); // flash led
    delay(50);
    ledcWrite(LEDC_FLASH_CHAN, 0);
    delay(50);
  }
}

void loop() {
  delay(1000);
  Serial.printf("RSSi: %ld dBm\n", WiFi.RSSI());
}
