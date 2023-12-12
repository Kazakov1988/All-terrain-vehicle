#include <esp32-hal-ledc.h>
int speed = 255;
int speedBalansR = 0;  // Если автомобиль отклоняется влево, уменьшите скорость правого мотора.
int speedBalansL = 0;  // Если автомобиль отклоняется вправо, уменьшите скорость левого мотора.
int noStop = 0;

#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "Arduino.h"
#include "Config.h"

typedef struct {
  httpd_req_t *req;
  size_t len;
} jpg_chunking_t;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;
httpd_handle_t camera_httpd = NULL;

static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len) {
  jpg_chunking_t *j = (jpg_chunking_t *)arg;
  if (!index) {
    j->len = 0;
  }
  if (httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) {
    return 0;
  }
  j->len += len;
  return len;
}

static esp_err_t capture_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  int64_t ts_start = esp_timer_get_time();

  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println(MSG_CAMERA_CAPTURE_FAIL);
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  httpd_resp_set_type(req, "image/jpeg");
  httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");

  size_t out_len, out_width, out_height;
  uint8_t * out_buf;
  size_t fb_len = 0;
  if (fb->format == PIXFORMAT_JPEG) {
    fb_len = fb->len;
    res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
  } else {
    jpg_chunking_t jchunk = {req, 0};
    res = frame2jpg_cb(fb, 80, jpg_encode_stream, &jchunk) ? ESP_OK : ESP_FAIL;
    httpd_resp_send_chunk(req, NULL, 0);
    fb_len = jchunk.len;
  }
  esp_camera_fb_return(fb);
  int64_t ts_end = esp_timer_get_time();
  Serial.printf("JPG: %uB %ums\n", (uint32_t)(fb_len), (uint32_t)((ts_end - ts_start) / 1000));
  return res;
}

static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  static int64_t last_frame = 0;
  if (!last_frame) {
    last_frame = esp_timer_get_time();
  }

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) {
    return res;
  }

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println(MSG_CAMERA_CAPTURE_FAIL);
      res = ESP_FAIL;
    } else {
      {
        if (fb->format != PIXFORMAT_JPEG) {
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!jpeg_converted) {
            Serial.println(MSG_CAMERA_JPEG_COMP_FAIL);
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK) {
      break;
    }
    int64_t fr_end = esp_timer_get_time();
    int64_t frame_time = fr_end - last_frame;
    last_frame = fr_end;
    frame_time /= 1000;
    Serial.printf("MJPG: %uB %ums (%.1ffps)\n",
                  (uint32_t)(_jpg_buf_len),
                  (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time
                 );
  }

  last_frame = 0;
  return res;
}

enum state {fwd, rev, stp};
state actstate = stp;

static esp_err_t cmd_handler(httpd_req_t *req)
{
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  char value[32] = {0,};

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if (!buf) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
          httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  int val = atoi(value);
  sensor_t * s = esp_camera_sensor_get();
  int res = 0;

  if (!strcmp(variable, "framesize")) {
    Serial.println("framesize");
    if (s->pixformat == PIXFORMAT_JPEG) res = s->set_framesize(s, (framesize_t)val);
  } else if (!strcmp(variable, "quality")) {
    Serial.println("quality");
    res = s->set_quality(s, val);
  } else if (!strcmp(variable, "flash")) {
    ledcWrite(LEDC_FLASH_CHAN, val);
  } else if (!strcmp(variable, "speed")) {
    if      (val > 255) val = 255;
    else if (val <   0) val = 0;
    speed = val;
  } else if (!strcmp(variable, "nostop")) {
    noStop = val;
  } else if (!strcmp(variable, "servo")) {
    // 3250, 4875, 6500
    if      (val > 650) val = 650;
    else if (val < 325) val = 325;
    ledcWrite(LEDC_SERVO_CHAN, 10 * val);
  } else if (!strcmp(variable, "car")) {
    if (val == 1) {
      Serial.println(MSG_MOVE_FORWARD);
      actstate = fwd;
      ledcWrite(LEDC_LEFT_MOTOR_IN1,  0);
      ledcWrite(LEDC_LEFT_MOTOR_IN2,  speed - ADJ_SPEED_R);
      ledcWrite(LEDC_RIGHT_MOTOR_IN1, speed - ADJ_SPEED_L);
      ledcWrite(LEDC_RIGHT_MOTOR_IN2, 0);
      delay(200);
    } else if (val == 2) {
      Serial.println(MSG_TURN_LEFT);
      ledcWrite(LEDC_LEFT_MOTOR_IN1, 0);
      ledcWrite(LEDC_LEFT_MOTOR_IN2, speed - ADJ_SPEED_R);
      ledcWrite(LEDC_RIGHT_MOTOR_IN1, 0);
      ledcWrite(LEDC_RIGHT_MOTOR_IN2, speed - ADJ_SPEED_L);
      delay(100);
    } else if (val == 3) {
      Serial.println(MSG_MOVE_STOP);
      actstate = stp;
      ledcWrite(LEDC_LEFT_MOTOR_IN1, 0);
      ledcWrite(LEDC_LEFT_MOTOR_IN2, 0);
      ledcWrite(LEDC_RIGHT_MOTOR_IN1, 0);
      ledcWrite(LEDC_RIGHT_MOTOR_IN2, 0);
    } else if (val == 4) {
      Serial.println(MSG_TURN_RIGHT);
      ledcWrite(LEDC_LEFT_MOTOR_IN1,  speed - ADJ_SPEED_R);
      ledcWrite(LEDC_LEFT_MOTOR_IN2, 0);
      ledcWrite(LEDC_RIGHT_MOTOR_IN1, speed - ADJ_SPEED_L);
      ledcWrite(LEDC_RIGHT_MOTOR_IN2, 0);
      delay(100);
    } else if (val == 5) {
      Serial.println(MSG_MOVE_BACKWARD);
      actstate = rev;
      ledcWrite(LEDC_LEFT_MOTOR_IN1, 0);
      ledcWrite(LEDC_LEFT_MOTOR_IN2, speed - ADJ_SPEED_R);
      ledcWrite(LEDC_RIGHT_MOTOR_IN1, 0);
      ledcWrite(LEDC_RIGHT_MOTOR_IN2, speed - ADJ_SPEED_L);
      delay(200);
    } else if (val == 6) {
      Serial.println(MSG_ESP_RESTART);
      ESP.restart();
    }
    if (noStop != 1) {
      ledcWrite(LEDC_LEFT_MOTOR_IN1, 0);
      ledcWrite(LEDC_LEFT_MOTOR_IN2, 0);
      ledcWrite(LEDC_RIGHT_MOTOR_IN1, 0);
      ledcWrite(LEDC_RIGHT_MOTOR_IN2, 0);
    }
  } else {
    Serial.println("variable");
    res = -1;
  }

  if (res) {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

static esp_err_t status_handler(httpd_req_t *req) {
  static char json_response[1024];

  sensor_t * s = esp_camera_sensor_get();
  char * p = json_response;
  *p++ = '{';

  p += sprintf(p, "\"framesize\":%u,", s->status.framesize);
  p += sprintf(p, "\"quality\":%u,", s->status.quality);
  *p++ = '}';
  *p++ = 0;
  httpd_resp_set_type(req, "application/json");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, json_response, strlen(json_response));
}

static esp_err_t index_handler(httpd_req_t *req){
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

void startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = index_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t status_uri = {
        .uri       = "/status",
        .method    = HTTP_GET,
        .handler   = status_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t cmd_uri = {
        .uri       = "/control",
        .method    = HTTP_GET,
        .handler   = cmd_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t capture_uri = {
        .uri       = "/capture",
        .method    = HTTP_GET,
        .handler   = capture_handler,
        .user_ctx  = NULL
    };

   httpd_uri_t stream_uri = {
        .uri       = "/stream",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };

    Serial.printf(MSG_START_WEBSERVER, config.server_port);
    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(camera_httpd, &index_uri);
        httpd_register_uri_handler(camera_httpd, &cmd_uri);
        httpd_register_uri_handler(camera_httpd, &status_uri);
        httpd_register_uri_handler(camera_httpd, &capture_uri);
    }

    config.server_port += 1;
    config.ctrl_port += 1;
    Serial.printf(MSG_START_STREAMSERVER, config.server_port);
    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &stream_uri);
    }
}
