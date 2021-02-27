/*! @file : sdk_hal_gpio.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 1.0.0
 * @date    10/01/2021
 * @brief   Driver for GPIO
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"







#include "sdk_hal_gpio.h"
#include "sdk_hal_uart0.h"
#include "sdk_hal_i2c0.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MMA851_I2C_DEVICE_ADDRESS	        0x1D
#define MMA8451_WHO_AM_I_MEMORY_ADDRESS		0x0D
#define MMA8451_X_MSB_MEMORY_ADDRESS        0x01
#define MMA8451_X_LSB_MEMORY_ADDRESS        0x02
#define MMA8451_Y_MSB_MEMORY_ADDRESS        0x03
#define MMA8451_Y_LSB_MEMORY_ADDRESS        0x04
#define MMA8451_Z_MSB_MEMORY_ADDRESS        0x05
#define MMA8451_Z_LSB_MEMORY_ADDRESS        0x06

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {
	status_t status;
	uint8_t  nuevo_byte_uart;
	uint8_t  nuevo_dato_i2c;
	uint8_t  nuevo_dato_i2c_1;
	uint8_t  nuevo_dato_i2c_2;
	int16_t   Dato_Acelerometro;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200); //115200bps
    (void)i2c0MasterInit(100000);	//100kbps

    PRINTF("Usar teclado para controlar LEDs\r\n");
    PRINTF("R enciende led ROJO - r apaga led ROJO\r\n");
    PRINTF("A enciende led AZUL - a apaga led AZUL\r\n");
    PRINTF("V enciende led VERDE - v apaga led VERDE\r\n");
    PRINTF("X-x Posicion acelerometro en eje x\r\n");
    PRINTF("Y-y Posicion acelerometro en eje y\r\n");
	PRINTF("Z-z Posicion acelerometro en eje z\r\n");


    while(1) {
    	if(uart0CuantosDatosHayEnBuffer()>0){

    		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);

    		if(status == kStatus_Success){

    			printf("dato:%c\r\n", nuevo_byte_uart);

    			switch (nuevo_byte_uart) {

				case 'a':
				case 'A':

					gpioPutToggle(KPTB10);
					break;

				case 'v':
					gpioPutHigh(KPTB7);
					break;
				case 'V':
					gpioPutLow(KPTB7);
					break;

				case 'r':
					gpioPutValue(KPTB6,1);
					break;
				case 'R':
					gpioPutValue(KPTB6,0);
					break;

				case 'M':
					i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS, MMA8451_WHO_AM_I_MEMORY_ADDRESS);

					if(nuevo_dato_i2c == 0x1A)
						printf("MMA8451 encontrado!!\r\n");
					else
						printf("MMA8451 error\r\n");

					break;

				case 'X':
			  	case 'x':
				   i2c0MasterReadByte(&nuevo_dato_i2c_1, MMA851_I2C_DEVICE_ADDRESS, MMA8451_X_MSB_MEMORY_ADDRESS);
				   i2c0MasterReadByte(&nuevo_dato_i2c_2, MMA851_I2C_DEVICE_ADDRESS, MMA8451_X_LSB_MEMORY_ADDRESS);
							Dato_Acelerometro = (nuevo_dato_i2c_1 << 6 ) | (nuevo_dato_i2c_2 >> 2);
							printf("La posicion del acelerometro en el eje X es: %d\r\n", Dato_Acelerometro);
					break;
				case 'Y':
				case 'y':
				 i2c0MasterReadByte(&nuevo_dato_i2c_1, MMA851_I2C_DEVICE_ADDRESS, MMA8451_Y_MSB_MEMORY_ADDRESS);
				 i2c0MasterReadByte(&nuevo_dato_i2c_2, MMA851_I2C_DEVICE_ADDRESS, MMA8451_Y_LSB_MEMORY_ADDRESS);
							Dato_Acelerometro = (nuevo_dato_i2c_1 << 6 ) | (nuevo_dato_i2c_2 >> 2);
							printf("La posicion del acelerometro en el eje Y es: %d\r\n", Dato_Acelerometro);
					break;
				case 'Z':
				case 'z':
				 i2c0MasterReadByte(&nuevo_dato_i2c_1, MMA851_I2C_DEVICE_ADDRESS, MMA8451_Z_MSB_MEMORY_ADDRESS);
				 i2c0MasterReadByte(&nuevo_dato_i2c_2, MMA851_I2C_DEVICE_ADDRESS, MMA8451_Z_LSB_MEMORY_ADDRESS);
							Dato_Acelerometro = (nuevo_dato_i2c_1 << 6 ) | (nuevo_dato_i2c_2 >> 2);
							printf("La posicion del acelerometro en el eje Z es: %d\r\n", Dato_Acelerometro);
					break;

				}
    		}else{
    			printf("error\r\n");
    		}
    	}
    }
    return 0 ;
}
