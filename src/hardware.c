#include "hardware.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/** @brief Definimos el pin del LED 
*/
#define ledPin GPIO_NUM_2
/** @file hardware.c
* @brief Implemetación de las funciones de control del ESP32.
*
* 	Este módulo implementa la inicialización y el control del LED 
* conectado al ESP32 mediante la interfaz GPIO proporcionada por 
* ESP-IDF.
*
*	Otorga los métodos para:
* - Inicializar el periférico GPIO.
* - Encender el LED.
* - Apagar el LED.
* - Ejecutar la secuencia de parpadeo determinada.
*
*/
/** @brief Inicializa el hardware del LED.
*
*	Configura el pin GPIO asociado al LED como salida digital,
* deshabilita las resistencias internas de pull-up y pull-down,
* desactiva las interrupciones y establece el LED inicialmente 
* en estado apagado. 
*
*/
void hardware_init(void){
    gpio_config_t gpioLed = {
        .pin_bit_mask = 1ULL << ledPin,
        .mode = GPIO_MODE_DEF_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };   
    // Configuramos usando el chequeo de errores nativo 
    ESP_ERROR_CHECK(gpio_config(&gpioLed));
    ledOff(); // Arranca apagado
}

/** @brief Enciende el LED.
*
* 	Establece un nivel lógico alto sobre el pin GPIO 
* asociado al LED.
*
*/
void ledOn(void){
    ESP_ERROR_CHECK(gpio_set_level(ledPin, 1));
}

/** @brief Apaga el LED.
*
*	Establece un nivel lógico bajo sobre el pin GPIO asociado
*al LED.
*
*/
void ledOff(void){
    ESP_ERROR_CHECK(gpio_set_level(ledPin, 0));
}

/** @brief Ejecuta una secuencia de tres parpadeos del LED.
*
* 	Realiza tres ciclos consecutivos de encendido y apagado del LED,
* manteniendo cada estado por aproximadamente 300 [ms].
*
*/
void ledBlink3(void){
    for (int i = 0; i < 3; i++) {
        ledOn();
        vTaskDelay(300 / portTICK_PERIOD_MS); // 300 ms encendido
        ledOff();
        vTaskDelay(300 / portTICK_PERIOD_MS); // 300 ms apagado
    }
}

