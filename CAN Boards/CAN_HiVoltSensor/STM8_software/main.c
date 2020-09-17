/* MAIN.C file
 * 
 *    400 Volt battery monitor
 *
 *		   Andrew McKinnon  15-sep-2020
 *       Mack-e-
 *
 */

#include <iostm8l.h>

#define enableInterrupts() {_asm("rim\n");} /*!<enable interrupts */
#define disableInterrupts() {_asm("sim\n");} /*!<disable interrupts */
#define rim() {_asm("rim\n");} /*!<enable interrupts */
#define sim() {_asm("sim\n");} /*!<disable interrupts */
#define nop() {_asm("nop\n");} /*!<No Operation */
#define trap() {_asm("trap\n");} /*!<Trap (soft IT) */
#define wfi() {_asm("wfi\n");} /*!<Wait For Interrupt */
#define wfe() {_asm("wfe\n");} /*!<Wait for event */
#define halt() {_asm("halt\n");} /*!<Halt */

#define byte unsigned char
#define int8 char

	//  LED 		PA2
#define LED_ON 			PA_ODR |= 4	
#define LED_OFF 		PA_ODR &= 251 //0b11111011
	//	Relay  	PB0
#define RELAY_ON		PB_ODR |= 0b01;
#define RELAY_OFF		PB_ODR &= 0b11111110;


#define startADC   ADC1_CR1 |= 2

#define SIZE	64		/* buffer size */
#define TRDE	0x80		/* transmit ready bit */

void toggleLED(void);
void setupTimer2(void);
void setupTimer4(void);
void setupUart(void);
void setupADC(void);
void init(void);
void loops(void);
long smoothX128(long, unsigned int, int);
void prepareBuffer(void);
char crc8x_fast( byte *,byte ) ;
void setTransmitBuffer(void);
void sendChars(void);

volatile char UID @0x4926;
volatile _Bool USART_SR_TXE @USART1_SR:7;
volatile _Bool RXNE @USART1_SR:5;
volatile _Bool FLASH_IAPSR_DUL @FLASH_IAPSR:3;
volatile _Bool LED @PA_ODR:2;
volatile _Bool READ_VREF @ADC1_TRIGR1:4;
volatile _Bool ADCCH_IN 	@ADC1_SQR2:0;
volatile _Bool ADCCH_VREF @ADC1_SQR1:4;
volatile _Bool PVD_IF @PWR_CSR1:5;
volatile _Bool PVD_OUTPUT @PWR_CSR1:6;


char ee_address @0x1002;

long millis = 0, millis_100 = 0 ;
long secondTimer = 0, threeSecondTimer =0 ;
long brownOutTime = 0;
volatile _Bool EOC @ADC1_SR:0;
volatile int adcReg @0x5344;
volatile unsigned int adcValue;
volatile long adcAVG=0;
volatile unsigned int adcVref;
volatile long avgVref=0;
byte sendbuffer[7];
byte outbuf[9];
byte sendPtr = 0;

volatile byte adcWaitFor = 10;
volatile byte discardAdc  = 10;

_Bool recvTerm = 0;
_Bool transmitting = 0;
_Bool setVref = 0;
_Bool setVin  = 0;
//_Bool adcWait = 0;
_Bool brownOut = 0;
_Bool firstRead_Vin = 1;
_Bool firstRead_Ref = 1;



main()
{
	static long lastMillis=0;	
	
	sim();
	init();
	
	setupTimer4();
	setupADC();
	
	rim();
	startADC;
	
	while (1)
	{
		if (lastMillis != millis)
		{
			lastMillis = millis;
			loops();
		}
	}
	
}

void prepareBuffer()
{
		long lavg,lavg2;
		int adc,avg,avg2;
		adc = adcValue;
		lavg = adcAVG ;
		lavg2 = avgVref;
		lavg += 64;
		avg = (int) (lavg >> 7);  // divide by 128
		lavg2 +=64;
		avg2 = (int) (lavg2 >> 7);  // divide by 128
		
		sendbuffer[0] = (byte) (adc & 0xff);
		sendbuffer[1] = (byte) ((adc >> 8) & 0xff);
		sendbuffer[2] = (byte) (avg & 0xff);
		sendbuffer[3] = (byte) ((avg >> 8) & 0xff);
		sendbuffer[4] = (byte) (avg2 & 0xff);
		sendbuffer[5] = (byte) ((avg2 >> 8) & 0xff);
		sendbuffer[6] = crc8x_fast(sendbuffer,6);
}

void Loop_100m(void)
{
	setVref = 1;
	//adcWait = 1;
	
}

void threeSecondLoop(void)
{
	
}

void secondLoop(void)
{
	//toggleLED();
	if (!brownOut)
	{
		RELAY_ON;
		if (millis - brownOutTime > 6000)
		{
			brownOut = 0;
		}
	}
	if (millis - threeSecondTimer > 2999)
	{
		threeSecondTimer = millis;
		threeSecondLoop();
	}
}

	//--- Called every 1 mSec
void loops()
{
	sendChars();
	startADC;
	
		//--- the STM32 wants some data
	if (recvTerm)
	{
		//LED_ON;
		if (!transmitting)
		{
			recvTerm = 0;
			prepareBuffer();
			setTransmitBuffer();
			// send data
		}
		//LED_OFF;
	}
	if (millis - millis_100 > 99)
	{
		millis_100 = millis;
		Loop_100m();
	}
	if (millis - secondTimer > 999)
	{
		secondTimer = millis;
		secondLoop();
	}
	
}


@svlreg @interrupt void  timer4_Overflow(void)
{
	millis ++;
	TIM4_SR1 &= 254; // clear interrupt pending
}

@interrupt void pvd(void)
{
	if (PVD_OUTPUT == 1)
	{
		//LED_OFF;
		RELAY_OFF;
		brownOutTime = millis;
		sendPtr = 0;
		outbuf[0] = '*';
		outbuf[1] = '\n ';
		outbuf[2] = '\r';
		outbuf[3] = 'R';
		outbuf[4] = 'E';
		outbuf[5] = 'S';
		outbuf[6] = 'E';
		outbuf[7] = 'T';
		
		outbuf[8] = ';';
		transmitting = 1;
	}
	PVD_IF = 1;
}


@interrupt void receivedData(void)
{
		// to clear interupt the SR may have 
		//  to be read
	byte a;
	a = USART1_SR ;
	//err = sr & 7;
	a =  USART1_DR;
	if (a == ';')
	{
		recvTerm = 1;
	}
}

@interrupt void adcEOC(void)
{
	long junk=0;
	unsigned int adc=0;
	long avg = 0;
	//LED_ON;
	adc = adcReg & 0x0FFF;
	
	if (discardAdc>0)
	{
		discardAdc --;
		return;
	}
	
	//adc = 1445;
	if (ADCCH_VREF)
	{
		//LED_OFF;
		adcVref = adc;
		if (firstRead_Ref)
		{
			avgVref = (long) adc << 7;
			firstRead_Ref = 0;
		}
		junk = (long) adc;
		junk = junk << 7;
		junk -= avgVref;
		junk = junk >> 5;
		avgVref =  junk + avgVref ;
			// switch mux to Vin
		ADCCH_IN = 1;
		ADCCH_VREF = 0;
		discardAdc = 2;
	}
	else
	{
		//LED_ON;
		adcValue = adc;
		if (firstRead_Vin)
		{
			adcAVG = (long) adc << 7;
			firstRead_Vin = 0;
		}
		junk = (long) adcValue;
		junk = junk << 7;
		junk -= adcAVG;
		junk = junk >> 5;
		adcAVG =  junk + adcAVG ;
		
		if (setVref)
		{
				// switch mux to Vref
			ADCCH_IN = 0;
			ADCCH_VREF = 1;
			discardAdc = 2;
			setVref=0;
		}
	}
	//LED_OFF;
}

void init()
{
	
	
	CLK_DIVR = 0;
	CLK_PCKENR1 = 39; //0xff;//4;
	//CLK_PCKENR1 |= 0x20;  // PCKEN15 aka usart1
	CLK_PCKENR2 |= 1;  // PCKEN20 aka ADC1 clk
	
		// Programable Voltage Detector
	
	PWR_CSR1 = 0x0C; 			// set PVR to 3.05v
	PWR_CSR1 |= 0b10000; 	// enable int
	PWR_CSR1 |= 1; 				// enable PVD
	PVD_IF = 1;						// clear int flag
	
	setupUart();
	
	// LED PA2
	PA_DDR = 4;//0b100;
	PA_CR1 = 4;//0b100;
	PA_CR2 = 0;
	
	// RELAY  PB0
	PB_DDR = 0b01;
	PB_CR1 = 0b01;
	PB_CR2 = 0;
}

/*------------------
Timer setup for timer interupt

-  	Disable UEV (soft int) by Setting UDIS bit in CR1
-		set ARPE bit in  CR1:7 ; Auto Reload PreLoad Enable;  
-		set UIE bit (Update Interupt Enable) in IER:0
--------------------*/

void setupTimer2()
{
	TIM2_PSCR = 7;   // prescale set to 128
	TIM2_ARRH = 0;
	TIM2_ARRL = 1;//125;		 // overflow at 125, should give 1mSec
	TIM2_IER = 1;
	//TIM2_EGR = 1;
	TIM2_CR1 = 0x1;  //
}

	// not used
void setupTimer4()
{
		TIM4_PSCR = 7;
		TIM4_ARR = 124;
		TIM4_IER = 1;
		TIM4_CR1 = 1;
}

void setupADC()
{
	// adc ch16 B2 , 25usec conversion rate
	ADC1_CR1 = 9; // 12 bit single conversion
	//ADC1_CR1 |= 0b100;  // Continuous mode
	ADC1_CR2 = 7; // 384 adc cycles
	ADC1_CR3 = 224; // 384 cycles
	ADC1_SQR1 = 0x80; // diable DMA
	//ADC1_SQR2 = 0b01; //ch16 
	ADC1_TRIGR2 = 0b01;  // clear schmitt for ch 16
	adcValue = adcReg;
	READ_VREF = 1;
	ADCCH_IN = 1;
	ADCCH_VREF = 0;
}

void setupUart()
{
	byte junk;
	USART1_CR1 = 0x00;		/* parameter for word length */
	USART1_CR2 = 0x2c;
	USART1_PSCR = 1;
	SYSCFG_RMPCR1 &= 0xCF;
	SYSCFG_RMPCR1 |= 0x20;  // alternate ports C6,C5   uart
			//300
	//USART1_BRR2 = 0xD5;
	//USART1_BRR1 = 0x05;
			//1200
	USART1_BRR2 = 0x35;
	USART1_BRR1 = 0x41;
			// 2400
	//USART1_BRR2 = 0x1b;
	//USART1_BRR1 = 0xa0;
			//4800
	//USART1_BRR2 = 0x05;
	//USART1_BRR1 = 0x0d;
			// 9600
	//USART1_BRR2 = 0x03;
	//USART1_BRR1 = 0x68;
			//19200
	//USART1_BRR2 = 0x01;
	//USART1_BRR1 = 0x34;
	
	
	
	RXNE = 0;
	junk = USART1_SR ;
	junk = USART1_DR ;
}

void toggleLED()
{
	LED =  ! LED;
}


		//---- Stored in program space

static byte const crc8x_table[] = {
    0x00, 0x31, 0x62, 0x53, 0xc4, 0xf5, 0xa6, 0x97, 0xb9, 0x88, 0xdb, 0xea, 0x7d,
    0x4c, 0x1f, 0x2e, 0x43, 0x72, 0x21, 0x10, 0x87, 0xb6, 0xe5, 0xd4, 0xfa, 0xcb,
    0x98, 0xa9, 0x3e, 0x0f, 0x5c, 0x6d, 0x86, 0xb7, 0xe4, 0xd5, 0x42, 0x73, 0x20,
    0x11, 0x3f, 0x0e, 0x5d, 0x6c, 0xfb, 0xca, 0x99, 0xa8, 0xc5, 0xf4, 0xa7, 0x96,
    0x01, 0x30, 0x63, 0x52, 0x7c, 0x4d, 0x1e, 0x2f, 0xb8, 0x89, 0xda, 0xeb, 0x3d,
    0x0c, 0x5f, 0x6e, 0xf9, 0xc8, 0x9b, 0xaa, 0x84, 0xb5, 0xe6, 0xd7, 0x40, 0x71,
    0x22, 0x13, 0x7e, 0x4f, 0x1c, 0x2d, 0xba, 0x8b, 0xd8, 0xe9, 0xc7, 0xf6, 0xa5,
    0x94, 0x03, 0x32, 0x61, 0x50, 0xbb, 0x8a, 0xd9, 0xe8, 0x7f, 0x4e, 0x1d, 0x2c,
    0x02, 0x33, 0x60, 0x51, 0xc6, 0xf7, 0xa4, 0x95, 0xf8, 0xc9, 0x9a, 0xab, 0x3c,
    0x0d, 0x5e, 0x6f, 0x41, 0x70, 0x23, 0x12, 0x85, 0xb4, 0xe7, 0xd6, 0x7a, 0x4b,
    0x18, 0x29, 0xbe, 0x8f, 0xdc, 0xed, 0xc3, 0xf2, 0xa1, 0x90, 0x07, 0x36, 0x65,
    0x54, 0x39, 0x08, 0x5b, 0x6a, 0xfd, 0xcc, 0x9f, 0xae, 0x80, 0xb1, 0xe2, 0xd3,
    0x44, 0x75, 0x26, 0x17, 0xfc, 0xcd, 0x9e, 0xaf, 0x38, 0x09, 0x5a, 0x6b, 0x45,
    0x74, 0x27, 0x16, 0x81, 0xb0, 0xe3, 0xd2, 0xbf, 0x8e, 0xdd, 0xec, 0x7b, 0x4a,
    0x19, 0x28, 0x06, 0x37, 0x64, 0x55, 0xc2, 0xf3, 0xa0, 0x91, 0x47, 0x76, 0x25,
    0x14, 0x83, 0xb2, 0xe1, 0xd0, 0xfe, 0xcf, 0x9c, 0xad, 0x3a, 0x0b, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xc0, 0xf1, 0xa2, 0x93, 0xbd, 0x8c, 0xdf, 0xee, 0x79,
    0x48, 0x1b, 0x2a, 0xc1, 0xf0, 0xa3, 0x92, 0x05, 0x34, 0x67, 0x56, 0x78, 0x49,
    0x1a, 0x2b, 0xbc, 0x8d, 0xde, 0xef, 0x82, 0xb3, 0xe0, 0xd1, 0x46, 0x77, 0x24,
    0x15, 0x3b, 0x0a, 0x59, 0x68, 0xff, 0xce, 0x9d, 0xac};

byte crc8x_fast( byte mem[],byte len) 
{
    byte crc;
		byte i,x;
		crc = 55;
		
		for (i =0 ; i < len;i++)
		{
        x = (byte) ( crc ^ mem[i]);
				crc = crc8x_table[x];
		}
    return crc;
}





void setTransmitBuffer()
{
		if (transmitting) return;
		
		outbuf[0] = '*';
		outbuf[1] = sendbuffer[0];
		outbuf[2] = sendbuffer[1];
		outbuf[3] = sendbuffer[2];
		outbuf[4] = sendbuffer[3];
		outbuf[5] = sendbuffer[4];
		outbuf[6] = sendbuffer[5];
		outbuf[7] = sendbuffer[6];
		
		outbuf[8] = ';';
		transmitting = 1;
		LED_ON;
}

void sendChars()
{
	//static byte p = 0;
	byte c;
	if (transmitting == 0) return;
	if (USART_SR_TXE == 0) return;
	c = outbuf[sendPtr];
	USART1_DR = c;
	
	sendPtr++;
	//toggleLED();
	if (sendPtr>8) 
	{
		sendPtr=0;
		outbuf[0] = 0;
		transmitting = 0;
		LED_OFF;
		//toggleLED();
	}
}
	