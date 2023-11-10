/**
 ******************************************************************************
 * @file           : LCD_int.h
 * @author         : Mostafa Edrees
 * @brief          : interface file of LCD.
 * @date           : 11/1/2023
 * @board          : ATMEGA32
 ******************************************************************************
**/

#ifndef LCD_INT_H_
#define LCD_INT_H_

ES_t LCD_enuInit(void);

ES_t LCD_enuDisplayChar(u8 Copy_u8Data);

ES_t LCD_enuSendCommand(u8 Copy_u8Command);

ES_t LCD_enuGoToPosition(u8 Copy_u8Line, u8 Copy_u8Position);

ES_t LCD_enuWriteString(u8 *Copy_pu8Str);

ES_t LCD_enuWriteNumber(int Copy_s32Number);

void LCD_vidClearScreen(void);


#endif /* LCD_INT_H_ */