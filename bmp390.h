#pragma once

#ifndef BMP390_H
#define BMP390_H

#define BMP390_ADD             (0x76 << 1)

#define BMP390_REG_CHIPID      0x00
#define BMP390_REG_REVID       0x01
#define BMP390_REG_TEMP        0x07
#define BMP390_REG_PRES        0x04

unsigned char bmp390_get_chipid();
unsigned char bmp390_get_revid();
unsigned char bmp390_init();

#endif