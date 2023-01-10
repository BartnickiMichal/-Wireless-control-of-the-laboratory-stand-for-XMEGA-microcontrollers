#include "Appconfiguration.h"
#include <avr/interrupt.h>
extern parameters_t device;
int received_flag;
uint16_t counter_100ms;

const uint16_t us_count = (1<<2)*100; // 100u
delay_loop_timer(uint16_t __count)
{
	__asm__ volatile (
	"1: sbiw %0,1" "\n\t"
	"brne 1b"
	: "=w" (__count)
	: "0" (__count)
	);
}
void count_timer() //PORTD_INT1_vect RTC_INT1_base
{
	 delay_loop_timer(us_count); 
	
	//TCE1_INT_base=0x06A;
	 static uint16_t counter100us, counter1ms,counter50ms,counter100ms, counter1s;
	 static uint16_t counter2s;
	  
	 if(!ISCLOCKFLAGSET(CLOCK100US))
	 {
		  CLOCKFLAGSET(CLOCK100US);
		 
	 }
	
	 
	 if(++counter1ms>10 && !ISCLOCKFLAGSET(CLOCK1MS)){
		 CLOCKFLAGSET(CLOCK1MS);
		 counter1ms = 0;
	 }
	 if(++counter50ms>49 && !ISCLOCKFLAGSET(CLOCK50MS)){
		 CLOCKFLAGSET(CLOCK50MS);
		 counter50ms = 0;
	 }
	 
	 if(++counter100ms>99 && !ISCLOCKFLAGSET(CLOCK100MS)){
		 CLOCKFLAGSET(CLOCK100MS);
		 counter100ms = 0;
	 }
	 
	 if(++counter1s>999 && !ISCLOCKFLAGSET(CLOCK1S)){
		 CLOCKFLAGSET(CLOCK1S);
		 counter1s = 0;
	 }
	 if(++counter2s>1999 && !ISCLOCKFLAGSET(CLOCK2S)){
		 CLOCKFLAGSET(CLOCK2S);
		 counter2s = 0;
	 }


}


void clock_timer_action(void)
{
	count_timer();
	if(ISCLOCKFLAGSET(CLOCK100US)){
		CLOCKFLAGRESET(CLOCK100US);
		
		
	}
	if(ISCLOCKFLAGSET(CLOCK1MS)){
		CLOCKFLAGRESET(CLOCK1MS);
		
	}
	if(ISCLOCKFLAGSET(CLOCK50MS)){
		bluetooth_init();
		CLOCKFLAGRESET(CLOCK50MS);
		
		
	}
	if(ISCLOCKFLAGSET(CLOCK100MS)){
		CLOCKFLAGRESET(CLOCK100MS);
		LED();
		 
		counter_100ms++;
		// if((device.mode_setting>>2)&0x0001==1) // czy jest tryb LED
		if(ISMODESET(SWITCH_ON))
		 {
			if(device.counter_led!=0)
			{
				if((20-device.counter_led)>counter_100ms)
				{
					
					send8LED(~date_var);
				}
				else
				send8LED(0xFF);
					
			}
			else
			{
				send8LED(~date_var);
			}
			
			 
		 }
		
		
		if(counter_100ms>21)
		counter_100ms=0;
	
		//received_flag=0;
		
	}
	if(ISCLOCKFLAGSET(CLOCK1S)){
		CLOCKFLAGRESET(CLOCK1S);
		

		
	}
}