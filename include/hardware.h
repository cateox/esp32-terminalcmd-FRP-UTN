#ifndef HARDWARE_H
#define HARDWARE_H
/**@file hardware.h
 * @brief Interfaz para el control del hardware del LED.
 *
 * Este archivo declara las funciones necesarias para inicializar
 * y controlar el LED conectado al ESP32 mediante GPIO.
 */


/** @brief Inicializa el hardware del LED.
*
*	Configura el pin del GPIO asociado al LED como salida digital y
*	establece su estado inicial en apagado.
*
*/
void hardware_init(void);

/** @brief Enciende el LED.
*
*	Establece un nivel lógico alto sobre el pin GPIO configurado
* para el LED.
*
*/
void ledOn(void);

/** @brief Apaga el LED.
 *
 * Establece un nivel lógico bajo sobre el pin GPIO configurado
 * para el LED.
 */
void ledOff(void);

/** @brief Ejecuta una secuencia de parpadeo del LED.
*
*	Realiza tres ciclos consecutivos de encedido y apagado del LED,
* manteniendo cada estado durante aproximadamente 300 [ms].
*
*/
void ledBlink3(void);

#endif