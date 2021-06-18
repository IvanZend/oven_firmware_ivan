#ifndef H_OVEN_DEVICE
#define H_OVEN_DEVICE

class ExternDevice
{
    public:
    bool device_enabled;
    ExternDevice();
};

extern ExternDevice heater_600;

#endif