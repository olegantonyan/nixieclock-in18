#include "pwm/pwm.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

bool pwm_init() {
  ledc_timer_config_t ledc_timer = {
    .duty_resolution = LEDC_TIMER_11_BIT,
    .freq_hz = 30000,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_num = LEDC_TIMER_0
  };
  ledc_timer_config(&ledc_timer);

  ledc_channel_config_t ledc_channel = {
    .channel    = LEDC_CHANNEL_0,
    .duty       = 0,
    .gpio_num   = 18,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .hpoint     = 0,
    .timer_sel  = LEDC_TIMER_0,
    .intr_type  = LEDC_INTR_DISABLE
  };
  ledc_channel_config(&ledc_channel);

  bool up = true;
  uint32_t duty = 100;
  while(true) {
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, (up ? ++duty : --duty));
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    if (up && duty >= 1950) {
      up = false;
    }
    if (!up && duty <= 100) {
      up = true;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }

  return true;
}
