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

void gpio_config() {
	// enable GPIO PORTB clock
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	// Reset pins and set to OUTPUT
	(lcd.port)->MODER &= ~((GPIO_MODER_MODE4) | (GPIO_MODER_MODE5) | (GPIO_MODER_MODE6) | (GPIO_MODER_MODE7) | (GPIO_MODER_MODE8) | (GPIO_MODER_MODE9));
    (lcd.port)->MODER |= ((GPIO_MODER_MODE4_0) | (GPIO_MODER_MODE5_0) | (GPIO_MODER_MODE6_0) | (GPIO_MODER_MODE7_0) | (GPIO_MODER_MODE8_0) | (GPIO_MODER_MODE9_0));
}

// Using HAL to toggle GPIO pins caused delay_us(1) to actually became 2us
void lcd_write(uint8_t value, uint8_t rs) {
	if(rs) // data
		(lcd.port)->BSRR = lcd.rs;
	else   // cmd
		(lcd.port)->BRR = lcd.rs;

	// set high 4-bit
	(lcd.port)->BSRR = lcd.en;
	if (value & 0x80) (lcd.port)->BSRR = lcd.d7; else (lcd.port)->BRR = lcd.d7;
	if (value & 0x40) (lcd.port)->BSRR = lcd.d6; else (lcd.port)->BRR = lcd.d6;
	if (value & 0x20) (lcd.port)->BSRR = lcd.d5; else (lcd.port)->BRR = lcd.d5;
	if (value & 0x10) (lcd.port)->BSRR = lcd.d4; else (lcd.port)->BRR = lcd.d4;
	(lcd.port)->BRR = lcd.en;
	delay_us(1);

	// set lower 4-bit
	(lcd.port)->BSRR = lcd.en;
	if (value & 0x08) (lcd.port)->BSRR = lcd.d7; else (lcd.port)->BRR = lcd.d7;
	if (value & 0x04) (lcd.port)->BSRR = lcd.d6; else (lcd.port)->BRR = lcd.d6;
	if (value & 0x02) (lcd.port)->BSRR = lcd.d5; else (lcd.port)->BRR = lcd.d5;
	if (value & 0x01) (lcd.port)->BSRR = lcd.d4; else (lcd.port)->BRR = lcd.d4;
	(lcd.port)->BRR = lcd.en;
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
