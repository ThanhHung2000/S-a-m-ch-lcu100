/*
 * Test_ADC_REFCTRL.c
 * Ki?m tra thanh ghi c?u hình ADCA.REFCTRL
 */ 

#include <avr/io.h>
#define F_CPU 2000000UL
#include <util/delay.h>

// ??nh ngh?a ?i?n áp tham chi?u chu?n c?p vào chân AREFB (Ví d? 2.5V ho?c 3.3V tùy ph?n c?ng trên board)
#define VREF_AREFB 2.7f

// 1. Kh?i t?o ADC v?i VREF t? PORT B
void ADC_Init(void) {
	// S? d?ng ?i?n áp tham chi?u ngoài t? chân AREFB (PORT B) chu?n theo code c? (0x31)
	ADCA.REFCTRL = ADC_REFSEL_AREFB_gc ;
	
	// ?? phân gi?i 12-bit Unsigned
	ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc ;
	
	// Prescaler chia 32 (ho?c ADC_PRESCALER_DIV512_gc n?u mu?n gi?ng h?t thanh ghi c?)
	ADCA.PRESCALER = ADC_PRESCALER_DIV32_gc;
	
	// B?t b? ADCA
	ADCA.CTRLA = ADC_ENABLE_bm;
	_delay_ms(1);
}

// 2. Hàm ??c giá tr? ADC t? chân ch? ??nh
uint16_t ADC_Read(uint8_t pin) {
	uint16_t timeout = 10000;
	
	// C?u hình ngõ vào Single-Ended
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL = (pin << 3);
	ADCA.CH0.CTRL |= ADC_CH_START_bm;

	while (!(ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm) && --timeout);

	if (ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm) {
		ADCA.CH0.INTFLAGS = ADC_CH_CHIF_bm;
	}

	return ADCA.CH0.RES;
}

// 3. Hàm main
int main(void) {
	volatile uint16_t adc_value = 0;
	volatile float voltage = 0.0;

	ADC_Init();

	while (1) {
		// ??c giá tr? t?i chân PA0 (Chân 44)
		adc_value = ADC_Read(0);

		// Công th?c tính ?i?n áp th?c t? d?a trên VREF ngo?i t? PORT B (AREFB)
		voltage = ((float)adc_value * VREF_AREFB) / 4095.0f;

		// _delay_ms(100);
	}
}