#include <avr/io.h>

#define BLUETOOTH_ON 0
#define LED_ON 1
#define SWITCH_ON 2
#define LCD_ON 3

#define MODESET(bit) device.mode_setting|=(1<<bit)
#define MODERESET(bit) device.mode_setting&=~(1<<bit)
#define ISMODESET(bit) (device.mode_setting & (1<<bit))

int volatile change_mod;
unsigned char line1[];
unsigned char line2[];
unsigned char line3[];
unsigned char dot[];
unsigned char date_var,flaga,counter,result;
unsigned char *data_received_UART;
/*
16 bitowy config:
1bit- 1 bluetooth, 0 bez bluetooth
2bit-1-tryb led
3bit-1-tryb switch
4bit-1-LCD
... - mozna rozwinac w przyszlosci o pozostale funkcje
8-16bit:ktore ledy maj? palic, ktory przycisk ma byc wlaczony, 0000 w przypadku LCD
na przyklad:
wlacz LED 0,1,2 na bluetooth:
000001110101

*/

typedef struct
{
	unsigned char usart_data;
	uint8_t clocks;
	uint16_t mode_setting;
	uint8_t bit_counter;
	uint8_t flag;
	uint8_t counter_led;
	
}parameters_t;

#define MODESET(bit) device.mode_setting|=(1<<bit)
#define MODERESET(bit) device.mode_setting&=~(1<<bit)
#define ISMODESET(bit) (device.mode_setting & (1<<bit))

parameters_t device;
//void bluetooth_Init(void);
void avr_init(void);
//void send7SEG(unsigned char);
//void send8LED(unsigned char);
unsigned char ReadDip8(void);
//void serviceSW2();
//void serviceSW3();
void LCD_view();

/* *** deklaracje zmiennych  *** */
unsigned char result;

//5ms clock flag
#define CLOCK100US      0
//25ms clock flag
#define CLOCK1MS		1
//50ms clock flag
#define CLOCK50MS       2
//100ms clock flag
#define CLOCK100MS      3
//1s clock flag
#define CLOCK1S         4
//2s clock flag
#define CLOCK2S         5

#define CLOCKFLAGSET(bit) (device.clocks |= (1<<bit))
#define CLOCKFLAGRESET(bit) (device.clocks &= ~(1<<bit))
#define ISCLOCKFLAGSET(bit) (device.clocks & (1<<bit))


