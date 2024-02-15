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
	.rs = GPIO_PIN_9,
	.en = GPIO_PIN_8,
	.d4 = GPIO_PIN_4,
	.d5 = GPIO_PIN_5,
	.d6 = GPIO_PIN_6,
	.d7 = GPIO_PIN_7
};

#endif /* LCD_CONFIG_H_ */
