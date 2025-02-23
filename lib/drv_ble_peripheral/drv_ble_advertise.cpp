#include "drv_ble_advertise.hpp"

#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/gatt.h>

LOG_MODULE_REGISTER(ble_advertise, LOG_LEVEL_INF);

namespace ble_adv 
{
struct bt_conn* ble_advertise::my_conn = nullptr;

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

struct bt_gatt_exchange_params exchange_params;

/** 
 * PRIVATE FUNCTIONS 
 * 
 * 
 * */
void ble_advertise::on_connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("Connection error %d", err);
		return;
	}
	LOG_INF("Connected");
    my_conn = bt_conn_ref(conn);

	/* Declare a structure to store the connection parameters */
	struct bt_conn_info info;
	err = bt_conn_get_info(conn, &info);
	if (err) {
		LOG_ERR("bt_conn_get_info() returned %d", err);
		return;
	}
	/* STEP 1.2 - Add the connection parameters to your log */
	double connection_interval = info.le.interval*1.25; // in ms
	uint16_t supervision_timeout = info.le.timeout*10; // in ms
	LOG_INF("Connection parameters: interval %.2f ms, latency %d intervals, timeout %d ms", connection_interval, info.le.latency, supervision_timeout);
}

void ble_advertise::on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected. Reason %d", reason);
	//dk_set_led(CONNECTION_STATUS_LED, 0);
	bt_conn_unref(my_conn);
}

/* Add the callback for connection parameter updates */
void ble_advertise::on_le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout)
{
	double connection_interval = interval*1.25;         // in ms
	uint16_t supervision_timeout = timeout*10;          // in ms
	LOG_INF("Connection parameters updated: interval %.2f ms, latency %d intervals, timeout %d ms", connection_interval, latency, supervision_timeout);
}

/* Write a callback function to inform about updates in the PHY */
void ble_advertise::on_le_phy_updated(struct bt_conn *conn, struct bt_conn_le_phy_info *param)
{
	// PHY Updated
	if (param->tx_phy == BT_CONN_LE_TX_POWER_PHY_1M) {
		LOG_INF("PHY updated. New PHY: 1M");
	}
	else if (param->tx_phy == BT_CONN_LE_TX_POWER_PHY_2M) {
		LOG_INF("PHY updated. New PHY: 2M");
	}
	else if (param->tx_phy == BT_CONN_LE_TX_POWER_PHY_CODED_S8) {
		LOG_INF("PHY updated. New PHY: Long Range");
	}
}

/* Write a callback function to inform about updates in data length */
void ble_advertise::on_le_data_len_updated(struct bt_conn *conn, struct bt_conn_le_data_len_info *info)
{
	uint16_t tx_len     = info->tx_max_len; 
	uint16_t tx_time    = info->tx_max_time;
	uint16_t rx_len     = info->rx_max_len;
	uint16_t rx_time    = info->rx_max_time;
	LOG_INF("Data length updated. Length %d/%d bytes, time %d/%d us", tx_len, rx_len, tx_time, rx_time);
}

void ble_advertise::exchange_func(struct bt_conn *conn, uint8_t att_err,
			  struct bt_gatt_exchange_params *params)
{
	LOG_INF("MTU exchange %s", att_err == 0 ? "successful" : "failed");
	if (!att_err) {
		uint16_t payload_mtu = bt_gatt_get_mtu(conn) - 3;   // 3 bytes used for Attribute headers.
		LOG_INF("New MTU: %d bytes", payload_mtu);
	}
}

/**
 * PUBLIC FUNCTIONS
 */
ble_advertise::ble_advertise(etl::string<18>&& randomStaticAddress)
: _randomStaticAddress{randomStaticAddress}
{
	int err;
	bt_addr_le_t addr;

	static struct bt_conn_cb connection_callbacks = {
		.connected              = on_connected,
		.disconnected           = on_disconnected,
		.le_param_updated       = on_le_param_updated,  	// Add the callback for connection parameter updates 
		.le_phy_updated         = on_le_phy_updated, 		// Add the callback for PHY mode updates
		.le_data_len_updated    = on_le_data_len_updated, 	// Add the callback for data length updates
	};
	bt_conn_cb_register(&connection_callbacks);
	
	err = bt_addr_le_from_str(_randomStaticAddress.c_str(), "random", &addr);
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
	static const uint8_t uuid_128[] = { // LBS service UUID
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


int ble_advertise::ble_update_phy(Phy_level newRxLevel, Phy_level newTxLevel)
{
	int rc;
	const struct bt_conn_le_phy_param preferred_phy = {
		.options	 = BT_CONN_LE_PHY_OPT_NONE, // TODO: Need to check benefit of other values, should we take this value as argument too?
		.pref_tx_phy = static_cast<uint8_t>(newTxLevel),
		.pref_rx_phy = static_cast<uint8_t>(newRxLevel),
	};

	rc = bt_conn_le_phy_update(my_conn, &preferred_phy);
	if (rc) {
		LOG_ERR("bt_conn_le_phy_update() returned %d", rc);
		return rc;
	}
	return rc;
}

int ble_advertise::ble_update_data_length(uint16_t maxPayloadSize, uint16_t maxPayloadTime)
{
	int rc;
	struct bt_conn_le_data_len_param my_data_len = {
		.tx_max_len = maxPayloadSize, //BT_GAP_DATA_LEN_MAX,
		.tx_max_time = maxPayloadTime, //BT_GAP_DATA_TIME_MAX,
	};

	rc = bt_conn_le_data_len_update(my_conn, &my_data_len);
	if (rc) {
		LOG_ERR("data_len_update failed (err %d)", rc);
	}

	return rc;
}

int ble_advertise::ble_update_mtu(uint16_t& maxSize)
{
	int rc;
	exchange_params.func = exchange_func;

	rc = bt_gatt_exchange_mtu(my_conn, &exchange_params);
	if (rc) {
		LOG_ERR("bt_gatt_exchange_mtu failed (err %d)", rc);
	}

	return rc;
}

}