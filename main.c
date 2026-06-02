#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

#define LED_PORT DT_NODELABEL(gpio1)
#define LED_PIN 3

static const struct device *gpio_dev = DEVICE_DT_GET(LED_PORT);

/* 임의로 만든 UUID */
#define BT_UUID_LED_SERVICE_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)

#define BT_UUID_LED_CHAR_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)

#define BT_UUID_LED_SERVICE \
    BT_UUID_DECLARE_128(BT_UUID_LED_SERVICE_VAL)

#define BT_UUID_LED_CHAR \
    BT_UUID_DECLARE_128(BT_UUID_LED_CHAR_VAL)

static ssize_t write_led(struct bt_conn *conn,
                         const struct bt_gatt_attr *attr,
                         const void *buf,
                         uint16_t len,
                         uint16_t offset,
                         uint8_t flags)
{
    char cmd[8];

    if (offset != 0) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }

    if (len >= sizeof(cmd)) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }

    memcpy(cmd, buf, len);
    cmd[len] = '\0';

    printk("Received: %s\n", cmd);

    if (strcmp(cmd, "ON") == 0) {
        gpio_pin_set(gpio_dev, LED_PIN, 1);
        printk("External LED ON\n");
    } else if (strcmp(cmd, "OFF") == 0) {
        gpio_pin_set(gpio_dev, LED_PIN, 0);
        printk("External LED OFF\n");
    } else {
        printk("Unknown command\n");
    }

    return len;
}

BT_GATT_SERVICE_DEFINE(led_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_LED_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_LED_CHAR,
                           BT_GATT_CHRC_WRITE,
                           BT_GATT_PERM_WRITE,
                           NULL,
                           write_led,
                           NULL),
);

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS,
                  BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA_BYTES(BT_DATA_UUID128_ALL,
                  BT_UUID_LED_SERVICE_VAL),
};

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE,
            CONFIG_BT_DEVICE_NAME,
            sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

int main(void)
{
    int err;

    printk("BLE LED Control Start\n");

    if (!device_is_ready(gpio_dev)) {
        printk("GPIO device not ready\n");
        return 0;
    }

    gpio_pin_configure(gpio_dev,
                       LED_PIN,
                       GPIO_OUTPUT_INACTIVE);

    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed: %d\n", err);
        return 0;
    }

    printk("Bluetooth initialized\n");

    err = bt_le_adv_start(
        BT_LE_ADV_PARAM(
            BT_LE_ADV_OPT_CONN,
            BT_GAP_ADV_FAST_INT_MIN_2,
            BT_GAP_ADV_FAST_INT_MAX_2,
            NULL
        ),
        ad,
        ARRAY_SIZE(ad),
        sd,
        ARRAY_SIZE(sd)
    );

    if (err) {
        printk("Advertising failed: %d\n", err);
        return 0;
    }

    printk("Advertising started\n");

    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
