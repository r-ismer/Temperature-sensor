// **********************************************************************************************************
// File name         : sht4x_driver.h                                                                       *
// Author            : Richard I.                                                                           *
// Date              : 30/01/2026                                                                           *
// Description       : SHT4x sensor driver                                                                  *
// **********************************************************************************************************
# ifndef _SHT4X_DRIVER_H_
# define _SHT4X_DRIVER_H_

// **********************************************************************************************************
//                                               Include                                                    *
// **********************************************************************************************************
#include "definitions.h"

// **********************************************************************************************************
//                                               Defines                                                    *
// **********************************************************************************************************
// Structure forware declaration
typedef struct sht4x_handle_s sht4x_handle_t;

// Sensor interface address
typedef enum
{
    SHT4x_A = 0u,
    SHT4x_B,
    SHT4x_C,
} sht4x_address_e;

// Measure precision
typedef enum
{
    SHT4x_PRECISION_LOW = 0u,
    SHT4x_PRECISION_MEDIUM,
    SHT4x_PRECISION_HIGH,
} sht4x_precision_e;

// Heater power
typedef enum
{
    SHT4x_HEATER_POWER_20 = 0u,
    SHT4x_HEATER_POWER_110,
    SHT4x_HEATER_POWER_200,
} sht4x_heater_power_e;

// Heater duration
typedef enum
{
    SHT4x_HEATER_DURATION_0_1SEC = 0u,
    SHT4x_HEATER_DURATION_1_0SEC,
} sht4x_heater_duration_e;

// Send and receive function pointer types
// **********************************************************************************************************
// Function name    : sht4x_send_function                                                                   *
// Description      : Send function pointer type definition.                                                *
// Argument         : (uint8_t) i_address: I2C device address                                               *
//                  : (uint8_t*) i_p_data: Pointer to data to send                                          *
//                  : (size_t) i_size: Size of data to send                                                 *
// Return value     : (status_e) : Status of the operation                                                  *
// **********************************************************************************************************
typedef status_e (*sht4x_send_function)(uint8_t i_address, uint8_t* i_p_data, size_t i_size);

// **********************************************************************************************************
// Function name    : sht4x_receive_function                                                                *
// Description      : Receive function pointer type definition.                                             *
// Argument         : (uint8_t) i_address: I2C device address                                               *
//                  : (uint8_t*) o_p_data: Pointer to data to receive                                       *
//                  : (size_t) i_size: Size of data to receive                                              *
// Return value     : (status_e) : Status of the operation                                                  *
// **********************************************************************************************************
typedef status_e (*sht4x_receive_function)(uint8_t i_address, uint8_t* o_p_data, size_t i_size);

// **********************************************************************************************************
// Function name    : sht4x_delay                                                                           *
// Description      : Delay function pointer type definition.                                               *
// Argument         : (uint32_t) i_delay_ms: Delay in milliseconds                                          *
// Return value     : None                                                                                  *
// **********************************************************************************************************
typedef void (*sht4x_delay)(uint32_t i_delay_ms);

// **********************************************************************************************************
//                                           Public fuctions                                                *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : sht4x_init                                                                            *
// Description      : Initialize the SHT4x sensor driver.                                                   *
// Argument         : (sht4x_address) i_address: Sensor I2C address                                         *
//                  : (sht4x_send_function) i_send_function: Pointer to the send function                   *
//                  : (sht4x_receive_function) i_receive_function: Pointer to the receive function          *
//                  : (sht4x_delay) i_delay_function: Pointer to the delay function                         *
// Return value     : (sht4x_handle_t*) : Pointer to the sensor handle structure                            *
// **********************************************************************************************************
sht4x_handle_t* sht4x_init(sht4x_address_e i_address, 
                           sht4x_send_function i_send_function, 
                           sht4x_receive_function i_receive_function,
                           sht4x_delay i_delay_function);

// **********************************************************************************************************
// Function name    : sht4x_get_serial_number                                                               *
// Description      : Get the sensor serial number.                                                         *
// Argument         : (sht4x_handle_t*) i_p_handle: Pointer to the sensor handle structure                  *
//                  : (uint32_t*) o_p_serial_number: Pointer to the serial number value                     *  
// Return value     : (status_e) : Status of the operation                                                  *
// **********************************************************************************************************
status_e sht4x_get_serial_number(sht4x_handle_t* i_p_handle, uint32_t* o_p_serial_number);

// **********************************************************************************************************
// Function name    : sht4x_soft_reset                                                                      *
// Description      : Perform a soft reset of the sensor.                                                   *
// Argument         : (sht4x_handle_t*) i_p_handle: Pointer to the sensor handle structure                  *
// Return value     : (status_e) : Status of the operation                                                  *
//  *********************************************************************************************************
status_e sht4x_soft_reset(sht4x_handle_t* i_p_handle);

// **********************************************************************************************************
// Function name    : sht4x_read_temperature_humidity                                                       *
// Description      : Read temperature and humidity from the sensor.                                        *
// Argument         : (sht4x_handle_t*) i_p_handle: Pointer to the sensor handle structure                  *
//                  : (sht4x_precision_e) i_precision: Measurement precision                                *
//                  : (int16_t*) o_p_temperature: Pointer to the temperature value (in 0.1 degree Celsius)  *
//                  : (uint16_t*) o_p_humidity: Pointer to the humidity value (in 0.1 %RH)                  *
// Return value     : (status_e) : Status of the operation                                                  *
// **********************************************************************************************************
status_e sht4x_read_temperature_humidity(sht4x_handle_t* i_p_handle, 
                                         sht4x_precision_e i_precision, 
                                         int16_t* o_p_temperature, 
                                         uint16_t* o_p_humidity);

// **********************************************************************************************************   
// Function name    : sht4x_read_temperature_humidity_heater                                                *
// Description      : Read temperature and humidity from the sensor with heater enabled.                    *
// Argument         : (sht4x_handle_t*) i_p_handle: Pointer to the sensor handle structure                  *
//                  : (sht4x_heater_power_e) i_heater_power: Heater power level                            *
//                  : (sht4x_heater_duration_e) i_heater_duration: Heater duration                          *
//                  : (int16_t*) o_p_temperature: Pointer to the temperature value (in 0.1 degree Celsius)  *
//                  : (uint16_t*) o_p_humidity: Pointer to the humidity value (in 0.1 %RH)                  *
// Return value     : (status_e) : Status of the operation                                                  *
// **********************************************************************************************************
status_e sht4x_read_temperature_humidity_heater(sht4x_handle_t* i_p_handle, 
                                                sht4x_heater_power_e i_heater_power,
                                                sht4x_heater_duration_e i_heater_duration,
                                                int16_t* o_p_temperature, 
                                                uint16_t* o_p_humidity);

# endif // _SHT4X_DRIVER_H_