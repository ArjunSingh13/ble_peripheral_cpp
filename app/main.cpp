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

//#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

/** Included from SDK */
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>

//#include <iostream>
#include "drv_led.hpp"
#include "etl/function.h"

#define INST_LED0 GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios)

int main(void)
{
	drv_led drvLed{INST_LED0};
	drvLed.drv_led_toggle();
	return 0;
}
