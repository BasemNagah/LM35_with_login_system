/*
 * main.c
 *
 *  Created on: Aug 24, 2023
 *      Author: hp
 */
#include "MCAL/DIO/DIO_interface.h"
#include "HAL/KPD/KPAD_interface.h"
#include "HAL/CLCD/CLCD_interface.h"
#include "MCAL/ADC/ADC_interface.h"
#define F_CPU 8000000UL
#include <util/delay.h>


int main(void)
{
	//keypad
	KPAD_voidInit();


	u32 ReadValue 		;
	u32 Real_Value		;
	u32 New_Value = 0	;
	ADC_Conf_t  LDR_VALUE = { ADC_AVCC ,ADC_FCPU_64};
	ADC_voidInit(&LDR_VALUE);
	ADC_voidChooseTrigSrc(ADC_FREE_RUNNING_MODE);

	CLCD_voidInit();


	while(1)
	{
		u8 num = 0 ;
		u8 getpressed = 0 ;
		CLCD_voidSendCommand(0x01);
		u16 divison = 1000 ;
		u32 Enterd_Password = 0 ;
		CLCD_voidSetPositoion(1,5);
		CLCD_voidSendString((u8*) "Password" ,8);
		CLCD_voidSetPositoion(2,5);


		while(getpressed != '=')
		{
			getpressed = KPAD_u8GetPressed() ;
			if(getpressed != 255)
			{
				CLCD_voidSendString((u8*) "*" ,2);
				Enterd_Password +=( (getpressed-'0')*divison);
				divison/=10 ;
				if( num>3 )
				{
					divison +=10 ;
				}
				num++ ;

			}
		}
		if(Enterd_Password == 1234 )
		{
			CLCD_voidSendCommand(0x01);
			CLCD_voidSetPositoion(1,5);
			CLCD_voidSendString((u8*) "Welcome!" ,8);
			_delay_ms(1000);
			while(1)
			{
				ReadValue = ADC_u16SynchReadChannel(ADC_SINGLE_ENDED_0);
				Real_Value = (ReadValue*5000)/1024 ;
				Real_Value = Real_Value/10 ;
				if (New_Value != Real_Value )
				{
					CLCD_voidSendCommand(0x01);
					CLCD_voidSendString((u8*)"temperature = ",15);
					CLCD_voidSendNumber(Real_Value);

					New_Value = Real_Value ;
				}
			}

		}
		else
		{
			CLCD_voidSendCommand(0x01);
			CLCD_voidSetPositoion(1,2);
			CLCD_voidSendString((u8*) "Wrong password!" ,16);
			_delay_ms(1000);
		}




	}


	return 0 ;
}

