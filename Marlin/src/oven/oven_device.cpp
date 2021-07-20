#include "oven_device.h"

MainDevice main_device;

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
    if ((minutes_low_digit != 0)||(minutes_high_digit != 0)||(hours_low_digit != 0)||(hours_high_digit != 0))
    {
        oven_display.lock_arrows_left();
        main_device.heating_timer_started = true;
        oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_stop_button, 0, 0);
        timer_enabled = true;
    }

}

void ProcessTimer::stop_process_timer(void)
{
    main_device.heating_timer_started = false;
    timer_enabled = false;
    oven_display.widgets_vector[13].change_image_in_widget(img_time_colon_char, 0, 0);
    oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_start_button, 0, 0);
    oven_display.unlock_arrows_left();
    oven_display.colon_displayed = true;
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
            seconds_counter = SEC_IN_MIN_COUNT;

            if (minutes_low_digit > 0)
            {
                minutes_low_digit--;
                oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
            }
            else if (minutes_high_digit > 0)
            {
                minutes_high_digit--;
                minutes_low_digit = DECIMAL_MAX_NUMBER;
                oven_display.replace_time_figure(oven_display.widgets_vector[11], oven_display.numbers_45_font_vector[minutes_high_digit]);
                oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
            }
            else if (hours_low_digit > 0)
            {
                hours_low_digit--;
                minutes_high_digit = SIX_DIGIT_MAX_NUMBER;
                minutes_low_digit = DECIMAL_MAX_NUMBER;
                oven_display.replace_time_figure(oven_display.widgets_vector[10], oven_display.numbers_45_font_vector[hours_low_digit]);
                oven_display.replace_time_figure(oven_display.widgets_vector[11], oven_display.numbers_45_font_vector[minutes_high_digit]);
                oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
            }
            else if (hours_high_digit > 0)
            {
                hours_high_digit--;
                hours_low_digit = DECIMAL_MAX_NUMBER;
                minutes_high_digit = SIX_DIGIT_MAX_NUMBER;
                minutes_low_digit = DECIMAL_MAX_NUMBER;
                oven_display.replace_time_figure(oven_display.widgets_vector[9], oven_display.numbers_45_font_vector[hours_high_digit]);
                oven_display.replace_time_figure(oven_display.widgets_vector[10], oven_display.numbers_45_font_vector[hours_low_digit]);
                oven_display.replace_time_figure(oven_display.widgets_vector[11], oven_display.numbers_45_font_vector[minutes_high_digit]);
                oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
            }
            else
            {
                stop_process_timer();
            }
        }
    }
}

MainDevice::MainDevice(void)
{
    heating_timer_started = 0;
    heating_is_enabled = 0;
    preset_temperature = 0;
    current_temperature = 0;
    input_temperature = 0;
}