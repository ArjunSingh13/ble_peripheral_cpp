#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#include <stdint.h>
#include <etl/array.h>
#include <etl/string.h>
#include <stdint.h>
#include <array>

//namespace ble_adv 
//{
    class ble_advertise {

        private:
            static struct bt_data ad[2];
            static struct bt_data sd[1];
            etl::string<30> _url{};
            uint8_t _deviceNameSize{};
        public:

            explicit ble_advertise(void);
            ~ble_advertise() noexcept;
            void ble_advertise_start(void) const noexcept;
    }; 
//}