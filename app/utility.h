/*
 * utility.h
 *
 *  Created on: Feb 12, 2024
 *      Author: henry cheung
 */

#ifndef UTILITY_H_
#define UTILITY_H_

// configure timer 6 for generating 1us clock base
void tim6_config() {
	RCC->APBENR1 |= RCC_APBENR1_TIM6EN;

	TIM6->CR1 = !TIM_CR1_CEN;    // disable timer
	TIM6->PSC = 63;              // 64000000Hz/64 = 1000000Hz
	TIM6->CNT = 0;
	TIM6->CR1 |= TIM_CR1_CEN;    // enable timer
}

// delay microsecond based on timer 6
void inline __attribute__((always_inline)) delay_us(uint16_t us) {
	uint16_t start = TIM6->CNT;
	while((TIM6->CNT - start) < us);
}

#endif /* UTILITY_H_ */
