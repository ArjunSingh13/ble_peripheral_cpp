#include "drv_ble_advertise.hpp"

LOG_MODULE_REGISTER(ble_advertise, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME // prj.conf
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

struct bt_data ble_advertise::ad[2] = {
	/* Set the advertising flags */
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	/* Set the advertising packet data  */
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

ble_advertise::ble_advertise()
{
	int err;
	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
	}

	// get the URL from Kconfig.
	_url.push_back(CONFIG_ADVERTISE_PREFIX);
	_url += CONFIG_ADVERTISE_URL;
	struct bt_data sd[1] = {
		/* Include the URL data in the scan response packet */
		BT_DATA(BT_DATA_URI, _url.c_str(), static_cast<uint8_t>(_url.size())),
	};

	LOG_INF("Bluetooth initialized with URL %s\n", _url.c_str());

	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
	}
	LOG_INF("Advertising successfully started\n");
}

ble_advertise::~ble_advertise(void){

	LOG_INF("ble advertise destructor called!!!");
}

void ble_advertise::ble_advertise_start(void) const{

}