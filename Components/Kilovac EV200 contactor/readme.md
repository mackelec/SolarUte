##  Kilovac EV200 contactor

![Kilovac EV200](https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/kilovac_3_60.jpg)


### Introduction

Three of these Kilovac EV200 contactors were purchased through the Ebay guy in Israel for the use in the Solar Ute.  Good thing since one of them is faulty - intermittant contacts.  Like most EV conversions, a contactor each will be used to connect the positive and negative rails to the Battery.  After experiencing the trouble with the intermittant EV200 I thought I'd look into their power requirements, Pull-in voltage and Hold-up voltage.

EV200 Datasheet  https://github.com/mackelec/SolarUte/blob/master/Components/Kilovac%20EV200%20contactor/Kilovac%20EV200.pdf

### Results

I found the Pull-in and Hold-up voltages using the Siglent SPD1305X power supply and adjusting the voltage in 100mV steps.  Current was also measured by the Siglent SPD1305X.
The EV200 included the Economiser that came with it.
100 mV below the results given will not Pull-in or Hold-up.

Pull-in Voltage   8.6 Volts
Hold-up Voltage   6.6 Volts
Current           117mA  @ 14 Volts (1.628W)
                  138mA  @ 12 Volts 
### 
