/*
 * Servo_Motor.c
 *
 * Created: 7/24/2020 9:20:07 PM
 *  Author: 20112
 */ 
#include "Servo_Motor.h"
#include "Timer_Cfg.h"

void Servo_Get_Angle(float32 angel)
{
	PWM1_ICR_Value((angel/36)+5 , (16*1000000)/(50*Timer_CLK ));
	
}