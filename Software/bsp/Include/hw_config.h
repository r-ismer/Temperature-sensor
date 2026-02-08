// **********************************************************************************************************
// File name		: hw_config.h                                                                           *
// Author           : Richard I.                                                                            *
// Date				: 30/01/2026                                                                            *
// Description		: Hardware configuration file.                                                          *
// **********************************************************************************************************

# ifndef _HW_CONFIG_H_
# define _HW_CONFIG_H_

// **********************************************************************************************************
//                                               Include                                                    *
// **********************************************************************************************************
#include "main.h"

// **********************************************************************************************************
//                                               Defines                                                    *
// **********************************************************************************************************
// ********************************************** GPIO ******************************************************
// Temperature and huidity sensor
#define TEMP_HUM_SDA_PIN                        GPIO_PIN_10
#define TEMP_HUM_SDA_PORT                       GPIOA

#define TEMP_HUM_SCL_PIN                        GPIO_PIN_9
#define TEMP_HUM_SCL_PORT                       GPIOA

// UART commuication 
#define COM_UART_TX_PIN                         GPIO_PIN_2
#define COM_UART_TX_PORT                        GPIOA

#define COM_UART_RX_PIN                         GPIO_PIN_3
#define COM_UART_RX_PORT                        GPIOA

// ********************************************* TIMER ******************************************************
#define TIM                                     TIM1
#define TIM_PRESCALER                           (59999u)
#define TIM_PERIOD                              (40000u)

// ********************************************** I2C *******************************************************
// Temperature and humidity sensor
#define TEMP_HUM_SENSOR                         I2C1
#define TEMP_HUM_SENSOR_TIMING                  (0x00201D2B)

// ********************************************** UART ******************************************************
// Communication UART
#define COMMUNICATION_UART                      USART1
#define COMMUNICATION_UART_BAUDRATE             (9600u)

// ******************************************* INTERRUPT ****************************************************
// Timer interrupt
#define TIM_IT_IRQ                              TIM1_BRK_UP_TRG_COM_IRQn
#define TIM_IT_IRQ_HANDLER                      TIM1_BRK_UP_TRG_COM_IRQHandler
#define TIM_UP_CALLBACK                         HAL_TIM_PeriodElapsedCallback

// **********************************************************************************************************
//                                           Public variables                                               *
// **********************************************************************************************************
// Global hw handle(s)
extern I2C_HandleTypeDef ge_hw_i2c_handle;
extern UART_HandleTypeDef ge_hw_uart_handle;
extern TIM_HandleTypeDef ge_hw_tim_handle;

// **********************************************************************************************************
//                                            Public fuctions                                               *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name	: hw_config                                                                             *
// Description		: Hardware configuration function.                                                      *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
void hw_config(void);

# endif // _HW_CONFIG_H_
