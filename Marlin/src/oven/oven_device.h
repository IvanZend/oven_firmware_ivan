#ifndef H_OVEN_DEVICE
#define H_OVEN_DEVICE

#include <cstdint>

class OvenExternDevice
{
    public:
    uint8_t output_pin;
    bool device_enabled;
};

class OvenSensor
{
    public:
    uint8_t input_pin;
};

extern OvenExternDevice heater_600;

#endif
