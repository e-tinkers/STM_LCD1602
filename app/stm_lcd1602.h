/*
 * stm_lcd1602.h
 *
 *  Created on: Feb 11, 2024
 *      Author: henry cheung
 */

#ifndef STM_LCD1602_H_
#define STM_LCD1602_H_

#include "stm32g0xx_hal.h"

/*
 * When power on for the first time, the LCD module is set to:
 *  - Display clear
 *  - Function: 8-bit mode, 1-line, 5x8 font
 *  - Display: off, no cursor, no blinking
 *  - Mode, increase to the right, no screen shift
 *  Reset MCU does not reset LCD module, so initialization of LCD is required based on actual needs
 *
 * RS: data/command. 0 = command mode, 1 = data mode
 * RW: read/write. 0 = write, 1 = read. Ground this pin if only write is perform on the LCD module.
 * EN: clocking input. The data is latched on the falling edge at this pin.
 */

// commands
#define LCD_CLEAR_DISPLAY         0x01
#define LCD_RETURN_HOME           0x02
#define LCD_ENTRY_MODE_SET        0x04
#define LCD_DISPLAY_CONTROL       0x08
#define LCD_CURSOR_SHIFT          0x10
#define LCD_FUNCTION_SET          0x20
#define LCD_SET_CGRAM_ADDR        0x40
#define LCD_SET_DDRAM_ADDR        0x80

// flags for display entry mode
#define LCD_ENTRY_RIGHT           0x00
#define LCD_ENTRY_LEFT            0x02
#define LCD_ENTRY_SHIFT_DISPLAY   0x01
#define LCD_ENTRY_SHIFT_CURSOR    0x00

// flags for display on/off control
#define LCD_DISPLAY_ON            0x04
#define LCD_DISPLAY_OFF           0x00
#define LCD_CURSOR_ON             0x02
#define LCD_CURSOR_OFF            0x00
#define LCD_BLINK_ON              0x01
#define LCD_BLINK_OFF             0x00

// flags for display/cursor shift
#define LCD_DISPLAY_MOVE          0x08
#define LCD_CURSOR_MOVE           0x00
#define LCD_MOVE_RIGHT            0x04
#define LCD_MOVE_LEFT             0x00

// flags for function set
#define LCD_8BIT_MODE             0x10
#define LCD_4BIT_MODE             0x00
#define LCD_2LINE                 0x08
#define LCD_1LINE                 0x00
#define LCD_5x10_DOTS             0x04
#define LCD_5x8_DOTS              0x00

typedef struct {
	GPIO_TypeDef * port;
	uint16_t d4;
	uint16_t d5;
	uint16_t d6;
	uint16_t d7;
	uint16_t en;
	uint16_t rs;
} LCD_t;

void lcd_config(void);
void lcd_write(uint8_t value, uint8_t rs);
void lcd_cmd(uint8_t cmd);
void lcd_data(char data);
void lcd_clear();
void lcd_init(uint8_t cursor, uint8_t blink);
void lcd_cursor(uint8_t col, uint8_t row);
void lcd_print(char c);
void lcd_print_str(char* str);
void lcd_create_char(uint8_t addr, const char* font);

#endif /* STM_LCD1602_H_ */
