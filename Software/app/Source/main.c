// **********************************************************************************************************
// File name     : main.c                                                                                   * 
// Author        : Richard I.                                                                               *
// Date          : 30/01/2026                                                                               *
// Description   : Main file                                                                                *
// **********************************************************************************************************
// **********************************************************************************************************
//                                               Include                                                    *
// **********************************************************************************************************
#include "main.h"
#include "sht4x_driver.h"

// **********************************************************************************************************
//                                               Defines                                                    *
// **********************************************************************************************************

// **********************************************************************************************************
//                                              Variables                                                   *
// **********************************************************************************************************

// **********************************************************************************************************
//                                       Private prototype functions                                        *
// **********************************************************************************************************

status_e i2c_send_function(uint8_t i_address, uint8_t* i_p_data, size_t i_size);
status_e i2c_receive_function(uint8_t i_address, uint8_t* o_p_data, size_t i_size);
void delay_function(uint32_t i_delay_ms);

// **********************************************************************************************************
//                                            Public fuctions                                               *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : main                                                                                  *
// Description      : Main function.                                                                        *
// Argument         : None                                                                                  *
// Return value     : int                                                                                   *
// **********************************************************************************************************
int main(void)
{
    // Initialize the HAL library
    HAL_Init();

    // Configure the hardware
    hw_config();

    // Main loop
    while (1)
    {
        // Get the microcontroller to sleep until an interrupt occurs
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

        sht4x_handle_t* sht4x_handle;
        status_e status;
        int16_t temperature;
        uint16_t humidity;
        // Initialize the SHT4x sensor
        sht4x_handle = sht4x_init(SHT4x_A, i2c_send_function, i2c_receive_function, delay_function);
        if (sht4x_handle != NULL)
        {
            // Read temperature and humidity
            status = sht4x_read_temperature_humidity(sht4x_handle, SHT4x_PRECISION_HIGH, &temperature, &humidity);
            if (status == STATUS_OK)
            {
                // Successfully read temperature and humidity
                // Process the temperature and humidity values as needed
            }
            else
            {
                // Error reading temperature and humidity
                error_handler();
            }
        }
        else
        {
            // Error initializing the SHT4x sensor
            error_handler();
        }
    }
}

// **********************************************************************************************************
// Function name    : error_handler                                                                         *
// Description      : Error handler function.                                                               *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
void error_handler(void)
{
    // Infinite loop
    while (1)
    {
        // Break here for debugger
        __asm("bkpt");
    }
}

// **********************************************************************************************************
// Function name    : assert_failed                                                                         *
// Description      : Assert failed function.                                                               *
// Argument         : file: Pointer to the source file name                                                 *
//                  : line: assert_param error line source number                                           *
// Return value     : None                                                                                  *
// **********************************************************************************************************
void assert_failed(uint8_t* file, uint32_t line)
{
    // Infinite loop
    while (1)
    {
        // Break here for debugger
        __asm("bkpt");
    }
}

status_e i2c_send_function(uint8_t i_address, uint8_t* i_p_data, size_t i_size)
{
    // Implement the I2C send functionality here
    HAL_I2C_Master_Transmit(&ge_hw_i2c_handle, i_address << 1, i_p_data, i_size, HAL_MAX_DELAY);

    return STATUS_OK;
}

status_e i2c_receive_function(uint8_t i_address, uint8_t* o_p_data, size_t i_size)
{
    // Implement the I2C receive functionality here
    HAL_I2C_Master_Receive(&ge_hw_i2c_handle, i_address << 1, o_p_data, i_size, HAL_MAX_DELAY);

    return STATUS_OK;
}

void delay_function(uint32_t i_delay_ms)
{
    // Implement the delay functionality here
    HAL_Delay(i_delay_ms);
}
