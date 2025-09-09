#include "display.h"


// Deklaracja obiektu display
GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(CS_PIN, DC_PIN, RES_PIN, BUSY_PIN));


void init_eink_display() {
    display.init(115200);
}

void screen_show_loading_screen() {
  display.setRotation(1);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);            // tło
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(5, 30);
    display.print("System Obslugi Slimaka");
    display.setTextColor(GxEPD_RED);
    display.setCursor(10, 60);
    display.setFont(&FreeMonoOblique18pt7b);
    display.print("\"Slimaczek\"");
    display.setFont(&FreeMonoOblique12pt7b);
    display.setCursor(10, 90);
    display.print("Ladowanie...");

  } while (display.nextPage());
}

void screen_show_mainscreen(){
  display.setRotation(1);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);            // tło
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(5, 10);
    display.print("Powietrze:");
    display.setCursor(10, 20);
    display.setFont(&FreeMono9pt7b);
    display.print("Temperatura:");
    display.setFont(&FreeMono9pt7b);
    display.setCursor(10, 30);
    display.print("Wilgotnosc:");

  } while (display.nextPage());
}
void screen_update_humidity(float temp) {

}
void screen_update_airtemp(float temp) {

}
void screen_update_thermostat_sensor_temp(float temp) {

}
void screen_update_zadana_temp(float temp) {

}
void screen_grzanie(bool ssnaksn){

}
void screen_wlaczone(bool ajksdans){

}
void wyjeba(String blad) {
  display.setRotation(1);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_RED);            // tło
    display.setTextColor(GxEPD_WHITE);
    display.setFont(&FreeMonoOblique18pt7b);
    display.setCursor(5, 30);
    display.print(blad);
  } while (display.nextPage());
}
