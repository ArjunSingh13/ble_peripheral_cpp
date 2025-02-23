#pragma once
#include <stdint.h>
#include <etl/string.h>
#include <etl/function.h>
#include <stdint.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/conn.h>

namespace ble_adv 
{
class ble_advertise {

    private:
        etl::string<30> _url{}; 
        etl::string<18> _randomStaticAddress{};
        static struct bt_data ad[2];  // ad is advertising data
        static struct bt_data sd[1]; // sd is scanning response data, a scanner can request it.
        static struct bt_conn* my_conn;

        static void on_connected(struct bt_conn *conn, uint8_t err);
        static void on_disconnected(struct bt_conn *conn, uint8_t reason);
        static void on_le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout);
        static void on_le_phy_updated(struct bt_conn *conn, struct bt_conn_le_phy_info *param);
        static void on_le_data_len_updated(struct bt_conn *conn, struct bt_conn_le_data_len_info *info);

        /* Create variable that holds callback for MTU negotiation */
        static struct bt_gatt_exchange_params exchange_params;

        /* Forward declaration of exchange_func(): */
        static void exchange_func(struct bt_conn *conn, uint8_t att_err, struct bt_gatt_exchange_params *params);


    public:

        enum class Phy_level{
            PHY_NONE,	 
	        PHY_1M,	    /** LE 1M PHY */ 
	        PHY_2M,     /** LE 2M PHY */
	        PHY_CODED,  /** LE Coded PHY */
        };

        explicit ble_advertise(etl::string<18>&& randomStaticAddress);
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

        static int ble_update_phy(Phy_level newRxLevel, Phy_level newTxLevel);
        static int ble_update_data_length(uint16_t maxPayloadSize, uint16_t maxPayloadTime);
        static int ble_update_mtu(uint16_t& maxSize);

        /**
         * @todo: maybe make two functions: adverise_connect, advertise_no_connect.
         * 
         * @todo: make it a singleton class. 
         * 
         */
}; // ble_advertise
} // ble_adv