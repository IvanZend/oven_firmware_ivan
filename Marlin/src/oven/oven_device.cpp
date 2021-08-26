#include "oven_device.h"

MainDevice main_device;             // глобальный объект устройства

ProcessTimer::ProcessTimer(void)    // конструктор таймера
{
    timer_enabled = false;          // влаг включенного таймера
    minutes_low_digit = 0;          // младший разряд минут
    minutes_high_digit = 0;         // старший разряд минут
    hours_low_digit = 0;            // младший разряд часов
    hours_high_digit = 0;           // старший разряд часов
    seconds_counter = SEC_IN_MIN_COUNT;     // счётчик секунд в минуте
}

// запустить таймер
void ProcessTimer::start_process_timer(Side_of_screen screen_side)
{
    if ((minutes_low_digit != 0)||(minutes_high_digit != 0)||(hours_low_digit != 0)||(hours_high_digit != 0))   // если хотя бы один разряд не выставлен в ноль
    {
        switch (screen_side)        // выбираем сторону экрана (левый или правый таймер)
        {
            case LEFT_SIDE:
            {
                oven_display.lock_arrows(screen_side);      // блокируем стрелки таймера
                oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_stop_button, 0, 0); // переключаем кнопку вкл/выкл таймера
                timer_enabled = true;                       // выставляем флаг включенного таймера
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

// остановить таймер
void ProcessTimer::stop_process_timer(Side_of_screen screen_side)
{
    switch (screen_side)            // выбираем сторону экрана (левый или правый таймер)
    {
        case LEFT_SIDE:
        {
            timer_enabled = false;  // выставляем флаг выключенного таймера
            oven_display.widgets_vector[13].change_image_in_widget(img_time_colon_char, 0, 0);              // отрисовываем двоеточие таймера
            oven_display.widgets_vector[3].change_image_in_widget(img_heating_timer_start_button, 0, 0);    // переключаем кнопку вкл/выкл таймера
            oven_display.left_colon_displayed = true;       // выставляем флаг отображения двоеточия
            oven_display.unlock_arrows(screen_side);        // разблокируем стрелки таймера
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

// обработка события с таймера (вызывается раз в секунду)
void ProcessTimer::seconds_timer_handler(Side_of_screen screen_side)
{
    if (timer_enabled)          // если таймер включён
    {
        oven_display.blink_clock_colon(screen_side);    // мигаем двоеточием

        if (seconds_counter > 0)                        // если минута не истекла
        {
            seconds_counter--;                          // декрементируем счётчик секунд
        }
        else                                            // если минута истекла
        {
            seconds_counter = SEC_IN_MIN_COUNT;         // сбрасываем счётчик секунд (записываем количество секунд в минуте)

            if ((hours_low_digit == 0) && (hours_low_digit == 0) && (minutes_high_digit == 0) && (minutes_low_digit == 1))  // если истекла последняя минута
            {
                minutes_low_digit = 0;                  // обнуляем последнюю минуту
                switch (screen_side)                    // выбираем сторону экрана (левый или правый таймер)
                {
                    case LEFT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[12], oven_display.numbers_45_font_vector[minutes_low_digit]);  // отрисовываем ноль в разряде
                        break;
                    }
                    case RIGHT_SIDE:
                    {
                        oven_display.replace_time_figure(oven_display.widgets_vector[49], oven_display.numbers_45_font_vector[minutes_low_digit]);
                        break;
                    }
                }
            }

            if (minutes_low_digit > 0)                  // если младший разряд минут больше нуля
            {
                minutes_low_digit--;                    // вычетаем минуту
                switch (screen_side)
                {
                    case LEFT_SIDE:                     // выбираем сторону и отрисовываем цифру в разряде
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
            else if (minutes_high_digit > 0)            // иначе если старший разряд минут больше нуля (истекли более младшие разряды)
            {
                minutes_high_digit--;                   // вычитаем 10 минут
                minutes_low_digit = DECIMAL_MAX_NUMBER; // обнуляем счётчик десятков минут (задаём максимум)
                switch (screen_side)                    // выбираем сторону и отрисовываем младший и старший разряд минут
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
            else if (hours_low_digit > 0)               // иначе если младший разряд часов больше нуля (истекли более младшие разряды)
            {
                hours_low_digit--;                      // вычитаем час
                minutes_high_digit = SIX_DIGIT_MAX_NUMBER;      // обнуляем десятки минут
                minutes_low_digit = DECIMAL_MAX_NUMBER;         // обнуляем минуты
                switch (screen_side)
                {
                    case LEFT_SIDE:                     // выбираем сторону и отрисовываем разряды
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
            else if (hours_high_digit > 0)              // иначе если старший раряд часов большенуля (истекли более младшие разряды)
            {
                hours_high_digit--;                     // вычитаем 10 часов
                hours_low_digit = DECIMAL_MAX_NUMBER;   // обнуляем часы
                minutes_high_digit = SIX_DIGIT_MAX_NUMBER;  // обнуляем десятки минут
                minutes_low_digit = DECIMAL_MAX_NUMBER;     // обнуляем минуты
                switch (screen_side)                    // выбираем сторону и отрисовываем разряды
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
            else            // иначе если все разряды по нулям
            {
                switch (screen_side)                // выбираем сторону
                {
                case (LEFT_SIDE):
                {
                    main_device.stop_heating();     // останавливаем нагрев
                    break;
                }
                case (RIGHT_SIDE):
                {
                    main_device.stop_vacuum();      // останавливаем вакуум
                    break;
                }
                }

                stop_process_timer(screen_side);    // останавливаем таймер
            }
        }
    }
}

// конструктор главного устройства
MainDevice::MainDevice(void)
{
    heating_is_enabled = 0;         // флаг включённого нагрева
    preset_temperature = 0;         // заданная температура
    current_temperature = 0;        // текущая температура
    input_temperature = 0;          // вводимая температура

    vacuum_is_enabled = 0;          // флаг включённого вакуума
    preset_pressure = 0;            // заданное давление
    current_pressure = 0;           // текущее давление
    input_pressure = 0;             // вводимое давление
}

// обновито показания датчиков температуры и давления
void MainDevice::update_sensors_data(void)
{
    if (SHOW_ALL_SENSORS)       // если отображаем все сенсоры (для сервиса)
    {
        current_temperature = Temperature::OVEN_HEATER_600.celsius;         // считываем текущюу температуру нагревателя на 600 Вт
        oven_display.widgets_vector[70].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, oven_display.numbers_30_font_vector, current_temperature);    // отрисовываем

        current_temperature = Temperature::OVEN_HEATER_800.celsius;         // считываем текущую температуру нагревателя на 800 Вт
        oven_display.widgets_vector[71].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, oven_display.numbers_30_font_vector, current_temperature);    // отрисовываем
        
        current_pressure = Temperature::OVEN_VACUUM_PUMP.celsius;           // считываем текущее давление
        oven_display.widgets_vector[72].change_value_in_wgt(ALIGN_RIGHT, FONT_11_GAP_PIX, oven_display.numbers_11_font_vector, current_pressure);       // отрисовываем
        
        current_temperature = Temperature::OVEN_AIR_TEMPER_SENSOR.celsius;      // считываем текущую температуру воздуха
        oven_display.widgets_vector[35].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, oven_display.numbers_30_font_vector, current_temperature);    // отрисовываем
    }
    else
    {
        current_temperature = Temperature::OVEN_HEATER_800.celsius;      // считываем текущую температуру нагревателя на 800 Вт
        oven_display.widgets_vector[35].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, oven_display.numbers_30_font_vector, current_temperature);    // отрисовываем
    }


    if ((preset_temperature < FAN_OFF_TEMPERATURE)||(main_device.heating_is_enabled == false))   // если заданная температура ниже порога включения обдува, или нагрев не включён
    {
        // если температура нагревателей больше пороговой
        if ((Temperature::OVEN_HEATER_600.celsius > FAN_OFF_TEMPERATURE) || (Temperature::OVEN_HEATER_800.celsius > FAN_OFF_TEMPERATURE)) 
        {
            if (Temperature::OVEN_CONVECTION_FAN != 255)        // если вентилятор не включён
            {
                Temperature::OVEN_CONVECTION_FAN = 255;         // включаем
            }
        }
        else       // иначе если температура нагревателей ниже пороговой
        {
            if (Temperature::OVEN_CONVECTION_FAN != 0)          // если вентилятор включён
            {
                Temperature::OVEN_CONVECTION_FAN = 0;           // выключаем
            }
        }
    }

    if (main_device.heating_is_enabled)     // если нагрев включён
    {
        if (Temperature::OVEN_AIR_TEMPER_SENSOR.celsius > preset_temperature)       // если температура воздуха выше заданной
        {
            if ((preset_temperature + HEATING_MAINTAIN) > TEMPERATURE_CRITICAL)     // если заданная температура выше критической
            {
                // если требуемая температура нагревателей превышает критическую
                if ((Temperature::OVEN_HEATER_600.target != TEMPERATURE_CRITICAL) || (Temperature::OVEN_HEATER_800.target != TEMPERATURE_CRITICAL))
                {
                    Temperature::OVEN_HEATER_600.target = TEMPERATURE_CRITICAL;     // задаём критическую температуру (не выше)
                    Temperature::OVEN_HEATER_800.target = TEMPERATURE_CRITICAL;
                }
            }
            else    // иначе если температура нагревателя не выше критической
            {
                // если требуемая температура не равна заданной плюс доп. температура для удержания тепла
                if ((Temperature::OVEN_HEATER_600.target != preset_temperature + HEATING_MAINTAIN) || (Temperature::OVEN_HEATER_800.target != preset_temperature + HEATING_MAINTAIN))
                {
                    Temperature::OVEN_HEATER_600.target = preset_temperature + HEATING_MAINTAIN;        // требуем заданную температуру + дополнительная для удержания тепла
                    Temperature::OVEN_HEATER_800.target = preset_temperature + HEATING_MAINTAIN;
                }
            }
        }
    }
}

// запуск нагрева
void MainDevice::start_heating(void)
{
    main_device.heating_is_enabled = true;          // выставляем флаг нагрева
    oven_display.widgets_vector[33].change_image_in_widget(img_slide_button_on, 0, 0);              // переключаем кнопку вкл/выкл нагрев
    oven_display.widgets_vector[1].change_image_in_widget(img_thermometer_icon_enabled, 0, 0);      // переключаем зачёркнутую иконку термометра
    if ((preset_temperature + HEATING_INCREASE) > TEMPERATURE_CRITICAL)                             // если заданная температура плюс доп. для нагрева выше критической
    {
        Temperature::OVEN_HEATER_600.target = TEMPERATURE_CRITICAL;                                 // требуем критическую температуру
        Temperature::OVEN_HEATER_800.target = TEMPERATURE_CRITICAL;
    }
    else
    {
        Temperature::OVEN_HEATER_600.target = preset_temperature + HEATING_INCREASE;                // иначе требуем заданную температуру + доп. для нагрева
        Temperature::OVEN_HEATER_800.target = preset_temperature + HEATING_INCREASE;
    }
    Temperature::OVEN_CONVECTION_FAN = 255;                                                         // запускаем обдув
}

// остановка нагрева
void MainDevice::stop_heating(void)
{
    main_device.heating_is_enabled = false;         // убираем флаг нагрева
    oven_display.widgets_vector[33].change_image_in_widget(img_slide_button_off, 0, 0);             // переключаем кнопку вкл/выкл нагрев
    oven_display.widgets_vector[1].change_image_in_widget(img_thermometer_icon_crossed, 0, 0);      // переключаем зачеркнутую иконку термометра
    Temperature::OVEN_HEATER_600.target = NO_HEAT_DUMMY_VALUE;                                      // требуем минимальную температуру
    Temperature::OVEN_HEATER_800.target = NO_HEAT_DUMMY_VALUE;
}

// запуск вакуума
void MainDevice::start_vacuum(void)
{
    main_device.vacuum_is_enabled = true;           // выставляем флаг вакуума
    oven_display.widgets_vector[69].change_image_in_widget(img_slide_button_on, 0, 0);              // переключаем кнопку вкл/выкл вакуум
    oven_display.widgets_vector[37].change_image_in_widget(img_pressure_sensor_icon_enabled, 0, 0); // переключаем иконку зачеркнутого давления
    Temperature::OVEN_VACUUM_PUMP.target = VACUUM_DUMMY_VALUE;                                      // требуем давление-заглушку для ПИД-регулятора
}

// выключение вакума
void MainDevice::stop_vacuum(void)
{
    main_device.vacuum_is_enabled = false;          // убираем флаг вакуума
    oven_display.widgets_vector[69].change_image_in_widget(img_slide_button_off, 0, 0);             // переключаем кнопку вкл/выкл вакуум
    oven_display.widgets_vector[37].change_image_in_widget(img_pressure_sensor_icon_crossed, 0, 0); // переключаем иконку зачёркнутого давления
    Temperature::OVEN_VACUUM_PUMP.target = 0;                                                       // требуем минимальое давление
}

// инициализируем внешние устройства
void MainDevice::init_extern_devices(void)
{
    Temperature::OVEN_HEATER_600.target = NO_HEAT_DUMMY_VALUE;      // нагреватели на минимум
    Temperature::OVEN_HEATER_800.target = NO_HEAT_DUMMY_VALUE;
    Temperature::OVEN_VACUUM_PUMP.target = 0;                       // вакуум на минимум
    Temperature::OVEN_CONVECTION_FAN = 0;                           // обдув выключен
}