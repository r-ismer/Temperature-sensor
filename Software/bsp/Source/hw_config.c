// **********************************************************************************************************
// File name		: hw_config.h                                                                           *
// Author           : Richard I.                                                                            *
// Date				: 30/01/2026                                                                            *
// Description		: Hardware configuration file.                                                          *
// **********************************************************************************************************
// **********************************************************************************************************
//                                               Include                                                    *
// **********************************************************************************************************
# include "hw_config.h"

// **********************************************************************************************************
//                                               Defines                                                    *
// **********************************************************************************************************

// **********************************************************************************************************   
//                                              Variables                                                   *
// **********************************************************************************************************
// Global hw handle(s)
I2C_HandleTypeDef ge_hw_i2c_handle;
UART_HandleTypeDef ge_hw_uart_handle;
TIM_HandleTypeDef ge_hw_tim_handle;

// **********************************************************************************************************
//                                       Private fuctions prototype                                         *
// **********************************************************************************************************     
// **********************************************************************************************************   
// Function name    : rcc_config                                                                            *
// Description      : RCC configuration function.                                                           *
// Argument         : None                                                                                  *   
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void rcc_config(void);

// **********************************************************************************************************
// Function name    : gpio_config                                                                           *
// Description      : GPIO configuration function.                                                          *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void gpio_config(void);

// **********************************************************************************************************
// Function name    : tim_config                                                                            *
// Description      : Timer configuration function.                                                         *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void tim_config(void);

// **********************************************************************************************************
// Function name    : i2c_config                                                                            *
// Description      : I2C configuration function.                                                           *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void i2c_config(void);

// **********************************************************************************************************
// Function name    : uart_config                                                                           *
// Description      : UART configuration function.                                                          *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void uart_config(void);

// **********************************************************************************************************
// Function name    : nvic_config                                                                           *
// Description      : NVIC configuration function.                                                          *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void nvic_config(void);

// **********************************************************************************************************
//                                            Public fuctions                                               *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name	: hw_config                                                                             *
// Description		: Hardware configuration function.                                                      *
// **********************************************************************************************************
void hw_config(void)
{
    // Configure RCC
    rcc_config();

    // Configure GPIO
    gpio_config();

    // Configure timer
    tim_config();

    // Configure I2C
    i2c_config();

    // Configure UART
    uart_config();

    // Configure NVIC
    nvic_config();
}

// **********************************************************************************************************   
//                                            Private fuctions                                              *
// **********************************************************************************************************
// **********************************************************************************************************
// Function name    : rcc_config                                                                            *
// Description      : RCC configuration function.                                                           *
// **********************************************************************************************************       
static void rcc_config(void)
{
    // Variable(s) decaration
    RCC_OscInitTypeDef rcc_init_struct = {0};
    RCC_ClkInitTypeDef clk_init_struct = {0};
    RCC_PeriphCLKInitTypeDef periph_init_struct = {0};

    // Initialize the oscillator
    rcc_init_struct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    rcc_init_struct.HSIState = RCC_HSI_ON;
    rcc_init_struct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    rcc_init_struct.PLL.PLLState = RCC_PLL_ON;
    rcc_init_struct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    rcc_init_struct.PLL.PLLMUL = RCC_PLL_MUL12;
    rcc_init_struct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_OK != HAL_RCC_OscConfig(&rcc_init_struct))
    {
        // Catch error
        error_handler();
    }

    // Initialize the CPU, AHB and APB buses clocks
    clk_init_struct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    clk_init_struct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk_init_struct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk_init_struct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_OK != HAL_RCC_ClockConfig(&clk_init_struct, FLASH_LATENCY_1))
    {
        // Catch error
        error_handler();
    }

    // Select the clocks for the peripherals
    periph_init_struct.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_I2C1;
    periph_init_struct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    periph_init_struct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
    if (HAL_OK != HAL_RCCEx_PeriphCLKConfig(&periph_init_struct))
    {
        // Catch error
        error_handler();
    }

    // Enable PWR clock for sleep mode functionality
    __HAL_RCC_PWR_CLK_ENABLE();
}

// **********************************************************************************************************
// Function name    : gpio_config                                                                           *
// Description      : GPIO configuration function.                                                          *
// **********************************************************************************************************
static void gpio_config(void)
{
    // Variable(s) declaration
    GPIO_InitTypeDef gpio_init_struct = {0};

    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Temperature and humidity sensor
    gpio_init_struct.Mode = GPIO_MODE_AF_OD;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = GPIO_AF4_I2C1;
    gpio_init_struct.Pin = TEMP_HUM_SDA_PIN;
    HAL_GPIO_Init(TEMP_HUM_SDA_PORT, &gpio_init_struct);
    gpio_init_struct.Pin = TEMP_HUM_SCL_PIN;
    HAL_GPIO_Init(TEMP_HUM_SCL_PORT, &gpio_init_struct);
    
    // UART communication
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = GPIO_AF1_USART1;
    gpio_init_struct.Pin = COM_UART_TX_PIN;
    HAL_GPIO_Init(COM_UART_TX_PORT, &gpio_init_struct);
    gpio_init_struct.Pin = COM_UART_RX_PIN; 
    HAL_GPIO_Init(COM_UART_RX_PORT, &gpio_init_struct);
}

// **********************************************************************************************************
// Function name    : tim_config                                                                            *
// Description      : Timer configuration function.                                                         *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void tim_config(void)
{
    // Variable(s) delcaration
    TIM_ClockConfigTypeDef clk_config = {0};
    TIM_MasterConfigTypeDef master_config = {0};

    // Enable timer clock
    __HAL_RCC_TIM1_CLK_ENABLE();

    // Initialize the timer handle
    ge_hw_tim_handle.Instance = TIM;
    ge_hw_tim_handle.Init.Prescaler = TIM_PRESCALER;
    ge_hw_tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    ge_hw_tim_handle.Init.Period = TIM_PERIOD;
    ge_hw_tim_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    ge_hw_tim_handle.Init.RepetitionCounter = 0u;
    ge_hw_tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_OK != HAL_TIM_Base_Init(&ge_hw_tim_handle))
    {
        // Catch error
        error_handler();
    }

    // Configure the clock source
    clk_config.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_OK != HAL_TIM_ConfigClockSource(&ge_hw_tim_handle, &clk_config))
    {
        // Catch error
        error_handler();
    }

    // Configure the master mode
    master_config.MasterOutputTrigger = TIM_TRGO_RESET;
    master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_OK != HAL_TIMEx_MasterConfigSynchronization(&ge_hw_tim_handle, &master_config))
    {
        // Catch error
        error_handler();
    }

    // Freeze the timer in debug mode
    __HAL_FREEZE_TIM1_DBGMCU();
}

// **********************************************************************************************************
// Function name    : i2c_config                                                                            *
// Description      : I2C configuration function.                                                           *
// **********************************************************************************************************
static void i2c_config(void)
{
    // Enable I2C clock
    __HAL_RCC_I2C1_CLK_ENABLE();

    // Initialize I2C handle
    ge_hw_i2c_handle.Instance = TEMP_HUM_SENSOR;
    ge_hw_i2c_handle.Init.Timing = TEMP_HUM_SENSOR_TIMING;
    ge_hw_i2c_handle.Init.OwnAddress1 = 0;
    ge_hw_i2c_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    ge_hw_i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    ge_hw_i2c_handle.Init.OwnAddress2 = 0;
    ge_hw_i2c_handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    ge_hw_i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    ge_hw_i2c_handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_OK != HAL_I2C_Init(&ge_hw_i2c_handle))
    {
        // Catch error
        error_handler();
    }

    // Configure the analogue filter
    if (HAL_OK != HAL_I2CEx_ConfigAnalogFilter(&ge_hw_i2c_handle, I2C_ANALOGFILTER_ENABLE))
    {
        // Catch error
        error_handler();
    }

    // Configure the digital filter
    if (HAL_OK != HAL_I2CEx_ConfigDigitalFilter(&ge_hw_i2c_handle, 0))
    {
        // Catch error
        error_handler();
    }
}

// **********************************************************************************************************
// Function name    : uart_config                                                                           *
// Description      : UART configuration function.                                                          *
// **********************************************************************************************************
static void uart_config(void)
{
    // Enable UART clock
    __HAL_RCC_USART1_CLK_ENABLE();

    // Initialize UART handle
    ge_hw_uart_handle.Instance = COMMUNICATION_UART;
    ge_hw_uart_handle.Init.BaudRate = COMMUNICATION_UART_BAUDRATE;
    ge_hw_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    ge_hw_uart_handle.Init.StopBits = UART_STOPBITS_1;
    ge_hw_uart_handle.Init.Parity = UART_PARITY_NONE;
    ge_hw_uart_handle.Init.Mode = UART_MODE_TX_RX;
    ge_hw_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    ge_hw_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    ge_hw_uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    ge_hw_uart_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_OK != HAL_UART_Init(&ge_hw_uart_handle))
    {
        // Catch error
        error_handler();
    }
}

// **********************************************************************************************************
// Function name    : nvic_config                                                                           *
// Description      : NVIC configuration function.                                                          *
// Argument         : None                                                                                  *
// Return value     : None                                                                                  *
// **********************************************************************************************************
static void nvic_config(void)
{
    // Enable IRQ for timer
    HAL_NVIC_SetPriority(TIM_IT_IRQ, 2, 0);
    HAL_NVIC_EnableIRQ(TIM_IT_IRQ);
}
