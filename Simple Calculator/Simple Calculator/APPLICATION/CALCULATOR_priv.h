/**
 ******************************************************************************
 * @file           : CALCULATOR_priv.h
 * @author         : Mostafa Edrees
 * @brief          : private file of the Calculator.
 * @date           : 9/11/2023
 * @board          : ATMEGA32
 ******************************************************************************
**/

#ifndef CALCULATOR_PRIV_H_
#define CALCULATOR_PRIV_H_

//Basic Operations
#define CALCULATOR_EQUAL							'='
#define CALCULATOR_ADD								'+'
#define CALCULATOR_SUB								'-'
#define CALCULATOR_MULTI							'*'
#define CALCULATOR_DIV								'/'

//Numbers
#define NUMBER_0									'0'
#define NUMBER_1									'1'
#define NUMBER_2									'2'
#define NUMBER_3									'3'
#define NUMBER_4									'4'
#define NUMBER_5									'5'
#define NUMBER_6									'6'
#define NUMBER_7									'7'
#define NUMBER_8									'8'
#define NUMBER_9									'9'

void CALCULATOR_vidCalcResult(void);
void CALCULATOR_vidRestart(void);
void CALCULATOR_vidGetOP_OPCODE(void);
void CALCULATOR_vidGetResultPosition(s32 Copy_s32Res);

#endif /* CALCULATOR_PRIV_H_ */