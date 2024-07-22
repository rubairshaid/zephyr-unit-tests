#include <zephyr/ztest.h>
#include "../../src/temperature.h"
#include "../../src/pac.h"
#include "../../src/buck_boost.h"
#include "../../src/eeprom.h"
#include "../../src/smart_battery.h"
#include "../../src/smbus_target.h"

#include <zephyr/device.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/ztest.h>


static void suite_setup(void)
{
    k_sleep(K_SECONDS(1));
    printk("Starting Dolos\n");
}

static bool predicate_fun(void)
{
    return true;
}

ZTEST_SUITE(temperature_pac_suite, predicate_fun, suite_setup, NULL, NULL, NULL);



ZTEST(temperature_pac_suite, test_temperature_value)
{    
    double temp_c = temperature_get_c();
    double min_temp = 0; 
    double max_temp = 37;
    
    zassert_between_inclusive(temp_c, min_temp, max_temp, "Temperature is out of range");

    printk("Temperature in Celsius: %f\n", temp_c);

    double actual_temp_k = temperature_get_k();
    double expected_temp_k = temp_c + 273.15;
    
    zassert_equal(expected_temp_k, actual_temp_k, "Temperature value in K unit is not as expected" );

    double actual_temp_f = temperature_get_f();
    double expected_temp_f = temp_c * (9.0 / 5.0) + 32;

    zassert_equal(expected_temp_f, actual_temp_f, "Temperature value in F unit is not as expected" );

}

/*
    In this test case we make sure that when the dolos is 
    not connected to charger the value and current ranger should be around 0.
*/
ZTEST(temperature_pac_suite, test_voltage_and_current_values_without_charger)
{
    uint32_t actual_voltage = pac1954_get_voltage_mv();

    uint32_t min_voltage = 0 ; 
    uint32_t max_voltage = 100;
    printk("Voltage %u ", actual_voltage);
    zassert_between_inclusive(actual_voltage, min_voltage, max_voltage, "Voltage value is out of range");

    uint32_t actual_current = pac1954_get_current_ma();
    uint32_t min_current = 0 ; 
    uint32_t max_current = 100;
    printk("Current %u" , actual_current);
    zassert_between_inclusive(actual_current, min_current, max_current, "Current value is out of range");    
}

/*
    In this test case we make sure that when the dolos is connected to 
    the charger the voltage and current value must be in specific range.
*/
ZTEST(temperature_pac_suite, test_voltage_and_current_values_with_charger)
{
    uint32_t actual_voltage = pac1954_get_voltage_mv();

    uint32_t min_voltage = 8000 ; 
    uint32_t max_voltage = 20000;

    printk("Voltage %u ", actual_voltage);
    
    zassert_between_inclusive(actual_voltage, min_voltage, max_voltage, "Voltage value is out of range");

    uint32_t actual_current = pac1954_get_current_ma();
    uint32_t min_current = 0 ; 
    uint32_t max_current = 3000;

    printk("Current %u ", actual_current);
    
    zassert_between_inclusive(actual_current, min_current, max_current, "Current value is out of range");    
}

ZTEST_EXPECT_FAIL(temperature_pac_suite, test_expect_fail);
ZTEST_EXPECT_SKIP(temperature_pac_suite, test_expect_skip);

ZTEST(temperature_pac_suite, test_expect_fail)
{
    zassert_true(false, NULL);
}

ZTEST(temperature_pac_suite, test_expect_skip)
{
    ztest_test_skip();
}