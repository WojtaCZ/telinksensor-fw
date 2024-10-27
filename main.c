#include <stdint.h>
#include "tl_common.h"
#include "app_config.h"
#include "voltages.h"
#include "drivers.h"


#include "bmp390.h"
#include "sht40.h"
#include "voltages.h"
#include "rtd.h"

extern void app_uart_irq_proc(void);
extern void app_uart_init(void);
extern void app_uart_loop(void);
extern uint16_t get_adc_mv(uint32_t p_ain);

extern void adc_channel_init(ADC_InputPchTypeDef p_ain);	
void user_init()
{
	//Initialize the LED GPIO
	gpio_set_func(GPIO_LED, AS_GPIO);
	gpio_set_output_en(GPIO_LED, 1);
	gpio_set_input_en(GPIO_LED, 0); 
	gpio_write(GPIO_LED, 0);

	//Initialize button GPIO
	gpio_set_func(GPIO_SW, AS_GPIO);
	gpio_set_output_en(GPIO_SW, 0);
	gpio_set_input_en(GPIO_SW, 1); 	

	//Initialize sensor power GPIO
	gpio_set_func(GPIO_SENSOR_ON, AS_GPIO);
	gpio_set_output_en(GPIO_SENSOR_ON, 1);
	gpio_set_input_en(GPIO_SENSOR_ON, 0); 	

	//Initialize RTD power GPIO
	gpio_set_func(GPIO_RTD_ON, AS_GPIO);
	gpio_set_output_en(GPIO_RTD_ON, 1);
	gpio_set_input_en(GPIO_RTD_ON, 0); 

}


void system_init()
{
	blc_pm_select_internal_32k_crystal();

	cpu_wakeup_init();

	//int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);


	clock_init(SYS_CLK_24M_Crystal);

}

int temp, humi, v1, v2, v3;

_attribute_ram_code_ int main (void)    //must run in ramcode
{
	system_init();

	user_init();

	app_uart_init();

    irq_enable();

	// Init I2C at 100kHz
    i2c_gpio_set(I2C_GPIO_GROUP_C0C1);
	i2c_master_init(0, 60);

	bmp390_init();
	sht40_init();


	while (1) 
	{
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
	sht40_get_measurements(&temp, &humi);
	printf("[SHT40]      Temperature: %d.%02d C   Humidity: %d.%02d %%\n\r", temp/1000, temp - ((int)temp/1000)*1000, humi/1000, humi - ((int)humi/1000)*1000);
	printf("[Voltages]   1V2: %d mV   VDD: %d mV\n\r", get_1v2_mv(), get_vdd_mv());
	printf("[Resistance]   2W: %d Ohm\n\r", rtd_2w_measure());

	gpio_write(GPIO_LED, 1);
	sleep_ms(100);
	gpio_write(GPIO_LED, 0);
	sleep_ms(5000);

	}
}


_attribute_ram_code_ void irq_handler(void)
{
	app_uart_irq_proc();
}

