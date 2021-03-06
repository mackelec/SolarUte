##  Kilovac EV200 contactor

![Kilovac EV200](https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/kilovac_3_60.jpg)


### Introduction

Three of these Kilovac EV200 contactors were purchased through the Ebay guy in Israel for the use in the Solar Ute.  Good thing since one of them is faulty - intermittant contacts.  Like most EV conversions, a contactor each will be used to connect the positive and negative rails to the Battery.  After experiencing the trouble with the intermittant EV200 I thought I'd look into their power requirements, Pull-in voltage and Hold-up voltage.

EV200 Datasheet  https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/Kilovac%20EV200.pdf

### Results

I found the Pull-in and Hold-up voltages using the Siglent SPD1305X power supply and adjusting the voltage in 100mV steps.  Current was also measured by the Siglent SPD1305X.
100 mV below the results given will not Pull-in or Hold-up.

####  With Economiser

 * Pull-in Voltage   8.6 Volts
 * Hold-up Voltage   6.6 Volts
 * Current
   - 117mA  @ 14 Volts (1.628W)
   - 138mA  @ 12 Volts 
   
####  Without Economiser

 * Pull-in Voltage   4.6 Volts
 * Hold-up Voltage   0.7 Volts
 * Current
   - 4100mA  @ 14 Volts (cold)
   - 3600mA  @ 12 Volts (cold)
 
 
### Waveforms

#### Economiser Removed

This scope shot is using a Hantek current clamp with the wire wrapped around 8 times with the Hantek set to 1mV / 100mA setting.
The economiser was removed for this test - the contactor got uncomfortably hot after a while.  The current also drops off as the contactor got warmer.  Depending on temperature, the current can be as high as 4 amps with ~3.6 amps being more the norm. (measured by the PSU)

![No Economiser](https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/SDS00001.png)




#### Economiser Installed


With the economiser installed.  This trace is measured the contactor current through a 1 ohm resistor.



![With Economiser](https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/SDS00003.png)



The trace is zoomed out, past the start up phase and showing how the economiser is keeping the current so low.

![With Economiser](https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/SDS00004.png)

Energising phase.  Soft starting.

![Start](https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/SDS00005.png)

![Start](https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/SDS00006.png)





