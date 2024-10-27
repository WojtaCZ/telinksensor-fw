#pragma once

#ifndef SHT40_H
#define SHT40_H

#define SHT40_ADD             (0x44 << 1)

unsigned char sht40_init();
int sht40_get_id();
void sht40_read(unsigned int Addr, unsigned char * dataBuf, int dataLen);
void sht40_get_measurements(int * temperature, int * humidity);
#endif