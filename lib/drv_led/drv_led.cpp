#include <zephyr/shell/shell.h>
#include <zephyr/version.h>
#include <zephyr/logging/log.h>

#include "drv_led.hpp"
#include "etl/function.h"


LOG_MODULE_REGISTER(drv_led, LOG_LEVEL_INF);

drv_led::drv_led(const struct gpio_dt_spec& ledInfo):
        _led(ledInfo), rc{0}
{
    if (!gpio_is_ready_dt(&_led)) {
        rc = -1;
	}

    rc = gpio_pin_configure_dt(&_led, GPIO_OUTPUT_ACTIVE);
	if (rc < 0) {
		rc = -1;
	}
}

drv_led::~drv_led(void){
    printk("Destructor for led is called\r\n");
}

int drv_led::drv_led_on(void) {
    if(rc != 0) return -1;
    gpio_pin_set_dt(&_led, 1);
    if(rc < 0) return rc;
    return 0;
}

int drv_led::drv_led_off(void) {
    if(rc != 0) return -1;
    rc = gpio_pin_set_dt(&_led, 0);
    if(rc < 0) return rc;
    return 0;
}

int drv_led::drv_led_toggle(void) {
    if(rc != 0) return -1;
    rc = gpio_pin_toggle_dt(&_led);
    if(rc < 0) return rc;
    return 0;
}