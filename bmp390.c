#include "app_config.h"
#include "bmp390.h"
#include "drivers.h"

unsigned char bmp390_init(){
    // Enable the sensor power
    gpio_write(GPIO_SENSOR_ON, 1);
	sleep_ms(500);
    unsigned char chipid = bmp390_get_chipid();

	if(chipid != 0 && chipid != 0xff){
		printf("Detected BMP390! Chip ID: %x, Revision: %x\n\r", chipid, bmp390_get_revid());
        return 1;
	}

    printf("Failed to detect BMP390!\n\r");

    return 0;
}

unsigned char bmp390_get_chipid(){
    //Setup the I2C slave address
    reg_i2c_id = BMP390_ADD;
    return i2c_read_byte(BMP390_REG_CHIPID, 1);
}

unsigned char bmp390_get_revid(){
    //Setup the I2C slave address
    reg_i2c_id = BMP390_ADD;

    return i2c_read_byte(BMP390_REG_REVID, 1);
}