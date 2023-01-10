#include "bluetooth.h"
#include "Appconfiguration.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
extern parameters_t device;
#define BAUDRATE 9600
uint8_t test;
int received_flag;
uint8_t receive_bytes_setting;
uint8_t end_transmission;
static uint16_t wait_for_data;
/*
uint16_t ISMODESET(uint16_t mode)
{
	uint16_t check_mode=device.mode_setting;
	check_mode=(check_mode<<(16-mode));
	check_mode=(check_mode>>(16-mode+(mode-1)));
	return check_mode;
//zwaraca 1 jest ustawiony jest bit na danym miejscu w innym wypadku zero	
	
}
*/

ISR(USARTD1_RXC_vect) //uart na d
{
	
	
	unsigned char get_bytes = USARTD1_DATA;
	uint8_t get_bytes_int=USARTD1_DATA;
	Received_Data_config(get_bytes_int);
	Received_Data_LCD(get_bytes);
	
	
}
// UART transmit interrupt - ustawienie flagi koñca wyslania bajta
ISR(USARTD1_TXC_vect)
{
	//device.data_recived_flag=1;
	end_transmission=0;
}
void Received_Data_config(uint16_t get_bytes_int)
{
	
	if((get_bytes_int>0 || receive_bytes_setting>0) && received_flag==0)
	{	
		if(receive_bytes_setting==0)
		device.mode_setting=0;
			
		device.mode_setting|=get_bytes_int;
		if(receive_bytes_setting==0)
		device.mode_setting=device.mode_setting<<8;
		
		receive_bytes_setting++;
		if(receive_bytes_setting==2)
		{
			receive_bytes_setting=0;
			
		}
		
		
		
		
		
	}
	
}
void Received_Data_LCD(unsigned char get_bytes)
{
	if(received_flag==1)
	{
		if(data_received_UART!=NULL)
		{
		*data_received_UART=get_bytes;
		data_received_UART++;
		device.bit_counter++;		
		
		
		
		
		}
		
	}

}




void sendbyte(unsigned char usart_data)
/* funkcja wysy³a bajt przez interfejs USART E	*/
{
	
	while(end_transmission == 1){};	       // czekaj na zakoñczenie transmisji

	USARTD1_DATA = usart_data;	   // uruchmienie transmisji
	end_transmission = 1;

	return;
	
}
void bluetooth_init()
{
	if(ISMODESET(BLUETOOTH_ON)) //wlaczono tryb bluetooth
	{
		if(ISMODESET(SWITCH_ON))
		{
			LCDclr();
			uint8_t choose_sw=(device.mode_setting>>8);
			
			if(choose_sw==0x01)
			serviceSW0();
			if(choose_sw==0x02)
			serviceSW1();
			if(choose_sw==0x03)
			serviceSW2();
			if(choose_sw==0x04)
			serviceSW3();
			device.mode_setting=0x0005;
			
			
		}
		else if(ISMODESET(LED_ON))
		{
			LCDclr();
			uint8_t choose_led=(device.mode_setting>>8);
			send8LED(choose_led);
			device.mode_setting=0x0000;
			
		}
		else if(ISMODESET(LCD_ON))
		{
			
			//send8LED(device.mode_setting);
			received_flag=1;
		
			
		 if(device.bit_counter>1)
			{		
				
				
				_delay_ms(500);	// czekaj na odbior danych
			// nie wiadomo czemu if(++counter1ms>500ms) nie zadziala, wchodzi mimo wszystko i czasami wyswietla sie niepe?ny tekst
				
				LCD_view(); 
				received_flag=0;
				device.mode_setting=0;	
				wait_for_data=0;
				
				
				
			}
			
		}
		
	}
	
}