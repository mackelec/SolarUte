

#define PRESCALE_256  0B100
#define PRESCALE_64   0B011
#define WGM13         0B00010000

#define PRESCALE PRESCALE_64
#define PWMPERIOD 14706

void setup() 
{
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
  setPWM_Percent(67);
}

void loop() {
  // put your main code here, to run repeatedly:

}


void setPWM_Percent(int percent)
{
  unsigned int x;
  x = map(percent,0,100,0,PWMPERIOD);
  setPWM(x);
}

void setPWM(int pwm)
{
  unsigned int x = (unsigned int) constrain(pwm,0,PWMPERIOD);
  OCR1A = x;
}


