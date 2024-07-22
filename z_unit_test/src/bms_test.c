#include <zephyr/ztest.h>
#include "../../src/bms.h"

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/smf.h>

#define LED_PWR DT_NODE_CHILD_IDX(DT_NODELABEL(ledpwr))

static const struct gpio_dt_spec out_en_spec =
	GPIO_DT_SPEC_GET(DT_NODELABEL(output_enable), gpios);
static const struct gpio_dt_spec bq_chrg_ok =
	GPIO_DT_SPEC_GET(DT_NODELABEL(bq_charge_ok), gpios);

static void suite_setup(void)
{
    k_sleep(K_SECONDS(1));
    printk("Starting Dolos\n");
}


ZTEST_SUITE(bms_tests, NULL, suite_setup, NULL, NULL, NULL);

ZTEST(bms_tests, check_output_enabled)
{
	if (device_is_ready(out_en_spec.port)) {
		int output_enabled_state = gpio_pin_get_dt(&out_en_spec);
		zassert_equal(output_enabled_state, 1, "output not enabled");
    }
}

ZTEST(bms_tests, check_charging_ok)
{
	if (device_is_ready(bq_chrg_ok.port)) {
		int charge_state = gpio_pin_get_dt(&bq_chrg_ok);
		zassert_equal(charge_state, 1, "No charging");
    }
}

ZTEST(bms_tests, check_system_present_polarity)
{
	bool system_pres_pol = bms_get_sys_pres_pol();
	zassert_equal(system_pres_pol, true, "system present polarity is low");
}


ZTEST(bms_tests, check_system_present)
{
	bool system_pres = bms_get_sys_pres();
	zassert_equal(system_pres, true, "system is not present");
}


ZTEST(bms_tests, check_bms_state)
{
	enum bms_state bms_state = bms_get_curr_state();
	zassert_equal(bms_state , BMS_STATE_POWER_OUTPUT_ON, "POWER OUTPUT IS NOT ON");
}
