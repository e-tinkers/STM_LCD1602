/*
 * lcd_config.h
 *
 *  Created on: Feb 15, 2024
 *      Author: henry cheung
 */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

LCD_t lcd = {
	.port = GPIOB,
	.rs = 9,
	.en = 8,
	.d4 = 4,
	.d5 = 5,
	.d6 = 6,
	.d7 = 7
};

#endif /* LCD_CONFIG_H_ */
