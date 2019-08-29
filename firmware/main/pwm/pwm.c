#include "pwm/pwm.h"
#include "pwm/adc.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_err.h"

static const char *TAG = "pwm";

static void set_duty(uint32_t duty);
static void thread(void *params);

static ledc_channel_config_t ledc_channel = {
  .channel    = LEDC_CHANNEL_0,
  .duty       = 0,
  .gpio_num   = 18,
  .speed_mode = LEDC_HIGH_SPEED_MODE,
  .hpoint     = 0,
  .timer_sel  = LEDC_TIMER_0,
  .intr_type  = LEDC_INTR_DISABLE
};

bool pwm_init() {
  ledc_timer_config_t ledc_timer = {
    .duty_resolution = LEDC_TIMER_11_BIT,
    .freq_hz = 35000,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_num = LEDC_TIMER_0
  };
  ledc_timer_config(&ledc_timer);
  ledc_channel_config(&ledc_channel);

  adc_init();

  return xTaskCreate(thread, TAG, 4096, NULL, 15, NULL) == pdPASS;
}

static void set_duty(uint32_t duty) {
  ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
  ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}

static void thread(void *params) {
  const uint32_t MAX_DUTY = 1500; // overvoltage protection
  const uint32_t MARGIN = 8;
  const uint32_t THRESHOLD = 800;

  uint32_t duty = 0;
  while(true) {
    if (duty > MAX_DUTY) {
      duty = MAX_DUTY;
    }

    uint32_t adc_reading = adc_read(); // 11 bit adc, 11 bit pwm

    if(adc_reading > (THRESHOLD + MARGIN)) {
  		set_duty(--duty);
  	} else if(adc_reading < (THRESHOLD - MARGIN)) {
  		set_duty(++duty);
  	}

    taskYIELD();
  }
}
