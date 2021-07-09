#ifndef H_OVEN_DEVICE
#define H_OVEN_DEVICE

#include <cstdint>

class ExternDevice
{
    public:
    bool device_enabled;
    ExternDevice();
};

class ProcessTimer
{
    public:
    uint16_t timer_miutes;
    uint8_t minutes_low_digit;
    uint8_t minutes_high_digit;
    uint8_t hours_low_digit;
    uint8_t hours_high_digit;
    ProcessTimer();
    void start_process_timer(void);
    void stop_process_timer(void);
};

class MainDevice
{
    public:
    bool heating_timer_started;
    bool heating_is_enabled;
    uint16_t preset_temperature;
    uint16_t current_temperature;
    ProcessTimer process_timer_left;
    ProcessTimer process_timer_right;
    MainDevice();
};


#endif