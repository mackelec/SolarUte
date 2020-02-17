
/*
 *      Nissan LEAF DC-DC converter Modulator.
 *      
 *      Example setup of TIMER 1 on the Arduino UNO / NAN0 etc
 *      
 *      ** According to Jim Hanna, 
 *          https://youtu.be/ipEzK1yzxHY?t=6424
 *          The 8.5Hz pwm signal inversely controls the output voltage
 *          from 13.0 Volts to 15.0 Volts at pwm = 20%
 *          
 *          
 *      Andrew McKinnon
 *      
 */




#define MIN_VOLTS         13000   //  milliVolts
#define MAX_VOLTS         15000   //  milliVolts
#define MAX_VOLTAGE_PWM   20  // percent
#define MIN_VOLTAGE_PWM   80  

/*----------------------------------------
 *    #defines required to set up Timer 1
 ----------------------------------------*/

#define PRESCALE_256  0B100
#define PRESCALE_64   0B011
#define WGM13         0B00010000

#define PRESCALE PRESCALE_64
#define PWMPERIOD 14706

//----------------------------

#define pin_POT     A0

void setup() 
{
  pinMode(pin_POT,INPUT);
  pinMode(13,OUTPUT);  //  LED
  pinMode(9,OUTPUT);   //  PWM output
  noInterrupts();

  /*--------------------------------
   *    Timer 1 Setup
   *        period = 8.5Hz
   ---------------------------------*/
  
  TCCR1A = 0;
  TCCR1B = PRESCALE + WGM13 ;
  TCCR1A = 0B10100000;  //COM1x1 = 1;  Clr on match
  ICR1 = PWMPERIOD;
  OCR1A = 0;
  interrupts();
  //setPWM(9999);
  //setPWM_Percent(67);
}

void loop() 
{
  int adc = analogRead(pin_POT);
  int myVolts = map(adc,0,1023,MIN_VOLTS,MAX_VOLTS);
  
  set_LeafDcDcConverterByVoltage(myVolts);
  delay(1000);
}


void set_LeafDcDcConverterByVoltage(int milliVolts)
{
  unsigned int x;
  int v = constrain(milliVolts,MIN_VOLTS,MAX_VOLTS);
  x = map(v,MIN_VOLTS,MAX_VOLTS,MIN_VOLTAGE_PWM,MAX_VOLTAGE_PWM);
  setPWM_Percent(x);
}

void setPWM_Percent(int percent)
{
  unsigned int x;
  int p = constrain(percent,MAX_VOLTAGE_PWM,MIN_VOLTAGE_PWM);
  x = map(p,0,100,0,PWMPERIOD);
  setPWM(x);
}

void setPWM(int pwm)
{
  unsigned int x = (unsigned int) constrain(pwm,0,PWMPERIOD);
  OCR1A = x;
}


