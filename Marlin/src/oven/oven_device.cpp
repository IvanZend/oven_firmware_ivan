#include "oven_device.h"

ExternDevice::ExternDevice(void)
{
    device_enabled = 0;
}

ProcessTimer::ProcessTimer(void)
{
    timer_miutes = 0;
    minutes_low_digit = 0;
    minutes_high_digit = 0;
    hours_low_digit = 0;
    hours_high_digit = 0;
}

MainDevice::MainDevice(void)
{
    heating_timer_started = 0;
    heating_is_enabled = 0;
    preset_temperature = 0;
    current_temperature = 0;
}