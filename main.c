#include <msp430.h>

#define CS1 BIT0  //2.0 is CS
#define CS2 BIT2  //2.0 is CS
#define MOSI BIT7  //1.7 is SPI MOSI
#define SCLK BIT5  //1.5 is SPI clock
#define chipSelectPin BIT1  //1.5 is SPI clock

int average = 500;


int digitcount =0;
int number =0;
int CS =0;
int NUMBERV1[4];
int NUMBERV2[4];
int NUMBERA1[4];
int NUMBERA2[4];
int numberfix =0;
int number2fix =0;
int number3fix =0;
int number4fix =0;
int SB =0;
double Volts1 =.1;
double Volts2 =.1;
double Amps1 =.1;
double Amps2 =.1;

double Volts1pre =0;
double Volts2pre =0;
double Amps1pre =0;
double Amps2pre =0;

void Init_MAX7219(void);
void SPI_Init(void); //SPI initialization
void SPI_Write1(unsigned char MSB, unsigned char CS); //SPI write one byte
void SPI_Write2(unsigned char MSB, unsigned char LSB,unsigned char CS); //SPI write two bytes
int spiA_transfer24(unsigned char one, unsigned char two, unsigned char three);
unsigned int spiA_transfer16(unsigned int inw16);
unsigned char spiA_transfer(unsigned char inb);
void spiA_init(void);

void Read_ADC(void) //SPI write one byte
{
	int i;

	for(i= average; i>0;i--)
	{
		Volts1pre += spiA_transfer24(0x0C,0x00,0x00);
	}
	Volts1pre = Volts1pre / average;
	Volts1 = ((double) (Volts1pre * 5)) / 687;


	for(i= average; i>0;i--)
	{
		Volts2pre += spiA_transfer24(0x0E,0x00,0x00);
	}
	Volts2pre = Volts2pre / average;
	Volts2 = ((double) (Volts2pre * 5)) / 689;

	for(i= average; i>0;i--)
	{
		Amps1pre += (double)(spiA_transfer24(0x09,0x00,0x00));
	}
	Amps1pre = Amps1pre / average;
	Amps1 = ((double) (Amps1pre + 12)) *8.8;
	_delay_cycles(10);
	if(Amps1 <0)
	{
		Amps1 =0;
	}

	for(i= average; i>0;i--)
	{
		Amps2pre += (spiA_transfer24(0x0B,0x00,0x00));
	}
	Amps2pre = Amps2pre / average;
	Amps2 = ((double) (Amps2pre + 5)) *8.8;
	_delay_cycles(10);

	if(Amps2 <0)
	{
		Amps2 =0;
	}

}

void Write_Voltage1(double number) //SPI write one byte
{
	unsigned char i,digit0,digit1,digit2,digit3;
if(number > 999)
{
numberfix =	number;
digit3=0;
digit2=0;
digit1=0;
digit0=1;

}
else if(number > 99 && number < 999)
{
numberfix =	number *10;
digit3=0;
digit2=0;
digit1=1;
digit0=0;
}
else if(number > 9 && number < 99)
{
numberfix =	number *100;
digit3=0;
digit2=1;
digit1=0;
digit0=0;
}
else if(number < 9)
{
numberfix =	number *1000;
digit3=1;
digit2=0;
digit1=0;
digit0=0;
}
	NUMBERV1[3]= ((numberfix /1000)|(digit3 << 7));
	NUMBERV1[2]= ((numberfix /100) % 10)|(digit2 << 7);
	NUMBERV1[1]= (((numberfix /10) % 100) % 10)|(digit1 << 7);
	NUMBERV1[0]= (((numberfix % 1000) % 100)%10)|(digit0 << 7) ;

	for(i=1;i<5;i++)
	{
		SPI_Write2(i,NUMBERV1[i-1],1);
	}

}
void Write_Voltage2(double number) //SPI write one byte
{
	unsigned char i,digit0,digit1,digit2,digit3;
if(number > 999)
{
number2fix =	number;
digit3=0;
digit2=0;
digit1=0;
digit0=1;

}
else if(number > 99 && number < 999)
{
number2fix =	number *10;
digit3=0;
digit2=0;
digit1=1;
digit0=0;
}
else if(number > 9 && number < 99)
{
number2fix =	number *100;
digit3=0;
digit2=1;
digit1=0;
digit0=0;
}
else if(number < 9)
{
number2fix =	number *1000;
digit3=1;
digit2=0;
digit1=0;
digit0=0;
}
	NUMBERV2[3]= ((number2fix /1000)|(digit3 << 7));
	NUMBERV2[2]= ((number2fix /100) % 10)|(digit2 << 7);
	NUMBERV2[1]= (((number2fix /10) % 100) % 10)|(digit1 << 7);
	NUMBERV2[0]= (((number2fix % 1000) % 100)%10)|(digit0 << 7) ;

	for(i=1;i<5;i++)
	{
		SPI_Write2(i,NUMBERV2[i-1],2);
	}

}

void Write_Amperage1(double number) //SPI write one byte
{
	unsigned char i,digit0,digit1,digit2,digit3;
if(number > 999)
{
number3fix =	number;
digit3=0;
digit2=0;
digit1=0;
digit0=1;

}
else if(number > 99 && number < 999)
{
number3fix =	number *10;
digit3=0;
digit2=0;
digit1=1;
digit0=0;
}
else if(number > 9 && number < 99)
{
number3fix =	number *100;
digit3=0;
digit2=1;
digit1=0;
digit0=0;
}
else if(number < 9)
{
number3fix =	number *1000;
digit3=1;
digit2=0;
digit1=0;
digit0=0;
}
	NUMBERA1[3]= ((number3fix /1000)|(digit3 << 7));
	NUMBERA1[2]= ((number3fix /100) % 10)|(digit2 << 7);
	NUMBERA1[1]= (((number3fix /10) % 100) % 10)|(digit1 << 7);
	NUMBERA1[0]= (((number3fix % 1000) % 100)%10)|(digit0 << 7) ;

	for(i=5;i<9;i++)
	{
		SPI_Write2(i,NUMBERA1[i-5],1);
	}
}
void Write_Amperage2(double number) //SPI write one byte
{
	unsigned char i,digit0,digit1,digit2,digit3;
if(number > 999)
{
number4fix =	number;
digit3=0;
digit2=0;
digit1=0;
digit0=1;

}
else if(number > 99 && number < 999)
{
number4fix =	number *10;
digit3=0;
digit2=0;
digit1=1;
digit0=0;
}
else if(number > 9 && number < 99)
{
number4fix =	number *100;
digit3=0;
digit2=1;
digit1=0;
digit0=0;
}
else if(number < 9)
{
number4fix =	number *1000;
digit3=1;
digit2=0;
digit1=0;
digit0=0;
}
	NUMBERA2[3]= ((number4fix /1000)|(digit3 << 7));
	NUMBERA2[2]= ((number4fix /100) % 10)|(digit2 << 7);
	NUMBERA2[1]= (((number4fix /10) % 100) % 10)|(digit1 << 7);
	NUMBERA2[0]= (((number4fix % 1000) % 100)%10)|(digit0 << 7) ;

	for(i=5;i<9;i++)
	{
		SPI_Write2(i,NUMBERA2[i-5],2);
	}

}

void SPI_Init(void) //SPI initialization
{
    P2DIR |= CS1;  //cs is output
    P2DIR |= CS2;  //cs is output
    P1SEL |= MOSI + SCLK; //spi init
    P1SEL2 |= MOSI + SCLK; //spi init
    UCB0CTL1 = UCSWRST;
     UCB0CTL0 |= UCMSB + UCMST + UCSYNC + UCCKPH; // 3-pin, 8-bit SPI master
     UCB0CTL1 |= UCSSEL_2;                     // SMCLK
     UCB0BR0 = 10;                          // spi speed is smclk/10
     UCB0BR1 = 0;                              //
     UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

     __enable_interrupt(); // enable all interrupts
}

void SPI_Write2(unsigned char MSB, unsigned char LSB,unsigned char CS) //SPI write two bytes
{

	if(CS == 1)
	{
    P2OUT &= ~CS1;
    _delay_cycles(50);
    UCB0TXBUF = MSB ;
    while (UCB0STAT & UCBUSY);
    UCB0TXBUF = LSB ;
    while (UCB0STAT & UCBUSY);
    P2OUT |= CS1;
	}
	else
	{
	    P2OUT &= ~CS2;
	    _delay_cycles(50);
	    UCB0TXBUF = MSB ;
	    while (UCB0STAT & UCBUSY);
	    UCB0TXBUF = LSB ;
	    while (UCB0STAT & UCBUSY);
	    P2OUT |= CS2;
	}
}
void SPI_Write1(unsigned char MSB, unsigned char CS) //SPI write one byte
{

	if(CS == 1)
	{
    P2OUT &= ~CS1;
    _delay_cycles(50);
    UCB0TXBUF = MSB ;
    while (UCB0STAT & UCBUSY);
    P2OUT |= CS1;
	}
	else
	{
	    P2OUT &= ~CS2;
	    _delay_cycles(50);
	    UCB0TXBUF = MSB ;
	    while (UCB0STAT & UCBUSY);
	    P2OUT |= CS2;
	}
}

void Init_MAX7219(void)
{
    SPI_Write2(0x09, 0xFF, 0);       //
    SPI_Write2(0x0A, 0x01, 0);       // intensity
    SPI_Write2(0x0B, 0x07, 0);       //
    SPI_Write2(0x0C, 0x0F, 0);       //
    SPI_Write2(0x0F, 0x0F ,0);       //
    SPI_Write2(0x0F, 0x00, 0);       //

    SPI_Write2(0x09, 0xFF, 1);       //
    SPI_Write2(0x0A, 0x01, 1);       // intensity
    SPI_Write2(0x0B, 0x07, 1);       //
    SPI_Write2(0x0C, 0x0F, 1);       //
    SPI_Write2(0x0F, 0x0F, 1);       //
    SPI_Write2(0x0F, 0x00, 1);       //
}
int main(void)
{
        WDTCTL = WDTPW | WDTHOLD;

        BCSCTL1 = CALBC1_16MHZ;
        DCOCTL = CALDCO_16MHZ;

    SPI_Init();
    spiA_init();
    _delay_cycles(100000);
    Init_MAX7219();
    _delay_cycles(1000);
    while(1)
    {
    	Read_ADC();
    	Write_Voltage1(Volts1); //SPI write one byte
    	Write_Voltage2(Volts2); //SPI write one byte
    	Write_Amperage1(Amps1); //SPI write one byte
    	Write_Amperage2(Amps2); //SPI write one byte
    	_delay_cycles(1000);

    }

}

    void spiA_init(void)
    {
      /* Configure ports on MSP430 device for USCI_A */
      P1SEL |= BIT1 | BIT2 | BIT4;
      P1SEL2 |= BIT1 | BIT2 | BIT4;
      P2DIR |= chipSelectPin;

      /* USCI-A specific SPI setup */
      UCA0CTL1 |= UCSWRST;
      UCA0MCTL = 0x00;  // Clearing modulation control per TI user's guide recommendation
      UCA0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;  // SPI mode 0, master
      UCA0BR0 = 10;  // SPI clocked at same speed as SMCLK
      UCA0BR1 = 0;
      UCA0CTL1 = UCSSEL_2;  // Clock = SMCLK, clear UCSWRST and enables USCI_A module.
    }



unsigned char spiA_transfer(unsigned char inb)
{
  UCA0TXBUF = inb;
  while ( !(IFG2 & UCA0RXIFG) )  // Wait for RXIFG indicating remote byte received via SOMI
    ;
  return UCA0RXBUF;
}

int spiA_transfer24(unsigned char one, unsigned char two, unsigned char three)
{
volatile unsigned char firstwrite24;
volatile unsigned char secondwrite24;
volatile unsigned char thirdwrite24;
volatile unsigned char firstread24;
volatile unsigned char secondread24;
volatile unsigned char thirdread24;
volatile long total;


/*
firstwrite24 = (inw24 & 0x000000FF);
secondwrite24 = (inw24 & 0x0000FF00);
thirdwrite24 = (inw24 & 0x00FF0000);
*/
firstwrite24 = three;
secondwrite24 = two;
thirdwrite24 = one;

total =0;
P2OUT &= ~chipSelectPin;
_delay_cycles(300);

UCA0TXBUF = thirdwrite24;
while ( !(IFG2 & UCA0RXIFG) )
  ;
	thirdread24 = UCA0RXBUF;
  UCA0TXBUF = secondwrite24;
  while ( !(IFG2 & UCA0RXIFG) )
    ;
 secondread24 = UCA0RXBUF;
  UCA0TXBUF = firstwrite24;
  while ( !(IFG2 & UCA0RXIFG) )
    ;
  firstread24 = UCA0RXBUF;

  _delay_cycles(300);
  P2OUT |= chipSelectPin;

  SB = (secondread24 & 0x10) >> 4;



  if(SB == 1)
  {
	  total = (-1) * (4096 -(((secondread24 & 0xF) << 8)+ firstread24));
  }
  else
  {
	  total = (((secondread24 & 0xF) << 8)+ firstread24);
  }

  _delay_cycles(300);

  return total;
}
