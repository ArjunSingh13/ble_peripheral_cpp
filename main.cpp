/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Sample app to demonstrate PWM-based LED fade
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <version.h>
#include <stdint.h>
#include <inttypes.h>
#include <utility> 	// used for std::move()

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <zephyr/posix/unistd.h>

#include "etl/function.h"

#include "drv_led.hpp"
#include "drv_ble_advertise.hpp"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

using namespace ble_adv;

/** TODO: see if there is a better way than using Macros here */
#define INST_LED0 GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios)
#define INST_LED1 GPIO_DT_SPEC_GET(DT_ALIAS(led1),gpios)

int main(void)
{
	drv_led drvLed0{INST_LED0};
	drv_led drvLed1{INST_LED1};	

	etl::string<18> address {"FF:EE:DD:CC:BB:AA"};
	ble_advertise ble_adver(std::move(address));

	while(1){
		k_sleep(K_FOREVER);
	}
	return 0;
}