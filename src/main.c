#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <driver/uart.h>
#include <driver/uart_vfs.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "hardware.h"
/** @file main.c
 * @brief Comunicación por consola de comandos UART para ESP32.
 *
 * En este codigo implementariamos una consola de comando que recibirar el microcontrolador ESP32 WROOM
 *	Será implementado en Visual Studio Code, con la extensión de PlatformIO y el firmware de Espresiff
 *	A continuación desarrollaremos cada método en fin de lograr entender su implementación.
 *
 * Los comandos disponibles permiten controlar un LED y realizar
 * operaciones aritméticas simples.
 *
	Definición del tipo puntero a función para los comandos */

/** @brief Tipo de puntero a método para utilizar la consola de comandos.
 *
 * Cada comando recibe una cantidad de argumentos y un vector de strings.
 *
 * @param argc Cantidad de argumentos a ingresar.
 * @param argv Vector de argumentos a ingresar.
 *
 * @return 0 si el comando se ejecuta correctamente, distinto de 0 si falla.
 */
}
typedef int (*pAction)(int, char **);

/**@brief Entrada de la tabla de comandos.
 *
 * Asocia una palabra clave con una método ejecutable y un texto de ayuda.
 * Estructura de comandos */
 
typedef struct {
    pAction act;
    const char *keyWord;
    const char *help;
} cmd;
/** @brief Prototipos de las funciones a implementar en el ESP32.
 *
 * @brief 	int help(int argc, char *argv[]);
 * @code 
 *	help
 * @endcode
 
 * 	Al llamar a este método por consola de comandos, este brindará información
 *	con tal de describir todos los métodos disponibles para utilizar.
 *
 * @brief 	int sumar (int argc, char *argv[]);
 * @code 
 *	sumar a b 
 * @endcode
 *	
 *	Este método suma dos enteros que el usuario ingrese seguido del nombre del método,
 * 	como si se tratara de llamar una función en cmd de Windows.
 * 
 * @brief 	int restar (int argc, char *argv[]);
 *@code 
 *	restar a b 
 *@endcode
 *
 *	Resta dos enteros ingresados por el usuario en la consola de comandos.
 *	Muestra por pantalla los resultados.
 *
 *@brief 	int mult(int argc, char *argv[]);
 *@code
 *	mult a b 
 *@endcode
 *
 *	A fin realizar las operaciones aritméticas mas elementales, implementamos un método
 * el cual multiplica dos enteros ingresador por el usuario. El resultado se demuestra por pantalla
 *
 *@brief	int cmdLedOn(int argc, char * argv[]);
 *@code
 *	ledOn
 *@endcode 
 *
 *	En este caso de llamada a método, no utilizaremos el comando on/off, solamente utilizaremos
 * la palabra clave con tal de prender el LED del ESP32, sin llamar ningun argumento aparte.
 *
 *@brief 	int cmdLedOff(int argc, char * argv[]);
 *@code
 *	ledOff
 *@endcode 
 *
 *	De la misma manera que el método anterior, llamamos a un método diferente con tal de apagar
 * el led utilizado.
 *
 *@brief 	int cmdLedParam(int argc, char * argv[]);
 *@code
 *	led on
 *	led off
 *@endcode
 *
 *	Este comando interpreta el segundo argumento ingresado por le usuario.
 * En caso de que fuera "on", se enciende el LED. En caso de que sea "off",
 * el LED se apaga. Cualquier otro argumento es invalido.
 *
 *
 *@brief int cmdLedBlink(int argc, char * argv[]);
 *@code
 *	blink
 *@endcode
 *
 *	Activa una secuencia de tres parpadeos consecutivos del LED.
 * Utilizaremos una rutina de control del LED de 300 [ms]
 *
 */
int help(int argc, char * argv[]);
int sumar (int argc, char *argv[]);
int restar (int argc, char *argv[]);
int mult(int argc, char *argv[]);
int cmdLedOn(int argc, char * argv[]);
int cmdLedOff(int argc, char * argv[]);
int cmdLedParam(int argc, char * argv[]);
int cmdLedBlink(int argc, char * argv[]);

/** @brief Tabla de comandos de la terminal. 
*
*	Contiene toda la asociación entre las palabras clave de los métodos a utilizar
* ingresados por el usuario, el método encargado de ejecutar el comando y su descripción,
* utilizada por el comando help para poder mostrar la descripción necesaria.
*
* Cada entrada en la consola indica:
* - keyWord indicando la palabra clave.
* - act siendo el método que ejecuta el comando.
* - help siendo la descripción utilizada para la ayuda.
*	La tabla finaliza con una linea ([NULL, NULL, NULL)] indicando el fin de la lista.
*
*/
static cmd commands [] = {
    {cmdLedOn,    "ledOn",   "Enciende el led"},
    {cmdLedOff,   "ledOff",  "Apaga el led"},
    {cmdLedParam, "led",     "Enciende o apaga el led en funcion del argumento (on u off)"},
    {cmdLedBlink, "blink",   "Hace parpadear el led 3 veces (300ms On / 300ms Off)"},
    {sumar,       "sumar",   "Suma 2 enteros ingresados por el usuario. Uso: sumar [a] [b]"},
    {restar,      "restar",  "Resta 2 enteros ingresados por el usuario. Uso: restar [a] [b]"},
    {mult,        "mult",    "Multiplica 2 enteros ingresados por el usuario. Uso: mult [a] [b]"},
    {help,        "help",    "Brinda informacion sobre los comandos disponibles"},
    {NULL, NULL, NULL} // Centinela
};

/** @brief void getString(char *buffer)
*
*	Capta los caracteres ingresados por el usuario a través de la consola
* UART y los almacena en el buffer indicado hasta recibir un salto de línea
*(\n). Al finalizar la lectura, agrega el caracter nulo (\0) para formar el 
*string válido en C.
*
*@note La función bloquea el buffer esperando a que el usuario ingrese 
*  Enter.
*
*@warning No verifica el tamaño del buffer. Se debe tener en cuenta la capacidad
*para evitar el desbordamiento de memoria.
*
*/
void getString(char *buffer) {
    int idx = 0;
    char c;
    while (1) {
        c = fgetc(stdin);
        if (c == 0xFF) { 
            vTaskDelay(10 / portTICK_PERIOD_MS); 
            continue;
        }
        if (c == '\n') { // Si es Enter, terminamos
            break; 
        }
        buffer[idx++] = c; // Guardamos la letra
    }
    buffer[idx] = '\0'; // Cerramos el string
}

/** @brief 	void app_main(void)
*
*	Inicializa los recursos necesarios para que se pueda ejecutar el programa,
* incluyendo la interfaz UART y el hardware del ESP32. Seguido, implementa
* un bucle el cual recibe comandos desde la terminal serie, los interpreta
* y ejecuta el método asociado mediante la tabla de comandos.
*
* 	El procesamiento de comandos se realiza de manera secuencial hasta que el
* programa es reiniciado o apagado.
*
* @note
 * Esta función constituye el punto de entrada de la aplicación en
 * ESP-IDF y es invocada automáticamente por el sistema operativo
 * FreeRTOS durante el inicio del firmware.
 *
*/
void app_main(void) {
    // Inicialización del driver UART y habilitación de VFS 
    uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);
    uart_vfs_dev_use_driver(UART_NUM_0);
    
    // Inicializamos el hardware del LED
    hardware_init();
    vTaskDelay(pdMS_TO_TICKS(1000)); // Espera de cortesía para estabilizar la consola
    
    printf("\n=== Terminal de Comandos Iniciada (PlatformIO) ===\n");

    char line[120];
    int argc;
    int k;
    char * argv[60];
    bool cmdOk;

    while(1) {
        printf("[*]> ");
        setbuf(stdin, NULL);
        fflush(stdin);
        
        // Capturamos lo que escribe el usuario
        getString(line);

        argc = 1;
        argv[0] = line;
        unsigned int lineLength = strlen(line);
        for(k = 0; (k < lineLength) && (argc < 60); k++){
            if(line[k] == ' '){
                line[k] = '\0';
                argv[argc] = line + k + 1;
                argc++;
            }
        }

        cmdOk = false;
        k = 0;
        while(commands[k].act){
            if(strcmp(commands[k].keyWord, argv[0]) == 0){
                cmdOk = true;
                if(commands[k].act(argc, argv)) {
                    printf(" [%s Fail]\n", argv[0]);
                }
                break;
            }
            k++;
        }
        if(!cmdOk && strlen(argv[0])) {
            printf("Comando inexistente\n");
        }
    }
}

int cmdLedOn(int argc, char * argv[]){
    (void)argc; (void)argv;
    ledOn();
    printf("LED Encendido.\n");
    return 0;
}

int cmdLedOff(int argc, char * argv[]){
    (void)argc; (void)argv;
    ledOff();
    printf("LED Apagado.\n");
    return 0;
}

int cmdLedParam(int argc, char * argv[]){
    if (argc == 2) {
        if (strcmp(argv[1], "on") == 0) {
            ledOn();
            printf("LED Encendido por argumento.\n");
            return 0;
        } else if (strcmp(argv[1], "off") == 0) {
            ledOff();
            printf("LED Apagado por argumento.\n");
            return 0;
        }
    }
    printf("Argumento invalido. Uso: led on | led off\n");
    return -1;
}

int cmdLedBlink(int argc, char * argv[]){
    (void)argc; (void)argv;
    printf("Iniciando parpadeo de 3 ciclos...\n");
    ledBlink3();
    printf("Parpadeo terminado.\n");
    return 0;
}

int help(int argc, char * argv[]){
    (void)argc; (void)argv;
    int k = 0;
    printf("\nComandos disponibles:\n");
    while(commands[k].act){
        printf("[%12s] : %s\n", commands[k].keyWord, commands[k].help);
        k++;
    }
    return 0;
}

int sumar(int argc, char * argv[]){
    float v1;
    float v2;
    int ret;
    if (argc == 3){
        sscanf(argv[1], "%f", &v1);
        sscanf(argv[2], "%f", &v2);
        printf("%.2f + %.2f = %.2f\n", v1, v2, v1 + v2);
        ret = 0;
    }
    else{
        printf("Cantidad de argumentos incorrecta\n");
        ret = -1;
    }
    return ret;
}

int restar(int argc, char * argv[]){
    float v1;
    float v2;
    int ret;
    if (argc == 3){
        sscanf(argv[1], "%f", &v1);
        sscanf(argv[2], "%f", &v2);
        printf("%.2f - %.2f = %.2f\n", v1, v2, v1 - v2);
        ret = 0;
    }
    else{
        printf("Cantidad de argumentos incorrecta\n");
        ret = -1;
    }
    return ret;
}

int mult(int argc, char* argv[]){
    float v1;
    float v2;
    int ret;
    if(argc == 3){
        sscanf(argv[1], "%f", &v1);
        sscanf(argv[2], "%f", &v2);
        printf("%.2f * %.2f = %.2f\n", v1, v2 ,v1*v2);
        ret = 0;
    }
    else {
        printf("Cantidad de argumentos incorrecta.\n");
        ret = -1;
    }
    return ret;
}