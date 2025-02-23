#ifndef DRV_LED_HPP
#define DRV_LED_HPP

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

class drv_led {

    private:  
        const struct gpio_dt_spec _led;
        int rc;
    public:
        explicit drv_led(const struct gpio_dt_spec& ledInfo) noexcept; // dont allow implicit conversions
        ~drv_led() noexcept;
        int drv_led_on(void);
        int drv_led_off(void);
        int drv_led_toggle(void);

        /**
         * @todo we dont need to follow rule of 6 for this class, we are not going to copy, move. 
         * so we can Explicitly delete copy and move operations to make intent clear.
         */
};

#endif // DRV_LED_HPP