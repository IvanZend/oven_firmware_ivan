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
                switch (screen_side)
                {
                case (LEFT_SIDE):
                {
                    main_device.stop_heating();
                    break;
                }
                case (RIGHT_SIDE):
                {
                    main_device.stop_vacuum();
                    break;
                }
                }

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

void MainDevice::update_sensors_data(void)
{
    current_temperature = Temperature::OVEN_HEATER_600.celsius; //OVEN_AIR_TEMPER_SENSOR.celsius; // T600 TRM.K2
    oven_display.widgets_vector[35].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, oven_display.numbers_30_font_vector, current_temperature);
    
    current_pressure = Temperature::OVEN_HEATER_800.celsius;
    oven_display.widgets_vector[71].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, oven_display.numbers_30_font_vector, current_pressure);
    
    current_pressure = Temperature::OVEN_VACUUM_PUMP.celsius;//OVEN_VACUUM_PUMP.celsius; // T600 TRM.K2
    oven_display.widgets_vector[72].change_value_in_wgt(ALIGN_RIGHT, FONT_11_GAP_PIX, oven_display.numbers_11_font_vector, current_pressure);
    
}

void MainDevice::start_heating(void)
{
    main_device.heating_is_enabled = true;
    oven_display.widgets_vector[33].change_image_in_widget(img_slide_button_on, 0, 0);
    oven_display.widgets_vector[1].change_image_in_widget(img_thermometer_icon_enabled, 0, 0);
    Temperature::OVEN_HEATER_600.target = preset_temperature;
    Temperature::OVEN_HEATER_800.target = preset_temperature;
    Temperature::OVEN_CONVECTION_FAN = 255;
}

void MainDevice::stop_heating(void)
{
    main_device.heating_is_enabled = false;
    oven_display.widgets_vector[33].change_image_in_widget(img_slide_button_off, 0, 0);
    oven_display.widgets_vector[1].change_image_in_widget(img_thermometer_icon_crossed, 0, 0);
    Temperature::OVEN_HEATER_600.target = 0;
    Temperature::OVEN_HEATER_800.target = 0;
    Temperature::OVEN_CONVECTION_FAN = 0;
}


void MainDevice::start_vacuum(void)
{
    main_device.vacuum_is_enabled = true;
    oven_display.widgets_vector[69].change_image_in_widget(img_slide_button_on, 0, 0);
    oven_display.widgets_vector[37].change_image_in_widget(img_pressure_sensor_icon_enabled, 0, 0);

}

void MainDevice::stop_vacuum(void)
{
    main_device.vacuum_is_enabled = false;
    oven_display.widgets_vector[69].change_image_in_widget(img_slide_button_off, 0, 0);
    oven_display.widgets_vector[37].change_image_in_widget(img_pressure_sensor_icon_crossed, 0, 0);

}

void MainDevice::init_extern_devices(void)
{
    Temperature::OVEN_HEATER_600.target = 0;
    Temperature::OVEN_HEATER_800.target = 0;
    Temperature::OVEN_VACUUM_PUMP.target = 0;
    Temperature::OVEN_CONVECTION_FAN = 0;
}