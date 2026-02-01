// **********************************************************************************************************
// File name         : sht4x_driver.c                                                                       *
// Author            : Richard I.                                                                           *
// Date              : 30/01/2026                                                                           *
// Description       : SHT4x sensor driver                                                                  *
// **********************************************************************************************************
// **********************************************************************************************************
//                                               Include                                                    *
// **********************************************************************************************************
#include "sht4x_driver.h"
#include <stdlib.h>

// **********************************************************************************************************
//                                               Defines                                                    *
// **********************************************************************************************************
// CRC8 polynomial definition
#define SHT4X_CRC8_POLYNOMIAL                  0x31u
#define SHT4X_CRC8_INIT                        0xFFu

// Structure definition for the sensor handle
typedef struct 
{
    sht4x_address_e address;
    sht4x_send_function send_function;
    sht4x_receive_function receive_function;
    sht4x_delay delay_function;
} sht4x_handle_s;

// Address table
static const uint8_t sht4x_addresses[] = 
{
    0x44, // SHT4x_A
    0x45, // SHT4x_B
    0x46, // SHT4x_C
};

// Normal mode commands
static const uint8_t sht4x_normal_commands[] = 
{
    0xE0, // Low precision
    0xF6, // Medium precision
    0xFD, // High precision
};

// Heater mode commands 
static const uint8_t sht4x_heater_commands[][2] = 
{
    {0x15, 0x1E}, // Heater power 20mW
    {0x24, 0x2F}, // Heater power 110mW
    {0x32, 0x39}, // Heater power 200mW
};

// Serial number command
static const uint8_t sht4x_serial_number_command = 0x89;

// Soft reset command
static const uint8_t sht4x_soft_reset_command = 0x94;

// Delay 
#define SHT4X_MEASUREMENT_DELAY_MS            (10u)
#define SHT4X_HEATER_MEASUREMENT_DELAY_SHORT  (110u)
#define SHT4X_HEATER_MEASUREMENT_DELAY_LONG   (1010u)

// Conversion constants
#define SHT4X_TEMPERATURE_MULTIPLIER          (1750u)
#define SHT4X_TEMPERATURE_OFFSET              (450u)
#define SHT4X_HUMIDITY_MULTIPLIER             (1250u)
#define SHT4X_HUMIDITY_OFFSET                 (60u)

// **********************************************************************************************************
//                                      Private fuctions prototype                                          *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : sht4x_crc8_check                                                                      *
// Description      : Check the CRC8 of the data.                                                           *
// Argument         : (uint8_t*) i_p_data: Pointer to the data to check (always 2 bytes)                    *
//                  : (uint8_t) i_crc: Expected CRC value                                                   *
// Return value     : (bool_e) : TRUE if CRC matches, FALSE otherwise                                       *
// **********************************************************************************************************
static bool_e sht4x_crc8_check(uint8_t* i_p_data, uint8_t i_crc);

// **********************************************************************************************************
//                                           Public fuctions                                                *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : sht4x_init                                                                            *
// Description      : Initialize the SHT4x sensor driver.                                                   *
// **********************************************************************************************************
sht4x_handle_t* sht4x_init(sht4x_address_e i_address, 
                           sht4x_send_function i_send_function, 
                           sht4x_receive_function i_receive_function,
                           sht4x_delay i_delay)
{
    // Variable declaration
    sht4x_handle_s* r_p_handle;

    // Allocate memory for the handle
    r_p_handle = (sht4x_handle_s*) malloc(sizeof(sht4x_handle_s));

    // Check allocation
    if (r_p_handle != NULL)
    {
        // Check the input parameters
        if (i_send_function != NULL && i_receive_function != NULL && i_delay != NULL)
        {
            // Initialize the handle
            r_p_handle->address = i_address;
            r_p_handle->send_function = i_send_function;
            r_p_handle->receive_function = i_receive_function;
            r_p_handle->delay_function = i_delay;
        }
        else
        {
            // Invalid parameters: free allocated memory and return NULL
            free(r_p_handle);
            r_p_handle = NULL;
        }
    }

    // Return the newly created handle
    return (sht4x_handle_t*) r_p_handle;
}

// **********************************************************************************************************
// Function name    : sht4x_get_serial_number                                                               *
// Description      : Get the sensor serial number.                                                         *
// **********************************************************************************************************
status_e sht4x_get_serial_number(sht4x_handle_t* i_p_handle, uint32_t* o_p_serial_number)
{
    // Variable(s) declaration
    sht4x_handle_s* p_handle;
    status_e r_status;
    uint8_t command;
    uint8_t data[6];

    // Variable(s) initialization
    p_handle = (sht4x_handle_s*) i_p_handle;

    // Send the serial number command
    command = sht4x_serial_number_command;

    // Check handle validity
    if (p_handle != NULL)
    {
        // Send the command
        r_status = p_handle->send_function(sht4x_addresses[p_handle->address], &command, 1u);

        // Check status
        if (r_status == STATUS_OK)
        {
            // Receive the serial number data
            r_status = p_handle->receive_function(sht4x_addresses[p_handle->address], data, 6u);

            // Check status
            if (r_status == STATUS_OK)
            {
                // Check CRCs
                if (sht4x_crc8_check(&data[0], data[2]) && sht4x_crc8_check(&data[3], data[5]))
                { 
                    // Combine the serial number bytes
                    *o_p_serial_number = ((uint32_t)data[0] << 24u) |
                                        ((uint32_t)data[1] << 16u) |
                                        ((uint32_t)data[3] << 8u)  |
                                        ((uint32_t)data[4]);

                    // CRC are valid: update the status
                    r_status = STATUS_OK;
                }
                else
                {
                    // CRC error: update the status
                    r_status = STATUS_ERROR;
                }
            }
            else
            {
                // Return error status
                return STATUS_ERROR;
            }
        }
        else
        {
            // Return error status
            return STATUS_ERROR;
        }
    }
    else
    {
        // Invalid handle: return error status
        r_status = STATUS_ERROR;
    }

    // Return the status of the operation
    return r_status;
}

// **********************************************************************************************************
// Function name    : sht4x_soft_reset                                                                      *
// Description      : Perform a soft reset of the sensor.                                                   *
//  *********************************************************************************************************
status_e sht4x_soft_reset(sht4x_handle_t* i_p_handle)
{
    // Variable(s) declaration
    sht4x_handle_s* p_handle;
    status_e r_status;
    uint8_t command;

    // Variable(s) initialization
    p_handle = (sht4x_handle_s*) i_p_handle;

    // Check handle validity
    if (p_handle != NULL)
    {
           // Send the soft reset command
        command = sht4x_soft_reset_command;

        // Send the command
        r_status = p_handle->send_function(sht4x_addresses[p_handle->address], &command, 1u);
    }
    else
    {
        // Invalid handle: return error status
        r_status = STATUS_ERROR;
    }


    // Return the status of the operation
    return r_status;
}

// **********************************************************************************************************
// Function name    : sht4x_read_temperature_humidity                                                       *
// Description      : Read temperature and humidity from the sensor.                                        *
// **********************************************************************************************************
status_e sht4x_read_temperature_humidity(sht4x_handle_t* i_p_handle, 
                                         sht4x_precision_e i_precision, 
                                         int16_t* o_p_temperature, 
                                         uint16_t* o_p_humidity)
{
    // Variable(s) declaration
    sht4x_handle_s* p_handle;
    status_e r_status;
    uint8_t command;
    uint8_t data[6];
    uint32_t raw_temperature;
    uint32_t raw_humidity;
    int32_t temp;
    uint32_t hum;

    // Variable(s) initialization
    p_handle = (sht4x_handle_s*) i_p_handle;

    // Check handle validity
    if (p_handle != NULL)
    {
        // Send the measurement command based on the precision
        command = sht4x_normal_commands[i_precision];

        // Send the command
        r_status = p_handle->send_function(sht4x_addresses[p_handle->address], &command, 1u);

        // Check status
        if (r_status == STATUS_OK)
        {
            // Wait for measurement to complete
            p_handle->delay_function(SHT4X_MEASUREMENT_DELAY_MS);

            // Receive the measurement data
            r_status = p_handle->receive_function(sht4x_addresses[p_handle->address], data, 6u);

            // Check status
            if (r_status == STATUS_OK)
            {
                // Check CRCs
                if (sht4x_crc8_check(&data[0], data[2]) && sht4x_crc8_check(&data[3], data[5]))
                { 
                    // Combine raw temperature and humidity bytes
                    raw_temperature = ((uint32_t)data[0] << 8u) | ((uint32_t)data[1]);
                    raw_humidity    = ((uint32_t)data[3] << 8u) | ((uint32_t)data[4]);

                    // Calculate temperature in 0.1 degree Celsius
                    temp = ((int32_t)(raw_temperature * SHT4X_TEMPERATURE_MULTIPLIER) >> 16u) - 
                           SHT4X_TEMPERATURE_OFFSET;

                    // Store temperature value
                    *o_p_temperature = (int16_t) temp;

                    // Calculate humidity in 0.1 %RH
                    hum = ((int32_t)(raw_humidity * SHT4X_HUMIDITY_MULTIPLIER) >> 16u) - 
                          SHT4X_HUMIDITY_OFFSET;

                    // Crop humidity to 0-1000 (0-100.0 %RH)
                    if (hum > 1000u)
                    {
                        // Limit to 100%
                        *o_p_humidity = 1000u;
                    }   
                    else if (hum < 0u)
                    {
                        // Limit to 0%
                        *o_p_humidity = 0u;
                    }
                    else
                    {
                        // Valid humidity
                        *o_p_humidity = (uint16_t) hum;
                    }

                    // CRC are valid: update the status
                    r_status = STATUS_OK;
                }
                else
                {
                    // CRC error: update the status
                    r_status = STATUS_ERROR;
                }
            }
            else
            {
                // Return error status
                r_status = STATUS_ERROR;
            }
        }
        else
        {
            // Return error status
            r_status = STATUS_ERROR;
        }
    }
    else
    {
        // Invalid handle: return error status
        r_status = STATUS_ERROR;
    }

    // Return the status of the operation
    return r_status;
}

// **********************************************************************************************************   
// Function name    : sht4x_read_temperature_humidity_heater                                                *
// Description      : Read temperature and humidity from the sensor with heater enabled.                    *
// **********************************************************************************************************
status_e sht4x_read_temperature_humidity_heater(sht4x_handle_t* i_p_handle, 
                                                sht4x_heater_power_e i_heater_power,
                                                sht4x_heater_duration_e i_heater_duration,
                                                int16_t* o_p_temperature, 
                                                uint16_t* o_p_humidity)
{
    // Variable(s) declaration
    sht4x_handle_s* p_handle;
    status_e r_status;
    uint8_t command;
    uint8_t data[6];
    uint32_t raw_temperature;
    uint32_t raw_humidity;
    int32_t temp;
    uint32_t hum;

    // Variable(s) initialization
    p_handle = (sht4x_handle_s*) i_p_handle;

    // Check handle validity
    if (p_handle != NULL)
    {
        // Send the measurement command based on the heater power and duration
        command = sht4x_heater_commands[i_heater_power][i_heater_duration];

        // Send the command
        r_status = p_handle->send_function(sht4x_addresses[p_handle->address], &command, 1u);

        // Check status
        if (r_status == STATUS_OK)
        {
            // Wait for measurement to complete depending on heater duration
            if (i_heater_duration == SHT4x_HEATER_DURATION_0_1SEC)
            {
                // Wait for a short duration
                p_handle->delay_function(SHT4X_HEATER_MEASUREMENT_DELAY_SHORT);
            }
            else
            {
                // Wait for a long duration
                p_handle->delay_function(SHT4X_HEATER_MEASUREMENT_DELAY_LONG);
            }

            // Receive the measurement data
            r_status = p_handle->receive_function(sht4x_addresses[p_handle->address], data, 6u);

            // Check status
            if (r_status == STATUS_OK)
            {
                // Check CRCs
                if (sht4x_crc8_check(&data[0], data[2]) && sht4x_crc8_check(&data[3], data[5]))
                { 
                    // Combine raw temperature and humidity bytes
                    raw_temperature = ((uint32_t)data[0] << 8u) | ((uint32_t)data[1]);
                    raw_humidity    = ((uint32_t)data[3] << 8u) | ((uint32_t)data[4]);

                    // Calculate temperature in 0.1 degree Celsius
                    temp = ((int32_t)(raw_temperature * SHT4X_TEMPERATURE_MULTIPLIER) >> 16u) - 
                           SHT4X_TEMPERATURE_OFFSET;

                    // Store temperature value
                    *o_p_temperature = (int16_t) temp;

                    // Calculate humidity in 0.1 %RH
                    hum = ((int32_t)(raw_humidity * SHT4X_HUMIDITY_MULTIPLIER) >> 16u) - SHT4X_HUMIDITY_OFFSET;

                    // Crop humidity to 0-1000 (0-100.0 %RH)
                    if (hum > 1000u)
                    {
                        // Limit to 100%
                        hum = 1000u;
                    }
                    else if (hum < 0u)
                    {
                        // Limit to 0%
                        hum = 0u;
                    }
                    else
                    {
                        // Valid humidity
                        *o_p_humidity = (uint16_t) hum;
                    }

                    // CRC are valid: update the status
                    r_status = STATUS_OK;
                }
                else
                {
                    // CRC error: update the status
                    r_status = STATUS_ERROR;
                }
            }
            else
            {
                // Return error status
                r_status = STATUS_ERROR;
            }
        }
        else
        {
            // Return error status
            r_status = STATUS_ERROR;
        }
    }
    else
    {
        // Invalid handle: return error status
        r_status = STATUS_ERROR;
    }

    // Return the status of the operation
    return r_status;
}
    
// **********************************************************************************************************
//                                              Private fuctions                                            *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : sht4x_crc8_check                                                                      *
// Description      : Check the CRC8 of the data.                                                           *
// **********************************************************************************************************
static bool_e sht4x_crc8_check(uint8_t* i_p_data, uint8_t i_crc)
{
    // Variable(s) declaration
    bool_e r_result;
    uint8_t crc;
    uint8_t polynomial;
    uint8_t byte_index;
    uint8_t bit_index;
    uint8_t data_byte;

    // Variable(s) initialization
    crc = SHT4X_CRC8_INIT;
    polynomial = SHT4X_CRC8_POLYNOMIAL;

    // Process each byte
    for (byte_index = 0u; byte_index < 2u; byte_index++)
    {
        // Get the current byte
        data_byte = i_p_data[byte_index];
        crc ^= data_byte;

        // Process each bit
        for (bit_index = 0u; bit_index < 8u; bit_index++)
        {
            // Shift left and apply polynomial if needed
            if (crc & 0x80u)
            {
                // Shift left and XOR with polynomial
                crc = (crc << 1u) ^ polynomial;
            }
            else
            {
                // Just shift left
                crc <<= 1u;
            }
        }
    }

    // Compare calculated CRC with the provided CRC
    if (crc == i_crc)
    {
        // CRC matches
        r_result = TRUE;
    }
    else
    {
        // CRC does not match
        r_result = FALSE;
    }

    // Return the result
    return r_result;
}