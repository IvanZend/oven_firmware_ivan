#ifndef H_OVEN_DEVICE
#define H_OVEN_DEVICE

#include <cstdint>
#include "oven_display.h"

#define SECONDS_TIMER_MS      100

class ExternDevice
{
    public:
    bool device_enabled;
    ExternDevice();
};

class ProcessTimer
{
    public:
    bool timer_enabled;
    uint32_t minutes_low_digit;
    uint32_t minutes_high_digit;
    uint32_t hours_low_digit;
    uint32_t hours_high_digit;
    uint8_t seconds_counter;
    uint16_t miutes_counter;
    ProcessTimer();
    void start_process_timer(void);
    void stop_process_timer(void);
    void convert_digits_to_minutes(void);
    void seconds_timer_handler(void);
    void display_current_time(void);
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