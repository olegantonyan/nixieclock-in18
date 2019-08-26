#include "config/config.h"
#include "storage/nvs.h"
#include "config/defaults.h"

#define MAX_WIFI_SSID_LENGTH 32
#define MAX_WIFI_PASS_LENGTH 64
#define MAX_TIMEZONE_LENGTH 32

char *config_wifi_ssid() {
  static char buffer[MAX_WIFI_SSID_LENGTH] = {0};
  bool ok = nvs_read_string("wifi_ssid", buffer, sizeof(buffer));
  if (ok) {
    return buffer;
  }
  config_save_wifi_ssid(DEFAULT_WIFI_SSID);
  return DEFAULT_WIFI_SSID;
}

char *config_wifi_pass() {
  static char buffer[MAX_WIFI_PASS_LENGTH] = {0};
  bool ok = nvs_read_string("wifi_pass", buffer, sizeof(buffer));
  if (ok) {
    return buffer;
  }
  config_save_wifi_pass(DEFAULT_WIFI_PASS);
  return DEFAULT_WIFI_PASS;
}

bool config_save_wifi_ssid(const char * arg) {
  if (arg == NULL) {
    return false;
  }
  return nvs_save_string("wifi_ssid", (char *)arg);
}

bool config_save_wifi_pass(const char * arg) {
  if (arg == NULL) {
    return false;
  }
  return nvs_save_string("wifi_pass", (char *)arg);
}

char *config_web_login() {
  char *t = "admin";
  return t;
}

char *config_web_password() {
  return config_ap_password();
}

char *config_ap_password() {
  static char buffer[16] = {0};
  bool ok = nvs_read_string("ap_pass", buffer, sizeof(buffer));
  if (ok) {
    return buffer;
  }
  nvs_save_string("ap_pass", DEFAULT_APPASS);
  return DEFAULT_APPASS;
}

config_ip_addr_t config_ap_static_ip() {
  config_ip_addr_t t = {
    .oct1 = 10,
    .oct2 = 10,
    .oct3 = 0,
    .oct4 = 1
  };
  return t;
}

char *config_timezone() {
  static char buffer[MAX_TIMEZONE_LENGTH] = {0};
  bool ok = nvs_read_string("timezone", buffer, sizeof(buffer));
  if (ok) {
    return buffer;
  }
  config_save_timezone(DEFAULT_TIMEZONE);
  return DEFAULT_TIMEZONE;
}

bool config_save_timezone(const char *arg) {
  return nvs_save_string("timezone", (char *)arg);
}
