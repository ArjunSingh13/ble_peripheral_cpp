#pragma once

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

	/** manufacturing data to advertise */
	typedef struct adv_mfg_data {
		uint16_t company_code;	    /* Company Identifier Code. */
		uint16_t number_press;      /* Number of times Button 1 is pressed*/
	} adv_mfg_data_type;

	/* since static member is shared between all objects. its not defined in objects.
	this is why we need to define it explicitly in .cpp file */
	struct bt_data ble_advertise::sd[1] = {}; 
	
	static adv_mfg_data_type adv_mfg_data = {CONFIG_COMPANY_ID_CODE, 0x00};
	
	struct bt_data ble_advertise::ad[2] = {
		/* Set the advertising flags */
		BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
		/* Set the advertising packet data  */
		BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME)-1),
		/* Advertise manufacturing data */
		//BT_DATA(BT_DATA_MANUFACTURER_DATA, (unsigned char *)&adv_mfg_data, sizeof(adv_mfg_data)),
	};

	ble_advertise::ble_advertise()
	{
		
		int err;
		bt_addr_le_t addr;
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
		// get the URL from Kconfig.
		_url.push_back(CONFIG_ADVERTISE_PREFIX);
		_url += CONFIG_ADVERTISE_URL;
		sd[0].type = BT_DATA_URI;
		sd[0].data = reinterpret_cast<const uint8_t*>(_url.c_str());
		sd[0].data_len = static_cast<uint8_t>(_url.size());
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
		adv_mfg_data.number_press = data;
		/* Send an updated value over BLE */
		bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
#endif
	}
}