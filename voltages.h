#pragma once

#ifndef VOLTAGES_H
#define VOLTAGES_H

#include <stdint.h>
#include "tl_common.h"
#include "app_config.h"
#include "voltages.h"
#include "drivers.h"

uint16_t get_1v2_mv();
uint16_t get_vdd_mv();
uint16_t get_adc_raw();
void adc_channel_init(ADC_InputPchTypeDef p_ain);
#endif