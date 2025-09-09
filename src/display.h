#ifndef DISPLAY_H
#define DISPLAY_H

#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoOblique18pt7b.h>
#include <Fonts/FreeMonoOblique12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>

// ESP8266 CS(SS)=15,SCL(SCK)=14,SDA(MOSI)=13,BUSY=16,RES(RST)=5,DC=4
#define CS_PIN (15)
#define BUSY_PIN (16)
#define RES_PIN (5)
#define DC_PIN (4)

extern GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display;

void init_eink_display();
void screen_show_loading_screen();
void screen_show_mainscreen();
void screen_update_humidity(float temp);
void screen_update_airtemp(float temp);
void screen_update_thermostat_sensor_temp(float temp);
void screen_update_zadana_temp(float temp);
void screen_grzanie(bool ssnaksn);
void screen_wlaczone(bool ajksdans);
void wyjeba(String blad);
#endif
