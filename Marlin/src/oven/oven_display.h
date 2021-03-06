#ifndef H_OVEN_DISPLAY
#define H_OVEN_DISPLAY

#include <cstdint>
#include <string>
#include <vector>

#include "../../inc/MarlinConfigPre.h"
#include "../temperature.h"
//#include "../../Configuration.h"

using namespace std;

#define OFFSET_FROM_EDGE        30
#define SPACE_BETWEEN_LINES     40
#define RECTANGLE_OFFSET        4
#define FONT_SIZE               24
#define RECTANGLE_HEIGHT        FONT_SIZE + (RECTANGLE_OFFSET*2)
#define ROUND_CORNER            1
#define RECTANGLE_THICKNESS     2
#define RECTANGLE_FILL          1
#define RECTANGLE_PUNCTIR       0
#define DEFAULT_DISPLAY_MODE    STANDARD_USER_MODE
#define DISPLAY_WIDTH           800
#define DISPLAY_HEIGHT          600
#define RIGHT_COLUMN_OFFSET     391
#define TIME_FIGURE_Y_OFFSET    21
#define CALIBRATION_OFFSET_X    -10
#define CALIBRATION_OFFSET_Y    -8
#define SIX_DIGIT_MAX_NUMBER    5
#define DECIMAL_MAX_NUMBER      9
#define SEC_IN_MIN_COUNT        59
#define BOUNCE_MAX_SAMPLES      4
#define HOUR_CHAR_COORD_X       28
#define HOUR_CHAR_COORD_Y       72
#define TEMPER_DISPL_SIZE_X     95
#define TEMPER_DISPL_SIZE_Y     30
#define TEMPER_INPUT_SIZE_X     30
#define TEMPER_INPUT_SIZE_Y     11
#define FONT_30_GAP_PIX         4
#define FONT_11_GAP_PIX         3
#define IMG_BACKGR_IN_VECT      10
#define SHOW_ALL_SENSORS        0

enum Decr_Incr
{
    DECREMENT,
    INCREMENT
};

enum Img_vect_list
{
    CONSTANT_IMG,
    CHANGEABLE_IMG,
    BTN_PRESSED_IMG,
    BTN_RELEASED_IMG,
    BTN_BLOCKED_IMG
};

enum Buttons_list
{
    NO_BUTTON,
    HEATING_TIMER_START_STOP,
    LEFT_UP_ARROW_1,
    LEFT_UP_ARROW_2,
    LEFT_UP_ARROW_3,
    LEFT_UP_ARROW_4,
    LEFT_DOWN_ARROW_1,
    LEFT_DOWN_ARROW_2,
    LEFT_DOWN_ARROW_3,
    LEFT_DOWN_ARROW_4,
    LEFT_KEYBOARD_1,
    LEFT_KEYBOARD_2,
    LEFT_KEYBOARD_3,
    LEFT_KEYBOARD_4,
    LEFT_KEYBOARD_5,
    LEFT_KEYBOARD_6,
    LEFT_KEYBOARD_7,
    LEFT_KEYBOARD_8,
    LEFT_KEYBOARD_9,
    LEFT_KEYBOARD_0,
    LEFT_KEYBOARD_BACKSPACE,
    LEFT_KEYBOARD_RECET,
    LEFT_KEYBOARD_ENTER_TOP,
    LEFT_KEYBOARD_ENTER_BOTTOM,
    HEATING_ON_OFF,

    VACUUM_TIMER_START_STOP,
    RIGHT_UP_ARROW_1,
    RIGHT_UP_ARROW_2,
    RIGHT_UP_ARROW_3,
    RIGHT_UP_ARROW_4,
    RIGHT_DOWN_ARROW_1,
    RIGHT_DOWN_ARROW_2,
    RIGHT_DOWN_ARROW_3,
    RIGHT_DOWN_ARROW_4,
    RIGHT_KEYBOARD_1,
    RIGHT_KEYBOARD_2,
    RIGHT_KEYBOARD_3,
    RIGHT_KEYBOARD_4,
    RIGHT_KEYBOARD_5,
    RIGHT_KEYBOARD_6,
    RIGHT_KEYBOARD_7,
    RIGHT_KEYBOARD_8,
    RIGHT_KEYBOARD_9,
    RIGHT_KEYBOARD_0,
    RIGHT_KEYBOARD_BACKSPACE,
    RIGHT_KEYBOARD_RECET,
    RIGHT_KEYBOARD_ENTER_TOP,
    RIGHT_KEYBOARD_ENTER_BOTTOM,
    VACUUM_ON_OFF
};

enum Display_mode
{
    STANDARD_USER_MODE,
    SIMPLIFIED_USER_MODE,
    SERVICE_MODE
};

enum Alignment
{
    ALIGN_LEFT,
    ALIGN_RIGHT
};

enum Side_of_screen
{
    LEFT_SIDE,
    RIGHT_SIDE
};

typedef struct 
{
const uint8_t *data;
uint16_t width;
uint16_t height;
uint8_t dataSize;
} tImage;

typedef struct 
{
    uint16_t img_coord_x, img_coord_y;      // координаты изображения внутри виджета
    tImage image_struct;
} ImageObj;

/*
typedef struct 
{
    uint16_t coord_x, coord_y;      // координаты текста внутри виджета
    string text_string;
} TextObj;

typedef struct
{
    uint16_t coord_x, coord_y;
    uint16_t width, height;
    uint8_t corner_round;
    uint32_t main_color, background_color;
    uint8_t layer;
    uint16_t thickness;
    uint8_t fill;
    uint8_t punctir;
} RectangleObj;
*/
/*
 * Виджет - визуальный объект, выводимый на экран. Может быть или не быть кнопкой.
 * Может содержать изображения. 
*/

// подключаем файлы bmp - изображений
#include "images/rocket.h"
#include "images/backgr_tile.h"
#include "images/rect_down_left_round.h"
#include "images/rect_down_right_round.h"
#include "images/rect_horiz_line.h"
#include "images/rect_vertic_line.h"
#include "images/rect_internal_round.h"
#include "images/rect_top_right_round.h"
#include "images/rect_top_left_corner.h"
#include "images/img_atm_string.h"
#include "images/img_heating_string.h"
#include "images/img_heating_timer_start_button.h"
#include "images/img_heating_timer_stop_button.h"
#include "images/img_pressure_sensor_icon_crossed.h"
#include "images/img_pressure_sensor_icon_enabled.h"
#include "images/img_slide_button_off.h"
#include "images/img_slide_button_on.h"
#include "images/img_temperature_display.h"
#include "images/img_pressure_display.h"
#include "images/img_temperature_entering_background.h"
#include "images/img_pressure_entering_background.h"
#include "images/img_thermometer_icon_crossed.h"
#include "images/img_thermometer_icon_enabled.h"
#include "images/img_time_colon_char.h"
#include "images/img_time_colon_backgr.h"
#include "images/img_time_down_arrow_pressed.h"
#include "images/img_time_down_arrow_released.h"
#include "images/img_time_down_arrow_blocked.h"
#include "images/img_time_figure_rect.h"
#include "images/img_time_hour_char.h"
#include "images/img_time_minute_char.h"
#include "images/img_time_up_arrow_pressed.h"
#include "images/img_time_up_arrow_released.h"
#include "images/img_time_up_arrow_blocked.h"
#include "images/img_vacuum_pump_string_1.h"
#include "images/img_vacuum_pump_string_2.h"
#include "images/img_keyboard_0_prsd.h"
#include "images/img_keyboard_0_rlsd.h"
#include "images/img_keyboard_1_prsd.h"
#include "images/img_keyboard_1_rlsd.h"
#include "images/img_keyboard_2_prsd.h"
#include "images/img_keyboard_2_rlsd.h"
#include "images/img_keyboard_3_prsd.h"
#include "images/img_keyboard_3_rlsd.h"
#include "images/img_keyboard_4_prsd.h"
#include "images/img_keyboard_4_rlsd.h"
#include "images/img_keyboard_5_prsd.h"
#include "images/img_keyboard_5_rlsd.h"
#include "images/img_keyboard_6_prsd.h"
#include "images/img_keyboard_6_rlsd.h"
#include "images/img_keyboard_7_prsd.h"
#include "images/img_keyboard_7_rlsd.h"
#include "images/img_keyboard_8_prsd.h"
#include "images/img_keyboard_8_rlsd.h"
#include "images/img_keyboard_9_prsd.h"
#include "images/img_keyboard_9_rlsd.h"
#include "images/img_keyboard_backspace_prsd.h"
#include "images/img_keyboard_backspace_rlsd.h"
#include "images/img_keyboard_cancel_prsd.h"
#include "images/img_keyboard_cancel_rlsd.h"
#include "images/img_keyboard_enter_bottom_prsd.h"
#include "images/img_keyboard_enter_bottom_rlsd.h"
#include "images/img_keyboard_enter_top_prsd.h"
#include "images/img_keyboard_enter_top_rlsd.h"
#include "images/numbers_45_0.h"
#include "images/numbers_45_1.h"
#include "images/numbers_45_2.h"
#include "images/numbers_45_3.h"
#include "images/numbers_45_4.h"
#include "images/numbers_45_5.h"
#include "images/numbers_45_6.h"
#include "images/numbers_45_7.h"
#include "images/numbers_45_8.h"
#include "images/numbers_45_9.h"
#include "images/numbers_45_background.h"
#include "images/numbers_30_0.h"
#include "images/numbers_30_1.h"
#include "images/numbers_30_2.h"
#include "images/numbers_30_3.h"
#include "images/numbers_30_4.h"
#include "images/numbers_30_5.h"
#include "images/numbers_30_6.h"
#include "images/numbers_30_7.h"
#include "images/numbers_30_8.h"
#include "images/numbers_30_9.h"
#include "images/numbers_30_background.h"
#include "images/numbers_11_0.h"
#include "images/numbers_11_1.h"
#include "images/numbers_11_2.h"
#include "images/numbers_11_3.h"
#include "images/numbers_11_4.h"
#include "images/numbers_11_5.h"
#include "images/numbers_11_6.h"
#include "images/numbers_11_7.h"
#include "images/numbers_11_8.h"
#include "images/numbers_11_9.h"
#include "images/numbers_11_background.h"

// класс виджета
class Widget
{
    public:
    Buttons_list button_name;       // имя кнопки виджета (может быть NO_BUTTON)
    uint16_t wgt_coord_x, wgt_coord_y, wgt_width, wgt_height;   // координаты и габариты виджета
    vector<ImageObj> constant_images;       // постоянные изображения
    vector<ImageObj> changeable_images;     // изменяемые изображения
    vector<ImageObj> pressed_btn_images;    // изображения при нажатой кнопке
    vector<ImageObj> released_btn_images;   // изображения при отпущенной кнопке
    vector<ImageObj> blocked_btn_images;    // изображения при заблокированной кнопке
    bool button_is_pressed;                 // флаг нажатия кнопки
    bool btn_locked;                        // флаг блокировки кнопки

    Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y);  // конструктор без габаритов
    Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y, uint16_t wgt_wdth, uint16_t wgt_hght);    // конструктор с габаритами
    void add_img_to_wgt(Img_vect_list vect_to_add_type, tImage add_image_generated, uint16_t add_img_coord_x, uint16_t add_img_coord_y);    // добавляем изображение в виджет
    void tile_area(tImage image_to_tile, uint16_t area_width, uint16_t area_hight); // замостить область повторяющейся картинкой
    void set_rectangle(void);                       // отрисовываем фоновые рамки
    void draw_img_vector(vector<ImageObj> img_vector_to_draw, uint16_t parent_wgt_coord_x, uint16_t parent_wgt_coord_y);    // отрисовываем вектор изображений
    void change_image_in_widget(tImage image_to_output, uint16_t img_out_coord_x, uint16_t img_out_coord_y);            // изменить изображение изменить изображение в виджете
    uint16_t img_center_x(tImage img_to_center);                                // получить координату для размещения изображения в центре виджета
    void lock_button(void);                 // заблокировать кнопку
    void unlock_button(void);               // разблокировать кнопку
    uint16_t string_align_right_x(vector<tImage>& img_nmbrs_vect, uint8_t fnt_space, vector<uint8_t>& value_to_align);  // выровнять строку по правой границе виджета
    void convert_value_to_int_arr(vector<uint8_t>& value_int_vect,  uint32_t value_to_conv);            // конвертировать числовое значение в массив цифр
    void change_value_in_wgt(Alignment numbers_align, uint8_t font_space, vector<tImage>& img_font, uint32_t value_to_displ);   // изменить числовое значение в виджете
    void temper_input_add_number(uint8_t num_to_enter, uint16_t &variable_to_change);       // добавить разряд к вводимому значению
    void temper_input_backspace(uint16_t &variable_to_change);          // удалить младший разряд из вводимого значения
    void temper_input_reset(uint16_t &variable_to_change);              // сбросить вводимое значение
    void temper_input_enter(uint16_t changed_variable, uint16_t &variable_to_write, uint16_t wgt_to_output_numbr);  // ввод из редактируемого значения в заданное значение
};

// класс дисплея (единственный)
class OvenDisplay 
{
    public:
    OvenDisplay(void);                  // конструктор
    Display_mode display_mode;          // режим дисплея (пользовательский стандартный, пользовательский упрощённый, сервисный)
    uint16_t display_width, display_height;     // габариты дисплея в пикселях
    bool draw_all_completed;            // Флаг, что весь дисплей был отрисован. Переключаем при запуске и при смене режима (DEFAULT_DISPLAY_MODE)
    bool left_colon_displayed;          // флаг отображения двоеточия в левом таймере
    bool right_colon_displayed;         // флаг обображения двоеточия в правом таймере
    Buttons_list previous_button;       // буфер предыдущей нажатой кнопки
    vector<Widget> widgets_vector;      // вектор виджетов
    vector<tImage> numbers_45_font_vector;  // вектор изображений шрифта 45 пикселей
    vector<tImage> numbers_30_font_vector;  // вектор изображений шрифта 30 пикселей
    vector<tImage> numbers_11_font_vector;  // вектор изображений шрифта 11 пикселей
    uint32_t bounce_sample_counter;     // счётчик дребезга
    Buttons_list bounce_btn_buff;       // буфер кнопки, для которой считаем дребезг

    void test_draw(void);                           // тестовая картинка, выводим изображение ракеты
    void init_widgets(void);                        // задаём содержимое виджетов
    void draw_all_widgets(void);                    // отрисовываем весь дисплей
    void init_widgets_size(void);                   // если размер виджета не был задан вручную, он автоматически равен размеру первой картинки - кнопки
    void handle_button_press(Buttons_list pressed_button);                  // обрабатываем нажатие кнопки
    void enter_related_event_left(void);            // если нажали на одну половину enter, имитируем нажатие на вторую
    void enter_related_event_right(void);           // то же для правой клавитуры
    uint16_t identify_pressed_btn(uint16_t pressing_coord_x, uint16_t pressing_coord_y);    // определяем, в какой кнопке лежат координаты нажатия
    void change_time_figure(Decr_Incr chng_type, Widget& figure_widget, uint32_t* digit, vector<tImage>& nmbrs_img_vect, uint8_t digit_max_value);  // изменить цифру в таймере
    void replace_time_figure(Widget& figure_widget, tImage& nmbr_img);   // отрисовать введённую цифру в таймере
    void init_fonts_img_vect(void);     // иницииализировать вектора изображений шрифтов
    void init_displayed_values(void);   // инициализировать отображаемые числовые значения
    void blink_clock_colon(Side_of_screen screen_side);     // вкл / выкл двоеточие в таймере
    void lock_arrows(Side_of_screen screen_side);           // заблокировать кнопки стрелок в таймере
    void unlock_arrows(Side_of_screen screen_side);         // разблокировать кнопки стрелок в таймере
    bool bounce_filter_passed(Buttons_list pressed_button); // проверка прохождения фильтра дребезга

};

extern OvenDisplay oven_display;       // глобальный объект дисплея

#endif