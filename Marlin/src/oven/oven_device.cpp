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

void ProcessTimer::start_process_timer(Side_of_screen screen_side)
{
    if ((minutes_low_digit != 0)||(minutes_high_digit != 0)||(hours_low_digit != 0)||(hours_high_digit != 0))
    {
        switch (screen_side)
        {
            case LEFT_SIDE:
            {
                oven_display.lock_arrows(screen_side);
                oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_stop_button, 0, 0);
                timer_enabled = true;
                break;
            }
            case RIGHT_SIDE:
            {
                oven_display.lock_arrows(screen_side);
                oven_display.widgets_vector[38].change_image_in_widget(img_heating_timer_stop_button, 0, 0);
                timer_enabled = true;
                break;
            }
        }
    }
}

void ProcessTimer::stop_process_timer(Side_of_screen screen_side)
{
    switch (screen_side)
    {
        case LEFT_SIDE:
        {
            timer_enabled = false;
            oven_display.widgets_vector[13].change_image_in_widget(img_time_colon_char, 0, 0);
            oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_start_button, 0, 0);
            oven_display.left_colon_displayed = true;
            oven_display.unlock_arrows(screen_side);
            break;
        }
        case RIGHT_SIDE:
        {
            timer_enabled = false;
            oven_display.widgets_vector[73].change_image_in_widget(img_time_colon_char, 0, 0);
            oven_display.widgets_vector[38].change_image_in_widget(img_heating_timer_start_button, 0, 0);
            oven_display.right_colon_displayed = true;
            oven_display.unlock_arrows(screen_side);
            break;
        }
    }
}

void ProcessTimer::seconds_timer_handler(Side_of_screen screen_side)
{
    if (timer_enabled)
    {
        oven_display.blink_clock_colon(screen_side);

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
                switch (screen_side)
                {
                    case LEFT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                    case RIGHT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[49], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                }
            }
            else if (minutes_high_digit > 0)
            {
                minutes_high_digit--;
                minutes_low_digit = DECIMAL_MAX_NUMBER;
                switch (screen_side)
                {
                    case LEFT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[11], oven_display.numbers_45_font_vector[minutes_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                    case RIGHT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[48], oven_display.numbers_45_font_vector[minutes_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[49], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                }
            }
            else if (hours_low_digit > 0)
            {
                hours_low_digit--;
                minutes_high_digit = SIX_DIGIT_MAX_NUMBER;
                minutes_low_digit = DECIMAL_MAX_NUMBER;
                switch (screen_side)
                {
                    case LEFT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[10], oven_display.numbers_45_font_vector[hours_low_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[11], oven_display.numbers_45_font_vector[minutes_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                    case RIGHT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[47], oven_display.numbers_45_font_vector[hours_low_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[48], oven_display.numbers_45_font_vector[minutes_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[49], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                }
            }
            else if (hours_high_digit > 0)
            {
                hours_high_digit--;
                hours_low_digit = DECIMAL_MAX_NUMBER;
                minutes_high_digit = SIX_DIGIT_MAX_NUMBER;
                minutes_low_digit = DECIMAL_MAX_NUMBER;
                switch (screen_side)
                {
                    case LEFT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[9], oven_display.numbers_45_font_vector[hours_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[10], oven_display.numbers_45_font_vector[hours_low_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[11], oven_display.numbers_45_font_vector[minutes_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                    case RIGHT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[46], oven_display.numbers_45_font_vector[hours_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[47], oven_display.numbers_45_font_vector[hours_low_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[48], oven_display.numbers_45_font_vector[minutes_high_digit]);
                        oven_display.replace_time_figure(oven_display.widgets_vector[49], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                }
            }
            else
            {
                stop_process_timer(screen_side);
            }
        }
    }
}

MainDevice::MainDevice(void)
{
    heating_is_enabled = 0;
    preset_temperature = 0;
    current_temperature = 0;
    input_temperature = 0;

    vacuum_is_enabled = 0;
    preset_pressure = 0;
    current_pressure = 0;
    input_pressure = 0;
}