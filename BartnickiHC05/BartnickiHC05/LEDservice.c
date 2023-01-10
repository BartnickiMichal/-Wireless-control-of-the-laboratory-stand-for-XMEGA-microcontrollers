
#include "Appconfiguration.h"
#include "LEDservice.h"
#include <avr/interrupt.h>
extern parameters_t device;
static char prev_date_from_led;
void send8LED(unsigned char bajt)
{
	/* wysterowanie linijki diod led   */

	PORTC_OUT = bajt;//bajt		   // wystawienie zmiennej na port C
	PORTA_OUTSET = 0x40; 	   // aktywacja uk�adu IC3 (od linijki LED)
	PORTA_OUTCLR = 0x40; 	   // zatrza�ni�cie stanu na wyj�ciu IC3
	return;
}
unsigned char ReadDip8(void)
{
	/* funkcja odczytuje stan prze�acznik�w DIP SWITCH  */
	uint8_t bajt;

	cli();			   // blokada przerwan
	PORTC_DIR = 0x00;		   // ustawienie portu C jako wej�ciowy
	PORTA_OUTCLR = 0x80; 	   // aktywacja uk�adu IC4 (od DIP-8)
	bajt = PORTC_IN;		   // odczyt z pin�w stanu prze��cznik�w DIP SWITCH
	bajt = PORTC_IN;		   // odczyt z pin�w stanu prze��cznik�w DIP SWITCH - dodatkowy
	PORTA_OUTSET = 0x80; 	   // dezaktywacja uk�adu IC4
	PORTC_DIR = 0xFF;		   // ustawienie portu C na wyj�ciowy
	sei();			   // globalne odblokowanie przerwa�
	return bajt;
}

void send7SEG(unsigned char bajt)
{
	/* wysterowanie wy�wietlacza 7 segmentowego  */
	bajt &= 0x0F;		   // u�ycie maski 0x0F - zerowanie czterech najstarszych bit�w
	//result = buf[bajt];		   // konwersja na kod wys. 7-seg.

	//PORTC_OUT = result;		   // wystawienie zmiennej na port C
	PORTA_OUTSET = 0x20; 	   // aktywacja uk�adu IC2 (od wysw. 7-seg.)
	PORTA_OUTCLR = 0x20; 	   // zatrza�ni�cie stanu na wyj�ciu IC2
	return;
}




void LED(void)
{
char date;
char max_value = 0xFF;
date = ReadDip8(); 	
//send8LED(!date);	       // odczyt prze��cznikow DIP SWITCH

	if(!ISMODESET(SWITCH_ON) && ISMODESET(LED_ON))
	{
		if(prev_date_from_led!=date)
		{
		//uint16_t send_choose_led=0;
		//sendbyte((prev_date_from_led^date));	 //
		//if(((date^zero)-(~prev_date_from_led))<129) // zapale ledy
		//if(date==0xFF)
		//sendbyte(0x11);
		//else
		//sendbyte(date^0xFF); 
		//sendbyte(date^prev_date_from_led);
		//sendbyte((date^max_value)-(~prev_date_from_led)); 
		//else
		//sendbyte(0x82);
		//send_choose_led=date^max_value; // ktory bit sie zmienil
		
		sendbyte(date);
		}
		//if(date!=0)
		
		//sendbyte(date);
		send8LED(date);
	}
	if(date<0xFF && date!=0xF7)
	{
		prev_date_from_led=date;
		MODERESET(SWITCH_ON);
		MODESET(LED_ON);
	}
	else
	MODERESET(LED_ON);	
	


}

