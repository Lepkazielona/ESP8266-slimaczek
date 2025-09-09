#include <Arduino.h>
//#include <base64.h>

#//include <ESP8266WiFi.h>

//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
#include "wifi_info.h"
#include <homekit/homekit.h>
#include <arduino_homekit_server.h>

#include "display.h"
#include <Fonts/FreeMonoBold9pt7b.h>

#define LOG_D(fmt, ...)   Serial.printf_P(PSTR("[DEBUG]" fmt "\n") , ##__VA_ARGS__);
#define LOG_HOMEKIT(fmt, ...)   Serial.printf_P(PSTR("[HOMEKIT DEBUG]" fmt "\n") , ##__VA_ARGS__);
#define LOG_EINK(fmt, ...)   Serial.printf_P(PSTR("[EINK DEBUG]" fmt "\n") , ##__VA_ARGS__);

extern "C" homekit_server_config_t config;
//air stats
extern "C" homekit_characteristic_t air_temperature;
extern "C" homekit_characteristic_t air_humidity;

//thermostat
extern "C" homekit_characteristic_t current_heating_cooling_state;
extern "C" homekit_characteristic_t target_heating_cooling_state;
extern "C" homekit_characteristic_t current_temperature;
extern "C" homekit_characteristic_t target_temperature;
extern "C" homekit_characteristic_t thermostat_threshold;

void my_homekit_report();
void target_temp_setter(const homekit_value_t value);
void target_heating_state_setter(const homekit_value_t value);

void setup() {
  Serial.begin(115200);
  delay(1000);
  init_eink_display();
  screen_show_loading_screen();
  LOG_D("Eink init done, loading screen")


  wifi_connect();
  pinMode(LED_BUILTIN, OUTPUT);
  homekit_storage_reset();
  
  target_temperature.setter = target_temp_setter;
  target_heating_cooling_state.setter = target_heating_state_setter;

  arduino_homekit_setup(&config);
  screen_show_mainscreen();
}



void target_temp_setter(const homekit_value_t value) {
  target_temperature.value = value;
  LOG_D("Temperatura ustawiona na %.1f", value.float_value)
}

void target_heating_state_setter(const homekit_value_t value){
  target_heating_cooling_state.value = value;

  if (value.int_value == 1){
    LOG_D("GRZANIE WLACZONE");
  } else {
    LOG_D("GRZANIE WYLACZONE")
  }
} 

//homekit update
static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 5000;


void loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
	if (t > next_report_millis) {
		// report sensor values every 10 seconds
		next_report_millis = t + 10 * 1000;
		my_homekit_report();
	}
	if (t > next_heap_millis) {
		// Show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}

  unsigned long now = millis();
  if (now - lastUpdate >= updateInterval) {
    lastUpdate = now;
    //update_screen();
  }
}




void my_homekit_report() {
 //
 // THERMOSTAT
 //

  float thermostat_temp = random(10,20); 

  if (isnan(thermostat_temp)) {
    LOG_D("AWARIA CZUJNIKA TEMPERATURY TERMOSTATU!!!");
    wyjeba("Awaria czujnika termostatu!");
    return;
  }

///
/// AIR
///
  float air_temp = random(10,20);//dht.readTemperature();
  float air_hum = random(10,20);//dht.readHumidity();

  if (isnan(air_temp) || isnan(air_hum)) {
    LOG_D("Czujnik 1 uszkodzony! (DHT)");
    wyjeba("Awaria czujnika DHT!");

  } else {
    static float last_air_temp = 0.0;
    static float last_air_hum = 0.0;
    if (fabs(last_air_temp - air_temp) > 0.1) {
      homekit_characteristic_notify(&air_temperature, HOMEKIT_FLOAT(air_temp));
      last_air_temp = air_temp;
      LOG_HOMEKIT("Zaktualizowano temperature powietrza do: %.2f", air_temp);

      screen_update_airtemp(air_temp);
      LOG_EINK("Zaktualizowano temperature powietrza na ekranie");
    }
    if (fabs(last_air_hum - air_hum) > 0.1) {
      homekit_characteristic_notify(&air_humidity, HOMEKIT_FLOAT(air_hum));
      last_air_hum = air_hum;
      LOG_HOMEKIT("HOMEKIT: Zaktualizowano temperature powietrza do: %.2f", air_hum);
      screen_update_humidity(air_hum);

    }
  }

  static float last_thermostat_temp = 0.0;
  if (fabs(last_thermostat_temp - thermostat_temp) > 0.1) {
    homekit_characteristic_notify(&current_temperature, HOMEKIT_FLOAT(thermostat_temp));
    last_thermostat_temp = thermostat_temp;
    LOG_HOMEKIT("Zaktualizowano temperature termostatu %.2f", thermostat_temp);

    screen_update_thermostat_sensor_temp(thermostat_temp);
    LOG_EINK("Zaktualizowano temperature czujnika termostatu na ekranie");
  }

  const float target_temp = target_temperature.value.float_value;
  const int target_state = target_heating_cooling_state.value.int_value;

  if (target_state == 1) { // 1 = HEAT
    if (thermostat_temp < target_temp) {
      LOG_D("GRZANIE");
      
      homekit_characteristic_notify(&current_heating_cooling_state, HOMEKIT_UINT8_(1)); // 1 = HEAT
      LOG_D("Turning on thermostat heating");
    } else {
      LOG_D("CHLODZENIE");
      homekit_characteristic_notify(&current_heating_cooling_state, HOMEKIT_UINT8_(0)); // 0 = OFF
      LOG_D("Turning off thermostat heating");
    }
  } else { // Inne tryby, np. OFF (0)
    LOG_D("TERMOSTAT WYLACZONY");
    homekit_characteristic_notify(&current_heating_cooling_state, HOMEKIT_UINT8_(0)); // 0 = OFF
  }
}