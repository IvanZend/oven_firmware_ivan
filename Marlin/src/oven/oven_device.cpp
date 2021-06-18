#include "oven_device.h"

ExternDevice heater_600;
ExternDevice heater_800;

ExternDevice::ExternDevice(void)
{
    device_enabled = 0;
}