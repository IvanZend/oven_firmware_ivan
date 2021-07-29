#ifndef H_OVEN_DEVICE
#define H_OVEN_DEVICE

#include <cstdint>
#include "oven_display.h"
#include "../gcode/gcode.h"
#include "../gcode/parser.h"

#define TOUCH_BOUNCE_TIMER_MS   1
#define SECONDS_TIMER_MS        1000
#define FAN_OFF_TEMPERATURE     40
#define VACUUM_DUMMY_VALUE      100
#define NO_HEAT_DUMMY_VALUE     15
#define HEATING_INCREASE        20
#define HEATING_MAINTAIN        20
#define TEMPERATURE_CRITICAL    150

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
    uint16_t seconds_counter;
    uint16_t miutes_counter;
    ProcessTimer();
    void start_process_timer(Side_of_screen screen_side);
    void stop_process_timer(Side_of_screen screen_side);
    void seconds_timer_handler(Side_of_screen screen_side);
};

class MainDevice
{
    public:

    bool heating_is_enabled;
    uint16_t preset_temperature;
    uint16_t current_temperature;
    uint16_t input_temperature;

    bool vacuum_is_enabled;
    uint16_t preset_pressure;
    uint16_t current_pressure;
    uint16_t input_pressure;

    ProcessTimer process_timer_left;
    ProcessTimer process_timer_right;

    MainDevice();
    void update_sensors_data(void);
    void start_heating(void);
    void stop_heating(void);
    void start_vacuum(void);
    void stop_vacuum(void);
    void init_extern_devices(void);
};


#endif