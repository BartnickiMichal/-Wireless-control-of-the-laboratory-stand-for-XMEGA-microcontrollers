
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Appconfiguration.h"
#include "TimeEvents.h"
#include "LEDservice.h"
#include "switch.h"
#include "bluetooth.h"
#include "lcd_lib.h"

parameters_t device;
/* *** Tablica znaków dla wyswietlacza 7-segmentowego *** */
//  '0' , '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'b', 'C', 'd', 'E', 'F',
const unsigned char buf[16]={0x21,0xF5,0x13,0x91,0xC5,0x89,0x09,0xF1,0x01,0x81,0x41,0x0D,0x2B,0x15,0x0B,0x4B};
unsigned char line1[] = "Mikrokontrolery ";
unsigned char line2[] = "Bluetooth       ";
unsigned char line3[] = "Trwa parowanie";
unsigned char dot[] = "A";
unsigned char test ='1';

void Init(void);
int main(void)
{


  avr_init();
  Init();
 

  while(1)
    {
		
	clock_timer_action();
	//bluetooth_init();
    //LED();
	
	//serviceSW3();
	//serviceSW2();
	//serviceSW1();
	//serviceSW0();
	
		
	
    }


    return(0);
}




void avr_init(void)
{
 /* inicjalizacja uk³adu */

  // Ustawienie zegara systemowego

    OSC_XOSCCTRL = 0xCB;			    // zakres 12 - 16 MHz, 16K clk start-up time
    OSC_CTRL |= 0x08;				    // External Oscillator (EO) mo¿liwy do wykorzystania

    do {} while((OSC_STATUS & 0x08) == 0);	    // czekaj a¿ EO bêdzie gotowy

    //CPU_CCP = 0xD8;				    // odblokuj rejest przed zapisem
    //CLK_CTRL = 0x03;				    // prze³¹cz system zegarowy na EO
    asm ("ldi r16,0xD8 \n"
	 "ldi r19,0x03 \n"
	 "ldi r27,0x00 \n"
	 "ldi r26,0x40 \n"
	 "out 0x34,r16 \n"
	 "st x,r19 \n"
	  : : );

   // Porty: A, B, C, D, E - kierunek linii: 0-wejœcie 1-wyjœcie

   PORTA_DIR = 0xF2;		     // linie PA7-PA4, PA1 wyjœciami, PA3,PA2 i PA0 wejœciami (analogowymi)
   PORTA_OUTSET = 0x80; 	     // sterowanie lini¹ PA7 - dezaktywacja uk³adu IC4

   PORTB_DIR = 0x00;		     // Port B - wejœcie
  PORTC_DIR = 0xFF;		     // port C - wyjœciowy
   /*
   Port D: TX wyjscie(PD7), RX wejscie(PC6) reszta wejsciem 1000000
   */
   
   PORTD_DIR = 0x80;
  // PORTD_DIR=0xBF;		     // port D - wejœciowy
   PORTE_DIR = 0x08;		     // linia PE3 (TxD) wyjœciem, pozosta³e linie wejœciowe

 /* wygaszenie diod po resecie */

   PORTC_OUT  = 0xFF;		    // wystawienie stanu wysokiego na diody LED
   PORTA_OUTSET = 0x40; 	    // aktywacja uk³adu IC3 (od linijki LED)
   PORTA_OUTCLR = 0x40; 	    // zatrzaœniêcie stanu na wyjœciu IC3


 /*  - kod konfiguracji przetwornika ADC A

 // Kalibracja przetwornika ADCA    // ADACAL0 - 0x20; ADACAL1 - 0x21;

   dana = (read_calibration_row(0x21)<<8) |  read_calibration_row(0x20);
   ADCA_CAL = dana;		    // ADCA_CAL = 0x0444;

// ADC A

   ADCA_CTRLB = 0x06;		    // wynik bez znaku, 12-bitowa rozdzielczoœæ, justowanie w lewo
   ADCA_REFCTRL = 0x20; 	    // zewnêtrzne napiêcie referencyjne z pinu AREF na PORT A
   ADCA_PRESCALER = 0x02;	    // podzia³ sygna³u zegarowego DIV16 (1 MHz)

   ADCA_CH0_CTRL = 0x01;	    // wejœcie dodatnie na kanale CH0
   ADCA_CH1_CTRL = 0x01;	    // wejœcie dodatnie na kanale CH1

   ADCA_CH0_MUXCTRL = 0x10;	    // kana³ CH0 na pinie ADC2
   ADCA_CH1_MUXCTRL = 0x18;	    // kana³ CH1 na pinie ADC3

   ADCA_CH0_INTCTRL = 0x01;	    // Low level interrupt na kanale CH0
   ADCA_CH1_INTCTRL = 0x01;	    // Low level interrupt na kanale CH1

   ADCA_CTRLA = 0x01;		    // w³¹czenie ADC A
  */


  /* przerwania zewnêtrzne INT0 i INT1 na PORT D */
  

   PORTD_INTCTRL = 0x05;	    // niski poziom przerwañ INT0 i INT1
   PORTD_INT1MASK = 0x03;	    // PD1 - INT1
   PORTD_INT0MASK = 0x0C;       // przerwanie na PD0-3 dla INT0
   PORTD_PIN0CTRL = 0x01;	    // INT0 na zbocze narastaj¹ce
   PORTD_PIN1CTRL = 0x01;	    // INT1 na zbocze narastaj¹ce

   PMIC_CTRL = 0x01;		    // Low level interrupt odblokowane
   
 

   
    // konfiguracja interfejsu szeregowego UART na porcie C


    USARTD1_BAUDCTRLA = 0x67;	    // 103 - daje 9600 b/s przy 16 MHz 
    USARTD1_BAUDCTRLB = 0x00;

    USARTD1_CTRLA = 0x14;	    // Low level interrupt Rx i TX odblokowane
    USARTD1_CTRLC = 0x03;	    // Tryb asynchroniczny, 8-bit dana
    USARTD1_CTRLB = 0x18;	    // W³¹czenie uk³adów odbiornika RX i nadajnika TX
	
/* USART */

/*
PORTD_OUT = 0xFF;
PORTD.DIRSET = PIN7_bm;	// PE3 (TXD0) as output
PORTD.DIRCLR = PIN6_bm;	// PE2 (RXD0) as input
int bsel = 1603   ;
uint8_t bscale = 10 ;
// Set baud rate & frame format
USARTD1.BAUDCTRLB = (bscale << 4) | (bsel >> 8);
USARTD1.BAUDCTRLA = (uint8_t) bsel;
// Set mode of operation
USARTD1.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;
// Enable transmitter and reciever
USARTD1.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
*/
   
   
   sei();

   return;
}
void Init(void)
{
	device.mode_setting=0x01;
	date_var=0x01; // poczatowa wartosc dla poruszania ledami
	LCDinit();			// inicjalizacja wyœwietlacza LCD 2 x 16 znaków
	LCDclr();			//  - lista dostêpnych funkcji znajduje siê w pliku LCD_lib.c
	LCDGotoXY(0,0);
	LCDstring(line1,16);
	LCDGotoXY(0,1);
	LCDstring(line2,16);

	LCDcursorOFF();
	send8LED(~0x01);
	send7SEG(counter);
	
	data_received_UART=(unsigned char*)malloc(sizeof(data_received_UART)*34) ;
	
}



