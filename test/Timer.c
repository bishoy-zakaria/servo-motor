/*
 * Timer.c
 *
 * Created: 7/17/2020 7:12:18 PM
 *  Author: 20112
 */ 
#include "Timer.h"
#include "Timer_Cfg.h"
#include <avr/interrupt.h>
uint32 num_over_flow;
uint8  init_value;
uint32 Compare_Match;
volatile uint8 count=0;
static void (*Timer0_Ptr)(void);

void timer0_init(void)
{
	CLR_BIT(TCCR0,7);
	
	#if     Timer_Mode    ==       TIMER0_Normal_Mode
	{
		CLR_BIT(TCCR0,6);
		CLR_BIT(TCCR0,3);
		
		#if interrupt_state  ==    interrupt_Enable 
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,0);
		}
		#endif
	}
	#elif   Timer_Mode    ==       TIMER0_CTC_Mode
	{
		CLR_BIT(TCCR0,6);
		SET_BIT(TCCR0,3);
		
		#if interrupt_state  ==    interrupt_Enable
		{
			SET_BIT(SREG ,7);
			SET_BIT(TIMSK,1);
		}
		#endif
	}
	
	#elif   Timer_Mode    ==       TIMER0_Fast_PWM_Mode 
	{
		SET_BIT(TCCR0,6);
		SET_BIT(TCCR0,3);
		
		#if   PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			CLR_BIT(TCCR0,4);
		}
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			SET_BIT(TCCR0,4);
		}
		#endif
	}
	#elif   Timer_Mode    ==       TIMER0_PWM_Phase_Correct_Mode
	{
		SET_BIT(TCCR0,6);
		CLR_BIT(TCCR0,3);
		
		#if   PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			CLR_BIT(TCCR0,4);
		}
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR0,5);
			SET_BIT(TCCR0,4);
		}
		#endif
	}
	#endif
	
}



void timer0_start(void)
{
	
	  #if  Timer_CLK     ==       Pres_CLK   
  {
	  SET_BIT(TCCR0,0);
	  CLR_BIT(TCCR0,1);
	  CLR_BIT(TCCR0,2);
  } 
  #elif  Timer_CLK     ==       Pres_CLK_8 
  {
	  CLR_BIT(TCCR0,0);
	  SET_BIT(TCCR0,1);
	  CLR_BIT(TCCR0,2);
  }
  #elif  Timer_CLK     ==       Pres_CLK_64
  {
      SET_BIT(TCCR0,0);
      SET_BIT(TCCR0,1);
	  CLR_BIT(TCCR0,2);
  }
 #elif  Timer_CLK     ==       Pres_CLK_256
 {
	 CLR_BIT(TCCR0,0);
	 CLR_BIT(TCCR0,1);
	 SET_BIT(TCCR0,2);
 } 
 #elif  Timer_CLK     ==      Pres_CLK_1024
 {
	 SET_BIT(TCCR0,0);
	 CLR_BIT(TCCR0,1);
	 SET_BIT(TCCR0,2);
 }
 #endif
 
}

void timer0_stop(void)
{
	CLR_BIT(TCCR0,0);
	CLR_BIT(TCCR0,1);
	CLR_BIT(TCCR0,2);
	
}


void timer0_delay(uint32 delay_ms )
{
	#if Timer_Mode     ==      TIMER0_Normal_Mode
	{		
	  uint8 tick_time = Timer_CLK /Freq;    // micro second
		
	  uint32 num_tick = (delay_ms*1000)/tick_time;
		
	  num_over_flow = num_tick/256;
		
	  init_value = 256 -(   (  (float32) num_tick/256 - num_over_flow  ) * 256  );
		
	  TCNT0 = init_value;
	
	  num_over_flow++;
	}
	
	#elif Timer_Mode     ==      TIMER0_CTC_Mode 
	{
		 uint8 tick_time = Timer_CLK /Freq;    // micro second
		 
		 uint32 num_tick = (delay_ms*1000)/tick_time;
		 
		 Compare_Match = num_tick/255;
		 
		 init_value =    ((  (float32) num_tick/255 - Compare_Match  ) * 255)-1;
		 
		 OCR0 = init_value;
		 
		 Compare_Match++;
	}
	#endif
}

void timer0_Set_CallBack(void (*ptr)(void))
{
	Timer0_Ptr = ptr;
}

/* ******************************** TIMER1 ************************************/
void timer1_init(void)
{
	#if  Timer_Mode    ==      TIMER1_Normal_Mode
	{
		CLR_BIT(TCCR1A,0);
		CLR_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		CLR_BIT(TCCR1B,4);
	}
	
	#elif Timer_Mode    ==      TIMER1_CTC_Mode
	{
		CLR_BIT(TCCR1A,0);
		CLR_BIT(TCCR1A,1);
		SET_BIT(TCCR1B,3);
		CLR_BIT(TCCR1B,4);
	}
	
	#elif Timer_Mode    ==      TIMER1_PWM_Phase_Correct_Mode
	{
		SET_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
	}
	
	#elif Timer_Mode    ==      TIMER1_PWM_Phase_Correct_ICR_Mode
	{
		CLR_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
	}
	
	#elif Timer_Mode    ==      TIMER1_Fast_PWM_Mode
	{
		SET_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		SET_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
		#if   PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			CLR_BIT(TCCR1A,6);
		}
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			SET_BIT(TCCR1A,6);
		}
		#endif
	}
	
	#elif Timer_Mode    ==      TIMER1_Fast_PWM_ICR_Mode
	{
		CLR_BIT(TCCR1A,0);
		SET_BIT(TCCR1A,1);
		SET_BIT(TCCR1B,3);
		SET_BIT(TCCR1B,4);
		#if   PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			CLR_BIT(TCCR1A,6);
		}
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			SET_BIT(TCCR1A,7);
			SET_BIT(TCCR1A,6);
		}
		#endif
	}
	#endif
}

void timer1_start(void)
{
	 #if  Timer_CLK     ==       Pres_CLK
	 {
		 SET_BIT(TCCR1B,0);
		 CLR_BIT(TCCR1B,1);
		 CLR_BIT(TCCR1B,2);
	 }
	 #elif  Timer_CLK     ==       Pres_CLK_8
	 {
		 CLR_BIT(TCCR1B,0);
		 SET_BIT(TCCR1B,1);
		 CLR_BIT(TCCR1B,2);
	 }
	 #elif  Timer_CLK     ==       Pres_CLK_64
	 {
		 SET_BIT(TCCR1B,0);
		 SET_BIT(TCCR1B,1);
		 CLR_BIT(TCCR1B,2);
	 }
	 #elif  Timer_CLK     ==       Pres_CLK_256
	 {
		 CLR_BIT(TCCR1B,0);
		 CLR_BIT(TCCR1B,1);
		 SET_BIT(TCCR1B,2);
	 }
	 #elif  Timer_CLK     ==      Pres_CLK_1024
	 {
		 SET_BIT(TCCR1B,0);
		 CLR_BIT(TCCR1B,1);
		 SET_BIT(TCCR1B,2);
	 }
	 #endif
	 
 }

 void timer1_stop(void)
 {
	 CLR_BIT(TCCR1B,0);
	 CLR_BIT(TCCR1B,1);
	 CLR_BIT(TCCR1B,2);
 }
 

void timer1_delay(uint32);

void timer1_Set_CallBack(void (*ptr)(void));



/* ************************** PWM0 ************************ */

void PWM0_Value(uint16 Duty_Cycle)
{
	#if Timer_Mode    ==       TIMER0_Fast_PWM_Mode 
	{
		#if PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR0 = (Duty_Cycle * 256 / 100) - 1;
		}
		
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			OCR0 = 255-(Duty_Cycle * 256 /100);
		}
		#endif 
	}
	
	#elif Timer_Mode    ==       TIMER0_PWM_Phase_Correct_Mode
	{
		#if PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR0 = (Duty_Cycle * 256 / 100);
		}
		
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			OCR0 = 255-(Duty_Cycle * 255 /100);
		}
		#endif
	}
	#endif
}
/* ************************** PWM1 ***************************** */

void PWM1_Value(uint32 Duty_Cycle)
{
	
	#if Timer_Mode    ==       TIMER1_Fast_PWM_Mode
	{
		#if PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * 65536 / 100) - 1;
		}
		
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = 65535-(Duty_Cycle * 65536 /100);
		}
		#endif
	}
	
	#elif Timer_Mode    ==       TIMER1_PWM_Phase_Correct_Mode
	{
		#if PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * 35536 / 100);
		}
		
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = 65535-(Duty_Cycle * 65535 /100);
		}
		#endif
	}
	#endif
	
}

void PWM1_ICR_Value(float32 Duty_Cycle,uint16 Top_Value)
{
	ICR1=Top_Value;
	
	#if Timer_Mode    ==      TIMER1_Fast_PWM_ICR_Mode
	{
		#if PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * (Top_Value+1) / 100) - 1;
		}
		
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = Top_Value-(Duty_Cycle * (Top_Value+1) /100);
		}
		#endif
	}
	
	#elif Timer_Mode    ==       TIMER1_PWM_Phase_Correct_ICR_Mode
	{
		#if PWM_Mode    ==   PWM_NON_Inverted_Mode
		{
			OCR1A  = (Duty_Cycle * (Top_Value+1) / 100);
		}
		
		#elif PWM_Mode    ==   PWM_Inverted_Mode
		{
			OCR1A  = Top_Value-(Duty_Cycle * Top_Value /100);
		}
		#endif
	}
	#endif
	
}

void PWM1_Set_CallBack(void (*ptr)(void));



/* **************************** ISR ************************ */

ISR (TIMER0_OVF_vect)
{
	count++;
	while (count == num_over_flow)
	{
		(*Timer0_Ptr)();
		TCNT0 = init_value;
		count=0;
	}
}

ISR(TIMER0_COMP_vect)
{
	count++;
	while (count == Compare_Match)
	{
		(*Timer0_Ptr)();
		OCR0 = init_value;
		count=0;
	}
	
}