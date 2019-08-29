#include "pwm/adc.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "adc";

#define ADC_CHANNEL ADC1_CHANNEL_6 // gpio 34

static void check_efuse();
static void print_char_val_type(esp_adc_cal_value_t val_type);

static esp_adc_cal_characteristics_t *adc_chars = NULL;

bool adc_init() {
  check_efuse();
  adc1_config_width(ADC_WIDTH_BIT_11);
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_0);

  adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
  if (adc_chars == NULL) {
    return false;
  }
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_11, 1100, adc_chars);
  print_char_val_type(val_type);
  return true;
}

uint32_t adc_read() {
  const size_t samples = 64;

  uint32_t adc_reading = 0;
  for (size_t i = 0; i < samples; i++) {
    adc_reading += adc1_get_raw(ADC_CHANNEL);
  }
  adc_reading /= samples;

  //uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
  //printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);

  return adc_reading;
}

static void check_efuse() {
  //Check TP is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
    ESP_LOGI(TAG, "eFuse Two Point: Supported");
  } else {
    ESP_LOGI(TAG, "eFuse Two Point: NOT supported");
  }

  //Check Vref is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
    ESP_LOGI(TAG, "eFuse Vref: Supported");
  } else {
    ESP_LOGI(TAG, "eFuse Vref: NOT supported");
  }
}

static void print_char_val_type(esp_adc_cal_value_t val_type) {
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    ESP_LOGI(TAG, "characterized using Two Point Value");
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    ESP_LOGI(TAG, "characterized using eFuse Vref");
  } else {
    ESP_LOGI(TAG, "characterized using Default Vref");
  }
}
