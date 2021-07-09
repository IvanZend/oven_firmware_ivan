#include "oven_device.h"

ExternDevice::ExternDevice(void)
{
    device_enabled = 0;
}

ProcessTimer::ProcessTimer(void)
{
    timer_enabled = false;
    minutes_low_digit = 0;
    minutes_high_digit = 0;
    hours_low_digit = 0;
    hours_high_digit = 0;
    seconds_counter = 0;
    miutes_counter = 0;
}

void ProcessTimer::start_process_timer(void)
{
    convert_digits_to_minutes();
    if (miutes_counter != 0)
    {
    oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_stop_button, 0, 0);
    timer_enabled = true;
    }
}

void ProcessTimer::stop_process_timer(void)
{
    timer_enabled = false;
    oven_display.widgets_vector[13].change_image_in_widget(img_time_colon_char, 0, 0);
    oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_start_button, 0, 0);
    oven_display.colon_displayed = true;
}

void ProcessTimer::convert_digits_to_minutes(void)
{
    miutes_counter += minutes_low_digit;
    miutes_counter += minutes_high_digit*10;
    miutes_counter += hours_low_digit*60;
    miutes_counter += hours_high_digit*600;
}

void ProcessTimer::seconds_timer_handler(void)
{
    if (timer_enabled)
    {
        oven_display.blink_clock_colon();

        if (seconds_counter > 0)
        {
            seconds_counter--;
        }
        else
        {
            seconds_counter = 60;
            if (miutes_counter > 0)
            {
                miutes_counter--;
            }
            else
            {
                stop_process_timer();
            }
            display_current_time();
        }

    }
}

void ProcessTimer::display_current_time(void)
{
    uint32_t current_minutes = miutes_counter % 60;
    uint32_t current_hours = (miutes_counter - current_minutes)/60;
    minutes_low_digit = current_minutes % 10;
    minutes_high_digit = (current_minutes - minutes_low_digit)/10;
    hours_low_digit = current_hours % 10;
    hours_high_digit = (current_hours - hours_low_digit)/10;

    oven_display.replace_time_figure(oven_display.widgets_vector[9], oven_display.numbers_45_font_vector[hours_high_digit]);
    oven_display.replace_time_figure(oven_display.widgets_vector[10], oven_display.numbers_45_font_vector[hours_low_digit]);
    oven_display.replace_time_figure(oven_display.widgets_vector[11], oven_display.numbers_45_font_vector[minutes_high_digit]);
    oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
}

MainDevice::MainDevice(void)
{
    heating_timer_started = 0;
    heating_is_enabled = 0;
    preset_temperature = 0;
    current_temperature = 0;
}