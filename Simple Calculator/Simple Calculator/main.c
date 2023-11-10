/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mostafa Edrees
 * @brief          : Simple Calculator Project.
 * @date           : 9/11/2023
 * @board          : ATMEGA32
 ******************************************************************************
**/
#define F_CPU		1000000

#include "CALCULATOR_int.h"
#include "CALCULATOR_config.h"

#include "Error_States.h"
#include "stdTypes.h"

#include "DIO_config.h"
#include "DIO_int.h"

#include "LCD_config.h"
#include "LCD_int.h"

#include "KEYPAD_config.h"
#include "KEYPAD_int.h"

int main(void)
{
	CALCULATOR_vidRun();
	//LCD_enuInit();
	
	//LCD_enuWriteNumber(12345);
	//LCD_enuGoToPosition(2,0);
	//LCD_enuWriteString("Mostafa Edrees123");
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

