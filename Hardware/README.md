# Temperature-sensor

## Overview
This project contains the KiCad files for a temperature/humidity sensor.

The temperature and humidity sensor communicates with the STM32F030 thourgh I2C. 

The  programming of the microcontroller can be done through a 19 pin JTAG interface using SWD. 

![alt text](https://github.com/r-ismer/Temperature-sensor/blob/main/board_3D.png)

## Setup and build

KiCad 9 was used to design the schematic and the PCB. To open the project clone the repository or download the files and open it with KiCad 9.

## Next steps

The next steps are the soldering of all components on the board. Once this is done, the programming part can begin. This consists of: 
- Creating a driver for the SHT4
- Programming the STM32F0 using  the driver