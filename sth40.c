#include "app_config.h"
#include "sht40.h"
#include "drivers.h"

unsigned char sht40_init(){
    // Enable the sensor power
    gpio_write(GPIO_SENSOR_ON, 1);
	sleep_ms(500);
    int chipid = sht40_get_id();

	if(chipid != 0 && chipid != 0xffffffff){
		printf("Detected SHT40! Chip ID: %x\n\r", chipid);
        return 1;
	}

    printf("Failed to detect SHT40!\n\r");

    return 0;
}

void sht40_get_measurements(int * temperature, int * humidity){
    //Setup the I2C slave address
    reg_i2c_id = SHT40_ADD;
    unsigned char data[6]; 
    sht40_read(0xFD, &data[0], 6);

    *temperature = (-45000 + 175 * ((((data[0] << 8) | data[1])*1000) / 65535));
    *humidity = (-6000 + 125 * ((((data[3] << 8) | data[4])*1000) / 65535));
}

int sht40_get_id(){
    //Setup the I2C slave address
    reg_i2c_id = SHT40_ADD;
    unsigned char data[6]; 
    sht40_read(0x89, &data[0], 6);
    return (data[0] << 24) | (data[1] << 16) | (data[3] << 8) | data[4];
}


// Special I2C read function as the STH40 needs stop conditions between reads and writes
void sht40_read(unsigned int Addr, unsigned char * dataBuf, int dataLen)
{

	reg_i2c_id	 &= (~FLD_I2C_WRITE_READ_BIT); //SlaveID & 0xfe,.i.e write data. R:High  W:Low

	reg_i2c_adr = (unsigned char)Addr; //address
	//lanuch start /id/04    start
	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_ADDR | FLD_I2C_CMD_START);
	
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);

    reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);

    sleep_ms(100);

	//start + id(Read)
	reg_i2c_id	 |= FLD_I2C_WRITE_READ_BIT;  //SlaveID & 0xfe,.i.e write data. Read:High  Write:Low
	reg_i2c_ctrl = (FLD_I2C_CMD_ID | FLD_I2C_CMD_START);
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);

	//read data
	unsigned int bufIndex = 0;

	dataLen--;    //the length of reading data must larger than 0
	//if not the last byte master read slave, master wACK to slave
	while(dataLen){  //
		reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID);
		while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
		dataBuf[bufIndex] = reg_i2c_di;
		bufIndex++;
		dataLen--;
	}
	//when the last byte, master will ACK to slave
	reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID | FLD_I2C_CMD_ACK);
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
	dataBuf[bufIndex] = reg_i2c_di;

	//termiante
	reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
}

