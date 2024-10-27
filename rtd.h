#pragma once

#ifndef RTD_H
#define RTD_H

#include <stdint.h>
#include "tl_common.h"
#include "app_config.h"
#include "voltages.h"
#include "drivers.h"


int rtd_2w_measure();
int rtd_3w_measure();

#endif