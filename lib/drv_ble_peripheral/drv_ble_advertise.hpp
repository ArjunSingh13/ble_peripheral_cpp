#include <stdint.h>
#include <etl/string.h>
#include <stdint.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

namespace ble_adv 
{
class ble_advertise {

    private:
        etl::string<30> _url{};
        static struct bt_data ad[2];  // ad is advertising data
        static struct bt_data sd[1]; // sd is scanning response data, a scanner can request it.

    public:

        explicit ble_advertise();
        ~ble_advertise() noexcept;

        /**
         * @brief Updates the advertised data with the given integer value.
         *
         * This function modifies the BLE manufacturing data to reflect the
         * specified integer, then refreshes the advertisement data broadcast.
         * 
         * @note This function only works when CONFIG_CONNECTABLE_PERIPHERAL is not true
         *
         * @param data  The integer value to insert into advertisement data.
         */
        void ble_advertise_data(uint32_t data) noexcept;

        /**
         * @todo: maybe make two functions: adverise_connect, advertise_no_connect.
         * 
         * @todo: make it a singleton class. 
         * 
         */
}; 
}