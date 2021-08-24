#ifndef H_OVEN_DEVICE
#define H_OVEN_DEVICE

#include <cstdint>
#include "oven_display.h"
#include "../gcode/gcode.h"
#include "../gcode/parser.h"

#define TOUCH_BOUNCE_TIMER_MS   1           // время одного семпла для фильтра дребезга
#define SECONDS_TIMER_MS        1000        // количество миллисекунд в секунде
#define FAN_OFF_TEMPERATURE     40          // пороговая температура выключения обдува
#define VACUUM_DUMMY_VALUE      10000       // значение-заглушка для вакуума (ПИД-регулятор всегда в максимуме)
#define NO_HEAT_DUMMY_VALUE     15          // значение температуры без нагрева
#define HEATING_INCREASE        0           // дополнительная температура нагревателей при нагреве
#define HEATING_MAINTAIN        0           // дополнительная температура нагревателей при удержании тепла
#define TEMPERATURE_CRITICAL    150         // критическая температура, которую нельзя превышать        
#define PERCENT_TO_VACUUM       30          // процент времени нагрева, после которого включаем вакуум

// класс таймера
class ProcessTimer
{
    public:
    bool timer_enabled;                 // флаг включённого таймера
    uint32_t minutes_low_digit;         // младший разряд минут
    uint32_t minutes_high_digit;        // старший разряд минут
    uint32_t hours_low_digit;           // младший разряд часов
    uint32_t hours_high_digit;          // старший разряд часов
    uint16_t seconds_counter;           // счётчик секунд в минуте
    uint32_t total_sec_counter;         // общее количество секунд (измеряется при запуске таймера)
    uint32_t remain_sec_counter;        // количество секунд до конца работы таймера
    ProcessTimer();                     // конструктор
    void start_process_timer(Side_of_screen screen_side);       // запустить таймер
    void stop_process_timer(Side_of_screen screen_side);        // остановить таймер
    void seconds_timer_handler(Side_of_screen screen_side);     // обработчик события с внешнего таймера
    uint32_t return_remain_sec(uint32_t min_low_dgt, uint32_t min_high_dgt, uint32_t hour_low_dgt, uint32_t hour_high_dgt, uint32_t sec_count);   // секунды до конца таймера
    bool time_not_zero(void);           // если хотя бы один разряд не равен нулю
};

// класс основного устройства
class MainDevice
{
    public:

    bool heating_is_enabled;            // флаг включённого нагрева
    uint16_t preset_temperature;        // заданная температура
    uint16_t current_temperature;       // текущая температура
    uint16_t input_temperature;         // вводимая температура

    bool vacuum_is_enabled;             // флаг включённого вакуума
    uint16_t preset_pressure;           // заданное давление
    uint16_t current_pressure;          // текущее давление
    uint16_t input_pressure;            // вводимое давление

    ProcessTimer process_timer_left;    // экземпляр таймера слева (температура)
    ProcessTimer process_timer_right;   // экземпляр таймера справа (вакуум)

    MainDevice();                       // конструктор
    void update_sensors_data(void);     // обновить отображаемые значения датчиков
    void start_heating(void);           // запуск нагрева
    void stop_heating(void);            // остановка нагрева
    void start_vacuum(void);            // запуск вакуума
    void stop_vacuum(void);             // остановка вакуума
    void init_extern_devices(void);     // инициализировать внешние устройства
};

#endif