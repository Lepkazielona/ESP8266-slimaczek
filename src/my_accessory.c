#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}


//DT22 TEMP
homekit_characteristic_t air_temperature = HOMEKIT_CHARACTERISTIC_(
    CURRENT_TEMPERATURE, 1
);

//DT22 HUMIDITY
homekit_characteristic_t air_humidity = HOMEKIT_CHARACTERISTIC_(
    CURRENT_RELATIVE_HUMIDITY, 1
);


//THERMOSTAT SETTINGS
//homekit_characteristic_t heating_toggle = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t current_heating_cooling_state = HOMEKIT_CHARACTERISTIC_(
    CURRENT_HEATING_COOLING_STATE, 0 // 0=nieaktywny, 1=ogrzewanie
);

homekit_characteristic_t target_heating_cooling_state = HOMEKIT_CHARACTERISTIC_(
    TARGET_HEATING_COOLING_STATE, 0,
    .valid_values = {
        .count = 2,
        .values = (uint8_t[]){0,1}
    }
);


homekit_characteristic_t current_temperature = HOMEKIT_CHARACTERISTIC_(
    CURRENT_TEMPERATURE,
     1
);
homekit_characteristic_t target_temperature = HOMEKIT_CHARACTERISTIC_(
    TARGET_TEMPERATURE,
     20,  // Domyślna temperatura docelowa: 20°C
    .min_value = (float[]) {10},  // Minimalna temperatura
    .max_value = (float[]) {30},  // Maksymalna temperatura
);
    
homekit_characteristic_t thermostat_threshold = HOMEKIT_CHARACTERISTIC_(
    HEATING_THRESHOLD_TEMPERATURE,
    2);


//------------------------------------

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_thermostat, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "System Obslugi Slimaka"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Mykoaj CORP"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "42069"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(THERMOSTAT, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Ogrzewanie"),
            &current_temperature,
            &target_temperature,
            &current_heating_cooling_state,
            &target_heating_cooling_state,
            NULL
        }),
        HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Temperatura Powietrza"),
            &air_temperature,
            NULL
        }),
        HOMEKIT_SERVICE(HUMIDITY_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Wilgotnosc Powietrza"),
            &air_humidity,
            NULL
        }),
        NULL
    }),
    NULL
};


homekit_server_config_t config = {
    .accessories = accessories,
    .password = "111-11-111"
};