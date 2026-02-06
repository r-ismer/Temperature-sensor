// **********************************************************************************************************
// File name     : task.c                                                                                   * 
// Author        : Richard I.                                                                               *
// Date          : 03/02/2026                                                                               *
// Description   : Task executed to capture the temperature and humidity and send them                      *
// **********************************************************************************************************
// **********************************************************************************************************
//                                               Include                                                    *
// **********************************************************************************************************
#include "task.h"
#include "sht4x_driver.h"

// **********************************************************************************************************
//                                               Defines                                                    *
// **********************************************************************************************************

// **********************************************************************************************************
//                                              Variables                                                   *
// **********************************************************************************************************
// Driver handle
sht4x_handle_t* g_sht4x_handle;

// **********************************************************************************************************
//                                       Private prototype functions                                        *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : i2c_sed_function                                                                      *
// Description      : Function used to send a message over I2C                                              *
// Argument         : (uint8_t) i_address   : The address of the I2C periperhal to send to                  *
//                  : (uint8_t*) i_p_data   : The data to send                                              *
//                  : (size_t) i_size       : The size of the data to send in bytes                         *
// Return value     : (status_e)    : The status of the operation                                           *
// **********************************************************************************************************
status_e i2c_send_function(uint8_t i_address, uint8_t* i_p_data, size_t i_size);

// **********************************************************************************************************
// Function name    : i2c_receive_function                                                                  *
// Description      : Function used to receive a message over I2C                                           *
// Argument         : (uint8_t) i_address   : The address of the I2C peripheral to receive from             *
//                  : (uint8_t*) o_p_data   : A pointer on a buffer to receive the data                     *
//                  : (size_t) i_size       : The size of the data to receive in bytes                      *
// Return value     : (status_e)    : The status of the operation                                           *
// **********************************************************************************************************
status_e i2c_receive_function(uint8_t i_address, uint8_t* o_p_data, size_t i_size);

// **********************************************************************************************************
// Function name    : delay_function                                                                        *
// Description      : Function used to create a delay                                                       *
// Argument         : (uint32_t) i_delay_ms : The delay in milliseconds                                     *
// Return value     : None                                                                                  *
// **********************************************************************************************************
void delay_function(uint32_t i_delay_ms);

// **********************************************************************************************************
//                                            Public fuctions                                               *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : task_init                                                                             *
// Description      : Initialize the sendor driver                                                          *
// **********************************************************************************************************
void task_init(void)
{
    // Initialize the sensor handle
    g_sht4x_handle = sht4x_init(SHT4x_A, &i2c_send_function, &i2c_receive_function, &delay_function);
}

// **********************************************************************************************************
// Function name    : task                                                                                  *
// Description      : Task executed to capture the temperature and humidity and send them                   *
// **********************************************************************************************************
void task(void)
{
    // Variable(s) delcaration
    int16_t temperature;
    uint16_t humidity;
    uint8_t message[4];

    // Execute the task by getting the temperature and humidity
    sht4x_read_temperature_humidity(g_sht4x_handle, SHT4x_PRECISION_HIGH, &temperature, &humidity);

    // Fill the message for the UART
    message[0] = (uint8_t)  (temperature & 0x00FF);
    message[1] = (uint8_t) ((temperature >> 8) & 0x00FF);
    message[2] = (uint8_t)  (humidity & 0x00FF);
    message[3] = (uint8_t) ((humidity >> 8) & 0x00FF);

    // Send the temperature and humdity over UART
    HAL_UART_Transmit(&ge_hw_uart_handle, message, 4, 0);
}

// **********************************************************************************************************
// Function name    : i2c_sed_function                                                                      *
// Description      : Function used to send a message over I2C                                              *
// **********************************************************************************************************
status_e i2c_send_function(uint8_t i_address, uint8_t* i_p_data, size_t i_size)
{
    // Variable(s) delaration
    status_e r_status;

    // Implement the I2C send functionality here
    if (HAL_OK == HAL_I2C_Master_Transmit(&ge_hw_i2c_handle, i_address << 1, i_p_data, i_size, HAL_MAX_DELAY))
    {
        // Success: update the status
        r_status = STATUS_OK;
    }
    else
    {
        // Error: update the status
        r_status = STATUS_ERROR;
    }

    // Retrun the status of the operation
    return r_status;
}

// **********************************************************************************************************
// Function name    : i2c_receive_function                                                                  *
// Description      : Function used to receive a message over I2C                                           *
// **********************************************************************************************************
status_e i2c_receive_function(uint8_t i_address, uint8_t* o_p_data, size_t i_size)
{
    // Variable(s) declaration
    status_e r_status;

    // Implement the I2C receive functionality here
    if (HAL_OK == HAL_I2C_Master_Receive(&ge_hw_i2c_handle, i_address << 1, o_p_data, i_size, HAL_MAX_DELAY))
    {
        // Success: update the status
        r_status = STATUS_OK;
    }
    else
    {
        // Error: update the status
        r_status = STATUS_ERROR;
    }

    // Retrun the status of the operation
    return r_status;
}

// **********************************************************************************************************
// Function name    : delay_function                                                                        *
// Description      : Function used to create a delay                                                       *
// **********************************************************************************************************
void delay_function(uint32_t i_delay_ms)
{
    // Implement the delay functionality here
    HAL_Delay(i_delay_ms);
}
