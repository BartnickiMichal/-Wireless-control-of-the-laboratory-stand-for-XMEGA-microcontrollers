#include "Appconfiguration.h"
#include "lcd_lib.h"
#include "switch.h"
#include <avr/interrupt.h>
extern parameters_t device;

ISR(PORTD_INT0_vect)
{
	
	MODERESET(BLUETOOTH_ON);
	if(bit_is_clear(PORTD_IN,2))
	{
		loop_until_bit_is_set(PORTD_IN,2);
		serviceSW2();	
	}
	if(bit_is_clear(PORTD_IN,3))
	{
		loop_until_bit_is_set(PORTD_IN,3);
		serviceSW3();	
	}
	
	
}

// external interrupt 1 - ...

ISR(PORTD_INT1_vect)
{
	
	MODERESET(BLUETOOTH_ON);
	//send8LED(0x0F);
	 //if (bit_is_clear(PORTD_IN,0))
	 //{
		loop_until_bit_is_set(PORTD_IN,0);
		serviceSW0();
	 //}
	if(bit_is_clear(PORTD_IN,1))
	{
		loop_until_bit_is_set(PORTD_IN,1);
		serviceSW1();
	}
	
	
}

void serviceSW0(void)
{
		MODESET(SWITCH_ON);
		//MODERESET(BLUETOOTH_ON);
		send8LED(0xF0);
		if(!ISMODESET(LED_ON))
		{
			date_var = date_var >> 1;
			if(date_var == 0) date_var = 0x80;
			send8LED(~date_var);		
		}
		MODERESET(SWITCH_ON);
	
		if(!ISMODESET(BLUETOOTH_ON))
		{
			
			sendbyte(0x1E);
		}
		
}
void serviceSW1(void)
{
			
	MODESET(SWITCH_ON);
	if(!ISMODESET(LED_ON))
	{
		date_var = date_var << 1;
		if(date_var == 0) date_var = 0x01;
		send8LED(~date_var);
		if(!ISMODESET(BLUETOOTH_ON))
		sendbyte(0x2E);	
	}
	MODERESET(SWITCH_ON);
	


}


void serviceSW2(void)
{
	 MODESET(SWITCH_ON);
	 //MODERESET(BLUETOOTH_ON);
	 if(!ISMODESET(LED_ON))
	 {
		 
		 
			 if(device.counter_led>0)
			 device.counter_led--;
			 if(!ISMODESET(BLUETOOTH_ON))
			 sendbyte(0x3E);
			 	 
	 }
	
	
}


void serviceSW3(void)
{
	MODESET(SWITCH_ON);
	//MODERESET(BLUETOOTH_ON);
	if(!ISMODESET(LED_ON))
	{
		
	if(device.counter_led>19)
	device.counter_led=20;
	else
	device.counter_led++;
	if(!ISMODESET(BLUETOOTH_ON))
	sendbyte(0x4E);	
	}
	
	
}

