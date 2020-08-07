/*
 * test.c
 *
 * Created: 7/25/2020 1:07:31 AM
 * Author : 20112
 */ 

	#include "ACD.h"
	#include "Servo_Motor.h"
	#include "DIO.h"
	#include "LCD.h"
	#define F_CPU 16000000
	#include "util/delay.h "
    float32 val=0;

int main(void)
{
    /* Replace with your application code */
	
	LCD_Init();
	
	DIO_SetPinDir(DIO_PORTA ,DIO_PIN0 , DIO_PIN_INPUT);
	
	DIO_SetPinDir(DIO_PORTD ,DIO_PIN5 , DIO_PIN_OUTPUT);
	
	ADC_INIT();
	
	timer1_init();
	
	timer1_start();

    while (1) 
    {
		
		ADC_READ (&val);               //from potintumeter to indecate the angel 
	    Servo_Get_Angle(val*180/1023);
		LCD_WriteString("Degree:");
		LCD_WriteInteger(val*180/1023);
		_delay_ms(500);
		LCD_Clear();
	    
    }
}

