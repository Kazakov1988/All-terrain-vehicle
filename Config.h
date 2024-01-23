// I18N settings
// ---------------------------------

/* uncoment one of */
/* #include "i18n/eng.h" */
#include "i18n/rus.h"

// WiFI settings
// ---------------------------------

/* should board try connect to WIFI_CONNECT_SSID
 * ... or setup built-in AP with WIFI_SOFTAP_SSID ? */
#define WIFI_MODE_CLIENT true

/* credentials for connecting to AP */
#define WIFI_CONNECT_SSID "sample-wifi-spot"
#define WIFI_CONNECT_PASS "123456789"

/* credentials for built-in access point */
#define WIFI_SOFTAP_SSID "ESP32-CAM"
#define WIFI_SOFTAP_PASS "12345678"

// Motion settings
// ---------------------------------

/* set exactly one variable to small negative value if car deviates in motion to left or right
 * reasonable values: -10..0 */
#define ADJ_SPEED_L 0 /* tune this if car goes to RIGHT */
#define ADJ_SPEED_R 0 /* tune this if car goes to LEFT  */

// Serial settings
// ---------------------------------

#define SERIAL_SPEED 115200
#define SERIAL_DEBUG true /* disable for release */

// Camera image settings
// ---------------------------------

#define CAMERA_DEFAULT_FRAMESIZE FRAMESIZE_QVGA /* 320x240 */
#define CAMERA_DEFAULT_QUALITY   12 /* acceptable range: 10..63 */
#define CAMERA_DEFAULT_FRAMERATE 15 /* acceptable range: 5 .. 50 */

/* !!! don't edit below this line unless know what you doing !!! */

// camera gpio settings
// ---------------------------------

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

// pwm channels settings
// ---------------------------------
#define LEDC_MOTORS_PWM_FREQ 2000 /* Hz */
#define LEDC_MOTORS_PWM_RES     8 /* bits */
#define LEDC_LEFT_MOTOR_IN1 3
#define LEDC_LEFT_MOTOR_IN2 4
#define LEDC_RIGHT_MOTOR_IN1 5
#define LEDC_RIGHT_MOTOR_IN2 6

#define LEDC_FLASH_PWM_FREQ 5000 /* Hz */
#define LEDC_FLASH_PWM_RES     8 /* bits */
#define LEDC_FLASH_CHAN 7

#define LEDC_SERVO_PWM_FREQ 50 /* Hz */
#define LEDC_SERVO_PWM_RES  16 /* bits */
#define LEDC_SERVO_CHAN 8

// pins settings
// ---------------------------------

#define PIN_LEFT_MOTOR_IN1 12
#define PIN_LEFT_MOTOR_IN2 13

#define PIN_RIGHT_MOTOR_IN1 14
#define PIN_RIGHT_MOTOR_IN2 15

#define PIN_FLASH_LED 4

#define PIN_SERVO 2
