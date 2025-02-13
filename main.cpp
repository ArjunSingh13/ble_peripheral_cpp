/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Sample app to demonstrate PWM-based LED fade
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

/** Included from SDK */
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>

//#include <iostream>
#include "drv_led.hpp"
#include "drv_ble_advertise.hpp"
#include "etl/function.h"

#include <zephyr/shell/shell.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <version.h>
#include <zephyr/logging/log.h>
#include <stdint.h>
#include <zephyr/posix/unistd.h>

/** TODO: see if there is a better way than using Macros here */
#define INST_LED0 GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios)
#define INST_LED1 GPIO_DT_SPEC_GET(DT_ALIAS(led1),gpios)

int main(void)
{
	drv_led drvLed0{INST_LED0};
	drv_led drvLed1{INST_LED1};
	//ble_adv::
	ble_advertise ble_adver;


	while(1){
		k_sleep(K_FOREVER);
	}
	return 0;
}