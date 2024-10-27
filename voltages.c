#include "voltages.h"


#define ADC_BUF_COUNT	8

// Process takes about 120 μs at CPU CLK 24Mhz.
_attribute_ram_code_
void adc_channel_init(ADC_InputPchTypeDef p_ain) {
	adc_set_sample_clk(5);
	adc_set_left_right_gain_bias(GAIN_STAGE_BIAS_PER100, GAIN_STAGE_BIAS_PER100);
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);
	adc_set_state_length(240, 0, 10);
	analog_write(anareg_adc_res_m, RES14 | FLD_ADC_EN_DIFF_CHN_M);
	adc_set_ain_chn_misc(p_ain, GND);
	adc_set_ref_voltage(ADC_MISC_CHN, ADC_VREF_1P2V);
	adc_set_tsample_cycle_chn_misc(SAMPLING_CYCLES_6);
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F2);
}

// Process takes about 260 μs at CPU CLK 24Mhz.
_attribute_ram_code_
uint16_t get_adc_raw() { // ADC_InputPchTypeDef
	volatile unsigned int adc_dat_buf[ADC_BUF_COUNT];
	uint16_t temp;
	int i, j;

	adc_power_on_sar_adc(1); // + 0.4 mA
	adc_reset_adc_module();
	u32 t0 = clock_time();

	uint16_t adc_sample[ADC_BUF_COUNT]; // = { 0 };
	u32 adc_average;
	for (i = 0; i < ADC_BUF_COUNT; i++) {
		adc_dat_buf[i] = 0;
	}

	while (!clock_time_exceed(t0, 25)); //wait at least 2 sample cycle(f = 96K, T = 10.4us)
	adc_config_misc_channel_buf((uint16_t *) adc_dat_buf, sizeof(adc_dat_buf));
	dfifo_enable_dfifo2();
	sleep_us(20);
	for (i = 0; i < ADC_BUF_COUNT; i++) {

		while (!adc_dat_buf[i]);

		if (adc_dat_buf[i] & BIT(13)) {
			adc_sample[i] = 0;
		} else {

			adc_sample[i] = ((uint16_t) adc_dat_buf[i] & 0x1FFF);
		}

		if (i) {
			if (adc_sample[i] < adc_sample[i - 1]) {
				temp = adc_sample[i];
				adc_sample[i] = adc_sample[i - 1];
				for (j = i - 1; j >= 0 && adc_sample[j] > temp; j--) {
					adc_sample[j + 1] = adc_sample[j];
				}
				adc_sample[j + 1] = temp;
			}
		}
	}
	dfifo_disable_dfifo2();
	adc_power_on_sar_adc(0); // - 0.4 mA

	adc_average = (adc_sample[2] + adc_sample[3] + adc_sample[4] + adc_sample[5]) / 4;

	return adc_average; // adc_vref default: 1175 (mV)
}

uint16_t get_1v2_mv(){
    adc_channel_init(B6P);
	return (get_adc_raw() * 2350) >> 12;
}

uint16_t get_vdd_mv(){
    adc_channel_init(B7P);
	return (get_adc_raw() * 3760) >> 12;
}