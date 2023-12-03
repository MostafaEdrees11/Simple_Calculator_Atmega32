/**
 ******************************************************************************
 * @file           : CALCULATOR_app.c
 * @author         : Mostafa Edrees
 * @brief          : application file of the Calculator.
 * @date           : 9/11/2023
 * @board          : ATMEGA32
 ******************************************************************************
**/

#include "Error_States.h"
#include "stdTypes.h"

#include "DIO_int.h"

#include "LCD_int.h"
#include "KEYPAD_int.h"

#include "CALCULATOR_priv.h"
#include "CALCULATOR_config.h"

#include <util/delay.h>

/* INITIAL_VALUE is equal zero */

/* control variable */
u8 CALCULATOR_u8OnOff = INITIAL_VALUE;

/* the basic operands & operators variables */
s32 CALCULATOR_OP1 = INITIAL_VALUE;
s32 CALCULATOR_OP2 = INITIAL_VALUE;
s32 CALCULATOR_OP = INITIAL_VALUE;
u8 CALCULATOR_OPCODE = INITIAL_VALUE;
s32 CALCULATOR_RES = INITIAL_VALUE;
u8 RES_u8Pos = INITIAL_VALUE;

/* */
s32 CALCULATOR_Help = INITIAL_VALUE;
s32 CALCULATOR_BASE = DECIMAL_BASE;
u8 CALCULATOR_KEYPAD_CHARACTER = INITIAL_VALUE;

/* It is a flag rised when we enter - to consider that the number is negative */
u8 Number_Is_Neg = 0;

/**
 ******************************************************************************
 * @Fn			: CALCULATOR_vidRun
 * @brief		: it is used to run the calculator
 * @param [in]	: none
 * @retval		: none
 * @note		: none
 ******************************************************************************
**/
void CALCULATOR_vidRun(void)
{
	/* Turn on Keypad */
	KEYPAD_enuInit();
	
	/* Loop until user press on ON/C button to turn on the Calculator */
	while(CALCULATOR_u8OnOff != CALCULATOR_ON)
	{
		CALCULATOR_u8OnOff = KEYPAD_u8GetPressedKey();
	}
	
	/* Turn on the Calculator */
	LCD_enuInit();
	LCD_enuGoToPosition(1,4);
	LCD_enuWriteString((u8 *)"Welcome");
	_delay_ms(1000);
	LCD_vidClearScreen();

	/* Get the operands and opcode of the calculator */	
	CALCULATOR_vidGetOP_OPCODE();
}

/**
 ******************************************************************************
 * @Fn			: CALCULATOR_vidGetOP_OPCODE
 * @brief		: it is used to get parameters of the operations
 * @param [in]	: none
 * @retval		: none
 * @note		: none
 ******************************************************************************
**/
void CALCULATOR_vidGetOP_OPCODE(void)
{
	/* Loop until the user enter '=' */
	while(1)
	{
		CALCULATOR_KEYPAD_CHARACTER = KEYPAD_u8GetPressedKey();
		
		switch(CALCULATOR_KEYPAD_CHARACTER)
		{
			/* opcode case */
			case CALCULATOR_ADD:
			case CALCULATOR_SUB:
			case CALCULATOR_MULTI:
			case CALCULATOR_DIV:
			if(CALCULATOR_KEYPAD_CHARACTER == '-' && !Number_Is_Neg && !CALCULATOR_OP)	/* check if the user enter negative sign */
			{
				LCD_enuDisplayChar(CALCULATOR_KEYPAD_CHARACTER);
				Number_Is_Neg = 1;										/* rise the flag to know that the number is has a '-' before it */
			}
			else
			{	
				CALCULATOR_OPCODE = CALCULATOR_KEYPAD_CHARACTER;		/* save value of opcode */
				CALCULATOR_OP1 = CALCULATOR_OP;							/* save value of op1 */
				CALCULATOR_OP = 0;										/* zero of op to use it to calculate op2 */
				LCD_enuDisplayChar(CALCULATOR_KEYPAD_CHARACTER);		/* display the opcode on lcd */
				
				if(Number_Is_Neg)										/* check the flag */
				{
					CALCULATOR_OP1 *= -1;								/* Multiply op1 by -1 because the flag is 1 */
					Number_Is_Neg = 0;									/* Down the flag */
				}
			}
			break;
			
			/* equal case */
			case CALCULATOR_EQUAL:
			CALCULATOR_OP2 = CALCULATOR_OP;							/* save value of op2 */
			LCD_enuDisplayChar(CALCULATOR_KEYPAD_CHARACTER);		/* display '=' on lcd */
			if(Number_Is_Neg)										/* check the flag */
			{
				CALCULATOR_OP2 *= -1;								/* Multiply op2 by -1 because the flag is 1 */
				Number_Is_Neg = 0;									/* Down the flag */
			}
			CALCULATOR_vidCalcResult();								/* calculate the result */
			CALCULATOR_vidRestart();								/* restart calculator when user press ON/C */
			break;
			
			/* numbers case */
			case NUMBER_0:
			case NUMBER_1:
			case NUMBER_2:
			case NUMBER_3:
			case NUMBER_4:
			case NUMBER_5:
			case NUMBER_6:
			case NUMBER_7:
			case NUMBER_8:
			case NUMBER_9:
			CALCULATOR_Help = CALCULATOR_KEYPAD_CHARACTER - NUMBER_0;						/* get the integer value of character */
			CALCULATOR_OP = (CALCULATOR_OP * CALCULATOR_BASE) + CALCULATOR_Help;			/* calculate the value if number if its more than one digit */
			LCD_enuDisplayChar(CALCULATOR_KEYPAD_CHARACTER);								/* display digit on lcd */
			break;
			
			/* this used in case of the use need to clear lcd when it enters a wrong number */
			case CALCULATOR_ON:
			CALCULATOR_vidRestart();
			break;
		}
	}
}

/**
 ******************************************************************************
 * @Fn			: CALCULATOR_vidCalcResult
 * @brief		: it is used to calculate the result of the operation
 * @param [in]	: none
 * @retval		: none
 * @note		: none
 ******************************************************************************
**/
void CALCULATOR_vidCalcResult(void)
{
	/* switch on the opcode */
	switch(CALCULATOR_OPCODE)
	{
		/* addition */
		case CALCULATOR_ADD:
		CALCULATOR_RES = (CALCULATOR_OP1 + CALCULATOR_OP2);								/* add the two operands */
		CALCULATOR_vidGetResultPosition(CALCULATOR_RES);								/* get the position of the result */
		LCD_enuGoToPosition(2,RES_u8Pos);
		LCD_enuWriteNumber(CALCULATOR_RES);												/* display the result */
		break;
		
		/* subtraction */
		case CALCULATOR_SUB:
		CALCULATOR_RES = (CALCULATOR_OP1 - CALCULATOR_OP2);								/* subtract the two operands */
		CALCULATOR_vidGetResultPosition(CALCULATOR_RES);								/* get the position of the result */
		LCD_enuGoToPosition(2,RES_u8Pos);
		LCD_enuWriteNumber(CALCULATOR_RES);												/* display the result */
		break;
		
		/* Multiplication */
		case CALCULATOR_MULTI:
		CALCULATOR_RES = (CALCULATOR_OP1 * CALCULATOR_OP2);								/* Multiply the two operands */
		CALCULATOR_vidGetResultPosition(CALCULATOR_RES);								/* get the position of the result */
		LCD_enuGoToPosition(2,RES_u8Pos);
		LCD_enuWriteNumber(CALCULATOR_RES);												/* display the result */
		break;
		
		/* Divion */
		case CALCULATOR_DIV:
		/* check if op2 = 0 because this is a math error */
		if(CALCULATOR_OP2 == 0)
		{
			_delay_ms(100);
			LCD_vidClearScreen();
			
			LCD_enuWriteString((u8 *)"MATH ERROR!!");
			LCD_enuGoToPosition(2,0);
			LCD_enuWriteString((u8 *)"Second operand 0");
			
			CALCULATOR_vidRestart();
		}
		else
		{
			s32 CALCULATOR_RES_MUL100 = 0;
			u8 CALCULATOR_RES_FRACTION = 0;
			CALCULATOR_RES = (CALCULATOR_OP1 / CALCULATOR_OP2);										/* calculate the integer part */
			CALCULATOR_RES_MUL100 = ((CALCULATOR_OP1 * 100) / CALCULATOR_OP2);						/* calculate the result multiply by 100 */
			CALCULATOR_RES_FRACTION = CALCULATOR_RES_MUL100 - (CALCULATOR_RES * 100);				/* calculate the fraction part */
			CALCULATOR_vidGetResultPosition(CALCULATOR_RES);
			LCD_enuGoToPosition(2,RES_u8Pos-3);														/* get the position of the result */
			LCD_enuWriteNumber(CALCULATOR_RES);														/* display the integer part */
			LCD_enuDisplayChar('.');																/* display '.' */
			LCD_enuWriteNumber(CALCULATOR_RES_FRACTION);											/* display the fraction part */
		}
		break;
	}
	
}

/**
 ******************************************************************************
 * @Fn			: CALCULATOR_vidGetResultPosition
 * @brief		: it is used to get the position of the result to print it at the end of 2nd line
 * @param [in]	: Copy_s32Res --> it is the value of the result.
 * @retval		: none
 * @note		: none
 ******************************************************************************
**/
void CALCULATOR_vidGetResultPosition(s32 Copy_s32Res)
{
	/* check the value of the result */
	if(Copy_s32Res > 0)
		RES_u8Pos = 16 - floor(log10(Copy_s32Res)+1);				/* log10(num) --> return (number of digit - 1) but in float form */
	else if(Copy_s32Res < 0)
		RES_u8Pos = 16 - (floor(log10(-Copy_s32Res)+1)+1);			/* Don't forget '-' sign */
	else
		RES_u8Pos = 15;
}

/**
 ******************************************************************************
 * @Fn			: CALCULATOR_vidRestart
 * @brief		: it is used to restart the calculator
 * @param [in]	: none
 * @retval		: none
 * @note		: none
 ******************************************************************************
**/
void CALCULATOR_vidRestart(void)
{
	/* reinitialize the variables */
	CALCULATOR_OP1 = 0;
	CALCULATOR_OP2 = 0;
	CALCULATOR_OP = 0;
	CALCULATOR_OPCODE = 0;
	CALCULATOR_Help = 0;
	
	while(CALCULATOR_KEYPAD_CHARACTER != CALCULATOR_ON)
	{
		CALCULATOR_KEYPAD_CHARACTER = KEYPAD_u8GetPressedKey();
		
		/* this in case of we need to complete more operations on it */
		if(CALCULATOR_KEYPAD_CHARACTER == '+' ||
			CALCULATOR_KEYPAD_CHARACTER == '-' ||
			CALCULATOR_KEYPAD_CHARACTER == '*' ||
			CALCULATOR_KEYPAD_CHARACTER =='/')
		{
			CALCULATOR_OP1 = CALCULATOR_RES;							/* last result is the 1st operand */
			LCD_vidClearScreen();										/* clear the screen */
			LCD_enuWriteNumber(CALCULATOR_OP1);							/* display op1 on lcd */
			CALCULATOR_OPCODE = CALCULATOR_KEYPAD_CHARACTER;			/* save value of opcode */
			LCD_enuDisplayChar(CALCULATOR_KEYPAD_CHARACTER);			/* display opcode on lcd */
			CALCULATOR_vidGetOP_OPCODE();								/* get the value of the 2nd operand */
		}
	}
	LCD_vidClearScreen();
	CALCULATOR_RES = 0;
}