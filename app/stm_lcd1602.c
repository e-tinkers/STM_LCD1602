/*
 * stm_lcd1602.c
 *
 *  Created on: Feb 11, 2024
 *      Author: henry cheung
 */
#include <stdint.h>
#include "stm32g0xx.h"
#include "stm32g071xx.h"
#include "stm_lcd1602.h"
#include "lcd_config.h"
#include "utility.h"

extern LCD_t lcd;

void lcd_config() {
	// enable GPIO PORTB clock
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	// Reset pins and set to OUTPUT
	(lcd.port)->MODER &= ~(
			(0x3 << (lcd.d4 * 2)) |
			(0x3 << (lcd.d5 * 2)) |
			(0x3 << (lcd.d6 * 2)) |
			(0x3 << (lcd.d7 * 2)) |
			(0x3 << (lcd.en * 2)) |
			(0x3 << (lcd.rs * 2))
	);
	(lcd.port)->MODER |= (
			(0x1 << (lcd.d4 * 2)) |
			(0x1 << (lcd.d5 * 2)) |
			(0x1 << (lcd.d6 * 2)) |
			(0x1 << (lcd.d7 * 2)) |
			(0x1 << (lcd.en * 2)) |
			(0x1 << (lcd.rs * 2))
	);
}

// Using HAL to toggle GPIO pins caused delay_us(1) to actually became 2us
void lcd_write(uint8_t value, uint8_t rs) {
	if(rs) // data
		(lcd.port)->BSRR = 1 << lcd.rs;
	else   // cmd
		(lcd.port)->BSRR = 1 << (lcd.rs + 16);

	// set high 4-bit
	(lcd.port)->BSRR = 1 << lcd.en;
	if (value & 0x80) (lcd.port)->BSRR = 1 << lcd.d7; else (lcd.port)->BSRR = 1 << (lcd.d7 + 16);
	if (value & 0x40) (lcd.port)->BSRR = 1 << lcd.d6; else (lcd.port)->BSRR = 1 << (lcd.d6 + 16);
	if (value & 0x20) (lcd.port)->BSRR = 1 << lcd.d5; else (lcd.port)->BSRR = 1 << (lcd.d5 + 16);
	if (value & 0x10) (lcd.port)->BSRR = 1 << lcd.d4; else (lcd.port)->BSRR = 1 << (lcd.d4 + 16);
	(lcd.port)->BSRR = 1 << (lcd.en +16);
	delay_us(1);

	// set lower 4-bit
	(lcd.port)->BSRR = 1 << lcd.en;
	if (value & 0x08) (lcd.port)->BSRR = 1 << lcd.d7; else (lcd.port)->BSRR = 1 << (lcd.d7 + 16);
	if (value & 0x04) (lcd.port)->BSRR = 1 << lcd.d6; else (lcd.port)->BSRR = 1 << (lcd.d6 + 16);
	if (value & 0x02) (lcd.port)->BSRR = 1 << lcd.d5; else (lcd.port)->BSRR = 1 << (lcd.d5 + 16);
	if (value & 0x01) (lcd.port)->BSRR = 1 << lcd.d4; else (lcd.port)->BSRR = 1 << (lcd.d4 + 16);
	(lcd.port)->BSRR = 1 << (lcd.en + 16);
	delay_us(37); // at least more than 37uS
}

void lcd_cmd(uint8_t cmd) {
	lcd_write(cmd, 0);
}

void lcd_data(char data) {
	lcd_write((uint8_t) data, 1);
}

void lcd_clear() {
	lcd_cmd(LCD_CLEAR_DISPLAY);
	HAL_Delay(2);
}

void lcd_init(uint8_t cursor, uint8_t blink) {
	lcd_cmd(LCD_FUNCTION_SET | LCD_8BIT_MODE);
	HAL_Delay(4);
	lcd_cmd(LCD_FUNCTION_SET | LCD_8BIT_MODE);
	delay_us(100);
	lcd_cmd(LCD_FUNCTION_SET | LCD_4BIT_MODE | LCD_2LINE | LCD_5x8_DOTS);
	lcd_cmd(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | cursor | blink);
	lcd_cmd(LCD_CLEAR_DISPLAY);
	HAL_Delay(2);
	lcd_cmd(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_CURSOR);
}

void lcd_cursor(uint8_t row, uint8_t col) {
	if (row > 2) row = 2;
	if (col > 16) col = 16;
	lcd_cmd(LCD_SET_DDRAM_ADDR | ((row - 1) << 6) | (col - 1));
}

void lcd_print(char c) {
	lcd_data(c);
}

void lcd_print_str(char* str) {
	uint8_t n = 0;
	while(str[n]) {
		lcd_print(str[n]);
		n++;
	}
}

void lcd_create_char(uint8_t addr, const char * font) {
	lcd_cmd(LCD_SET_CGRAM_ADDR | (addr * 8));
	for (int i=0; i<8; i++) {
		lcd_data(font[i]);
	}
}
