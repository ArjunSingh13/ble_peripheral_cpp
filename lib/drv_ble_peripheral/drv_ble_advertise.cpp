#include "drv_ble_advertise.hpp"

#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>

LOG_MODULE_REGISTER(ble_advertise, LOG_LEVEL_INF);

namespace ble_adv 
{
	/** this contains advertising parameter */
	static const struct bt_le_adv_param *adv_param =
		BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONNECTABLE|BT_LE_ADV_OPT_USE_IDENTITY),
		CONFIG_MIN_ADVERTISE_INTERVAL, 
		CONFIG_MAX_ADVERTISE_INTERVAL,
		nullptr);

	/* since static member is shared between all objects. its not defined in objects.
	this is why we need to define it explicitly in .cpp file */
	struct bt_data ble_advertise::sd[1] = {}; //defined in constructor
	struct bt_data ble_advertise::ad[2] = {
		/* Set the advertising flags */
		BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
		/* Set the advertising packet data  */
		BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME)-1),
	};

	ble_advertise::ble_advertise(connCallback onConnectFn, connCallback onDisconnectFn)
	: _onConnect{onConnectFn}, _onDisconnect{onDisconnectFn}
	{
		int err;
		bt_addr_le_t addr;

		static struct bt_conn_cb connection_callbacks = {
			.connected              = _onConnect,
			.disconnected           = _onDisconnect,
		};
		bt_conn_cb_register(&connection_callbacks);
		
		err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
		if (err) {
			printk("Invalid BT address (err %d)\n", err);
		}

		err = bt_id_create(&addr, NULL);
		if (err < 0) {
			printk("Creating new ID failed (err %d)\n", err);
		}

		err = bt_enable(NULL);
		if (err) {
			LOG_ERR("Bluetooth init failed (err %d)\n", err);
		}
#if CONFIG_CONNECTABLE_PERIPHERAL
		// Create a UUID128 array (manually encoded)
		static const uint8_t uuid_128[] = {
			BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
		};

		// Correctly set the second scan response data entry (UUID)
		sd[0].type = BT_DATA_UUID128_ALL;
		sd[0].data_len = sizeof(uuid_128);
		sd[0].data = uuid_128;  // Assign pointer to encoded UUID data
#else
		// TODO update this later
#endif
		LOG_INF("Bluetooth initialized with URL %s\n", _url.c_str());

		/* Start advertising */
		err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
		if (err) {
			LOG_ERR("Advertising failed to start (err %d)\n", err);
		}
		LOG_INF("Advertising successfully started\n");
	}

	ble_advertise::~ble_advertise(void){
		LOG_INF("ble advertise destructor called!!!");
	}

	void ble_advertise::ble_advertise_data(uint32_t data) noexcept{
#if !CONFIG_CONNECTABLE_PERIPHERAL
	  // TODO update this later
#endif
	}
}