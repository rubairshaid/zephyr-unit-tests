#include <zephyr/ztest.h>
#include "../../src/eeprom.h"

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/ztest.h>

#define EEPROM_I2C_ADDR DT_REG_ADDR(DT_NODELABEL(eeprom))
static const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

static void suite_setup(void)
{
    k_sleep(K_SECONDS(1));
    printk("Starting Dolos\n");
}

ZTEST_SUITE(eeprom_suite, NULL, suite_setup, NULL, NULL, NULL);

ZTEST(eeprom_suite, test_writing_and_reading_eeprom)
{
    uint16_t address = 0x00;
    uint8_t data = 0xaa;
    int write_data = eeprom_write(address, data);

    zassert_equal(write_data, 0, "error while writing on eeprom");

    k_sleep(K_MSEC(500));

    uint8_t eeprom_data;
    int read_data = eeprom_read(address, &eeprom_data);

    zassert_equal(read_data, 0, "error while reading eeprom");

    zassert_equal(eeprom_data, data, "the data is not correct");
}

ZTEST(eeprom_suite, test_reading_n_bytes)
{
    uint16_t address = 0x00;
    uint8_t data;
    size_t len = 4;

    int read_n_byte_data = eeprom_read_n(address, &data, len);

    zassert_equal(read_n_byte_data, 0, "error while reading from eeprom");
}

ZTEST(eeprom_suite, test_writing_and_reading_n_bytes)
{
    uint16_t address = 0x00;
    uint8_t data[16] = {0xaa, 0xbb, 0xcc};
    size_t len = 3;

    int write_n_byte_data = eeprom_write_n(address, data, len);

    zassert_equal(write_n_byte_data, 0, "error while reading from eeprom");

    k_sleep(K_MSEC(1000));
    
    uint8_t eeprom_data;
    
    for(int i = 0 ; i < 3 ; i++, address++)
    {
        int read_data = eeprom_read(address, &eeprom_data);
        zassert_equal(read_data, 0, "error while reading eeprom");
        zassert_equal(data[i], eeprom_data, "the data is not correct");
    }   
}