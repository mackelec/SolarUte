# CANio-18

### CAN Bus module with 18 mixed Inputs & Outputs.


![CANio-18](https://github.com/mackelec/SolarUte/blob/master/CAN%20Boards/CANio-18/CANio-18.PNG)


### Decription

This board was designed as a general purpose module using CAN Bus to expand the number of I/O.  Inspired by my EV conversion project where This module will be controlling the existing Dashboard warning lights and gauges.  

This module is a compact, low cost, using the stm32F103C8 processor (same as BluePill), optimised for smd assembly by JLCPCB.
The module has 8 mosfet driven outputs, 4 analogue outputs, 4 analogue inputs, internal battery (12v) voltage sensor and a ttl serial port.

### Features

* CAN bus operated.  (with 120 ohm terminator jumper.)
* STM32F103C8 processor.  64k program memory 20k RAM.
* Arduino IDE platform.
* 60mm x 100mm.
* 8 Outputs:
  * 7 outputs are Ground switching (open Drain).
  * 2 of the above outputs can be jumpered to +12 (battery) switching.
  * 1 output is PUSH PULL. (PWM)
  * 4 of the outputs are PWM able.
* 4 Analogue Outputs, specifically designed for gauges like Fuel and Temp.
* 4 Analalogue Inputs, 0 - 15v range.
  * 2 inputs can be configured to 0 - 5v range.
* Battery (12V) voltage sensor (internal).
* DB25 male connector.
* Each Input or Output is simply accessed by basic CAN Buss messages. 
* Software can be customised using Arduino IDE.

#### Outputs

The outputs are mosfet driven which will drive modest outputs such as Dash lights and your average relay.  There are 8 outputs, one is PUSH PULL, the rest are Ground switching (Open Drain) with 2 that are jumperable to 12 volt switching.  

For example, drive the Tachometer (frequency modulated square wave), and drive (simulate) the existing engine water Temperature and Fuel sensor inputs to manipulate the respective gauges. 
