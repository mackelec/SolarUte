#  CANbv    

###  CAN Bus Module measures High Volt Battery Voltage and Current


![CANbv](https://github.com/mackelec/SolarUte/blob/master/CAN%20Boards/CAN_HiVoltSensor/CANbv_3d.PNG)



### Decription

This board was designed as a general purpose module using CAN Bus to enable measuring an EV Battery or DC Link Voltage, and measure Current utilising a Hall Effect sensor such LEM or HSTS21.    

This module is a compact, low cost, using the stm32F103C8 processor (same as BluePill), optimised for smd assembly by JLCPCB.


### Features

* CAN bus operated.  (with 120 ohm terminator jumper.)
* STM32F103C8 processor.  64k program memory 20k RAM.
* Arduino IDE platform. (Official)
* 48mm x 80mm
* Current transducer input and reference signal input - 12bit adc
* Voltage resolution: 100mV
* Current resolution: 100mA
  (400 Amp range transducer)
* Power reolution:    1Watt
* One CAN address, 8 byte frame contains Voltage, Current and Power Measurments.
* COM1 is not shared, found on PROG port.
* COM3 is used to communicate with the STM8.

#### High Voltage Input

* Optocoupler isolated Serial interface.
* Galvanically isolated DC-DC power supply, derived off 12v.
* STM8L051 processor 12bit ADC 
* High Voltage Input: 423 Volts max.
* Auto power off.  (STM32 must keep transmitting to maintain power-on.)
* Relay disconnects positive High Volt input from input circuit.
** (negative side of High Volt input is still connected.  The Relay removes the input circuit to revent slow discharge)




![CANbv](https://github.com/mackelec/SolarUte/blob/master/CAN%20Boards/CAN_HiVoltSensor/CANbv_001.png)


![HSTS21](https://github.com/mackelec/SolarUte/blob/master/CAN%20Boards/CAN_HiVoltSensor/HSTS21.jpeg)
