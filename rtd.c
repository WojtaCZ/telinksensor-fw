#include "rtd.h"
#include "voltages.h"

int rtd_2w_measure(){

	gpio_set_func2(GPIO_RTD_ON,AS_GPIO); // set tx pin

    gpio_write(GPIO_RTD_ON, 1);

    adc_channel_init(B5P);
    uint16_t rawValue = get_adc_raw(); 
    adc_channel_init(B6P);
    uint16_t raw1v2 = get_adc_raw()*2; 
    int res = (rawValue*1000) / (raw1v2 - rawValue);

    gpio_write(GPIO_RTD_ON, 0);

    gpio_set_func2(GPIO_RTD_ON,AS_UART);

    printf("%d %d\n\r", rawValue, raw1v2);

    return res;
    
}