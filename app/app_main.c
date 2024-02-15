/*
 * app_main.c
 *
 * Example program of using STM32_LCD1602 driver
 *
 *  Created on: Feb 11, 2024
 *      Author: henry cheung
 */

#include "main.h"
#include "stm_lcd1602.h"

extern void tim6_config(void);
extern void gpio_config(void);

// Maximum 8 custom char can be upload to CGRAM using lcd_create_char()
const char heart[] = {0x00, 0x00, 0x0A, 0x15, 0x11, 0x0E, 0x04, 0x00};      // Heart
const char smiley[] = {0x00, 0x00, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00};     // smiley
const char omega[] = {0x00, 0x0E, 0x11, 0x11, 0x11, 0x0A, 0x1B, 0x00};      // omega
const char plusMinus[] = {0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x1F, 0x00};  // +-
const char bell[] = {0x00, 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x04, 0x00};       // bell

void app_main() {

	gpio_config();

	tim6_config();

	HAL_Delay(50);  // ensure that the LCD internal reset is completed
	lcd_init(LCD_CURSOR_OFF, LCD_BLINK_OFF);

	lcd_create_char(0, heart);
	lcd_create_char(1, smiley);
	lcd_create_char(2, omega);
	lcd_create_char(3, plusMinus);
	lcd_create_char(4, bell);

	while(1) {
		lcd_cursor(1, 1);
		lcd_print(0); // print heart
		lcd_print_str(" Hello World ");
		lcd_print(0);
		lcd_cursor(2, 1);
		lcd_print_str("  LCD1602 demo");

		HAL_Delay(1000);
	}
}

