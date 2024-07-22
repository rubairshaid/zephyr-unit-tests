#include <zephyr/ztest.h>
#include "../../src/buck_boost.h"

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/ztest.h>

#define DT_DRV_COMPAT ti_bq25731
static const struct i2c_dt_spec i2c_spec = I2C_DT_SPEC_INST_GET(0);


static void suite_setup(void)
{
    k_sleep(K_SECONDS(1));
    printk("Starting Dolos\n");

	bq25731_init();
}

ZTEST_SUITE(buck_boost_suite, NULL, suite_setup, NULL, NULL, NULL);

ZTEST(buck_boost_suite, test_registers_initial_values)
{    
    uint8_t register_addresses[] = {0x00, 0x30, 0x32};
	uint16_t expected_registers_values[] = {0x010a, 0x3700, 0x0037};

	uint16_t reg_data;
	int ret;
    k_sleep(K_SECONDS(1));
	for(size_t i = 0; i < ARRAY_SIZE(register_addresses); i++) {
        ret = i2c_write_read_dt(&i2c_spec, &register_addresses[i], sizeof(register_addresses[i]), &reg_data, sizeof(reg_data));
        if(ret != 0) {
            printk("Failed to read register at address=0x%02x, err=%d", register_addresses[i], ret);
        }
        zassert_equal(expected_registers_values[i],reg_data , "The initial value of register @0x%02x not as expected", register_addresses[i]);
	}
}


