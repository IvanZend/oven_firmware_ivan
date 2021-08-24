#include "ltdc.h"
#include "oven_display.h"
#include "../core/macros.h"
#include "oven_core.h"

OvenDisplay oven_display;             // глобальный объект дисплея

// конструктор виджета (без габаритов)
Widget::Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y)
{
    button_name = btn_name;
    wgt_coord_x = wgt_x;
    wgt_coord_y = wgt_y;
    wgt_width = 0;
    wgt_height = 0;
    button_is_pressed = false;
    btn_locked = false;
}

// конструктор виджета (с габаритами)
Widget::Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y, uint16_t wgt_wdth, uint16_t wgt_hght)
{
    button_name = btn_name;
    wgt_coord_x = wgt_x;
    wgt_coord_y = wgt_y;
    wgt_width = wgt_wdth;
    wgt_height = wgt_hght;
    button_is_pressed = false;
    btn_locked = false;
}

// добавить изображение в вектор внутри виджета
void Widget::add_img_to_wgt(Img_vect_list vect_to_add_type, tImage add_image_generated, uint16_t add_img_coord_x, uint16_t add_img_coord_y)
{
    ImageObj tmp_img_obj_1;
    tmp_img_obj_1.img_coord_x = add_img_coord_x;
    tmp_img_obj_1.img_coord_y = add_img_coord_y;
    tmp_img_obj_1.image_struct = add_image_generated;
    
    switch (vect_to_add_type)
    {
    case CONSTANT_IMG:                                      // изображение не меняется на протяжении всей программы
    {
        constant_images.push_back(tmp_img_obj_1);
        break;
    }
    case CHANGEABLE_IMG:                                    // изображение меняется при вызове специальной функции
    {
        changeable_images.push_back(tmp_img_obj_1);
        break;
    }
    case BTN_PRESSED_IMG:                                   // изображение при нажатии кнопки
    {
        pressed_btn_images.push_back(tmp_img_obj_1);
        break;
    }
    case BTN_RELEASED_IMG:                                  // изображение при отпущенной кнопке
    {
        released_btn_images.push_back(tmp_img_obj_1);
        break;
    }
    case BTN_BLOCKED_IMG:                                   // изображение при заблокированной кнопке
        blocked_btn_images.push_back(tmp_img_obj_1);
        break;
    }
}

OvenDisplay::OvenDisplay(void)          // конструктор экземпляря дисплея
{
    display_mode = DEFAULT_DISPLAY_MODE;    // режим дисплея (пока есть только один)
    display_width = DISPLAY_WIDTH;
    display_height = DISPLAY_HEIGHT;
    draw_all_completed = false;             // флаг начальной отрисовки всего экрана
    left_colon_displayed = true;            // флаг отображения двоеточия в таймере (левом и правом)
    right_colon_displayed = true;
    previous_button = NO_BUTTON;            // предыдущая нажатая кнопка
    bounce_sample_counter = 0;              // счётчик касаний для фильтра дребезга тачскрина
    bounce_btn_buff = NO_BUTTON;            // кнопка, для которой работает фильтр дребезга
    init_fonts_img_vect();                  // инициализируем шрифты (цифры)
    init_widgets();                         // создаём все виджеты и кладём в общий вектор
}

// первое тестовое изображение. сейчас ненужно
void OvenDisplay::test_draw(void)
{
    TFT_DrawBitmap_d(100, 100, rocket.data, rocket.width, rocket.height, MAIN_LAYER);
}

// создаём виджеты и складываем в общий вектор
void OvenDisplay::init_widgets(void)
{
    // создаём все виджеты
    Widget background_frame = {NO_BUTTON, 0, 0};
    Widget thermometer_icon = {NO_BUTTON, 29, 32};
    Widget heating_string = {NO_BUTTON, 120, 42};
    Widget heating_timer_start_stop_btn = {NO_BUTTON, 88, 127, 0, 0};
    Widget temperature_display = {NO_BUTTON, 200, 50};
    Widget left_time_up_arrow_1 = {LEFT_UP_ARROW_1, 49, 200, 0, 0};
    Widget left_time_up_arrow_2 = {LEFT_UP_ARROW_2, 124, 200, 0, 0};
    Widget left_time_up_arrow_3 = {LEFT_UP_ARROW_3, 218, 200, 0, 0};
    Widget left_time_up_arrow_4 = {LEFT_UP_ARROW_4, 292, 200, 0, 0};
    Widget left_time_figure_1 = {NO_BUTTON, 49, 230};
    Widget left_time_figure_2 = {NO_BUTTON, 124, 230};
    Widget left_time_figure_3 = {NO_BUTTON, 218, 230};
    Widget left_time_figure_4 = {NO_BUTTON, 292, 230};
    Widget left_time_colon_char = {NO_BUTTON, 201, 269};
    Widget left_time_down_arrow_1 = {LEFT_DOWN_ARROW_1, 49, 320, 0, 0};
    Widget left_time_down_arrow_2 = {LEFT_DOWN_ARROW_2, 124, 320, 0, 0};
    Widget left_time_down_arrow_3 = {LEFT_DOWN_ARROW_3, 218, 320, 0, 0};
    Widget left_time_down_arrow_4 = {LEFT_DOWN_ARROW_4, 292, 320, 0, 0};
    Widget left_temperature_entering_background = {NO_BUTTON, 50, 357};
    Widget temperature_left_keyboard_1 = {LEFT_KEYBOARD_1, 54, 382, 0, 0};
    Widget temperature_left_keyboard_2 = {LEFT_KEYBOARD_2, 105, 382, 0, 0};
    Widget temperature_left_keyboard_3 = {LEFT_KEYBOARD_3, 155, 382, 0, 0};
    Widget temperature_left_keyboard_4 = {LEFT_KEYBOARD_4, 205, 382, 0, 0};
    Widget temperature_left_keyboard_5 = {LEFT_KEYBOARD_5, 255, 382, 0, 0};
    Widget temperature_left_keyboard_6 = {LEFT_KEYBOARD_6, 54, 416, 0, 0};
    Widget temperature_left_keyboard_7 = {LEFT_KEYBOARD_7, 105, 416, 0, 0};
    Widget temperature_left_keyboard_8 = {LEFT_KEYBOARD_8, 155, 416, 0, 0};
    Widget temperature_left_keyboard_9 = {LEFT_KEYBOARD_9, 205, 416, 0, 0};
    Widget temperature_left_keyboard_0 = {LEFT_KEYBOARD_0, 255, 416, 0, 0};
    Widget temperature_left_keyboard_backspace = {LEFT_KEYBOARD_BACKSPACE, 54, 450, 0, 0};
    Widget temperature_left_keyboard_cancel = {LEFT_KEYBOARD_RECET, 155, 450, 0, 0};
    Widget temperature_left_keyboard_enter_top = {LEFT_KEYBOARD_ENTER_TOP, 306, 382, 0, 0};
    Widget temperature_left_keyboard_enter_bottom = {LEFT_KEYBOARD_ENTER_BOTTOM, 256, 450, 0, 0};
    Widget heating_on_off_button = {HEATING_ON_OFF, 132, 495, 0, 0};
    Widget temperature_displ_preset = {NO_BUTTON, 238, 81, TEMPER_DISPL_SIZE_X, TEMPER_DISPL_SIZE_Y};
    Widget temperature_displ_current = {NO_BUTTON, 238, 129, TEMPER_DISPL_SIZE_X, TEMPER_DISPL_SIZE_Y};
    Widget temperature_displ_input = {NO_BUTTON, 320, 362, TEMPER_INPUT_SIZE_X, TEMPER_INPUT_SIZE_Y};

    Widget pressure_sensor_icon = {NO_BUTTON, 422, 32};
    Widget vacuum_pump_string_1 = {NO_BUTTON, 512, 39};
    Widget vacuum_pump_string_2 = {NO_BUTTON, 512, 65};
    Widget atm_string = {NO_BUTTON, 415, 100};
    Widget vacuum_timer_start_stop_btn = {NO_BUTTON, 481, 127, 0, 0};
    Widget pressure_display = {NO_BUTTON, 592, 50};
    Widget right_time_up_arrow_1 = {NO_BUTTON, 442, 200, 0, 0};
    Widget right_time_up_arrow_2 = {NO_BUTTON, 517, 200, 0, 0};
    Widget right_time_up_arrow_3 = {NO_BUTTON, 611, 200, 0, 0};
    Widget right_time_up_arrow_4 = {NO_BUTTON, 686, 200, 0, 0};
    Widget right_time_figure_1 = {NO_BUTTON, 442, 230, 0, 0};
    Widget right_time_figure_2 = {NO_BUTTON, 517, 230, 0, 0};
    Widget right_time_figure_3 = {NO_BUTTON, 611, 230, 0, 0};
    Widget right_time_figure_4 = {NO_BUTTON, 686, 230, 0, 0};
    Widget right_time_down_arrow_1 = {NO_BUTTON, 442, 320, 0, 0};
    Widget right_time_down_arrow_2 = {NO_BUTTON, 517, 320, 0, 0};
    Widget right_time_down_arrow_3 = {NO_BUTTON, 611, 320, 0, 0};
    Widget right_time_down_arrow_4 = {NO_BUTTON, 686, 320, 0, 0};
    Widget temperature_right_entering_background = {NO_BUTTON, 443, 357};
    Widget temperature_right_keyboard_1 = {NO_BUTTON, 448, 382, 0, 0};
    Widget temperature_right_keyboard_2 = {NO_BUTTON, 498, 382, 0, 0};
    Widget temperature_right_keyboard_3 = {NO_BUTTON, 549, 382, 0, 0};
    Widget temperature_right_keyboard_4 = {NO_BUTTON, 599, 382, 0, 0};
    Widget temperature_right_keyboard_5 = {NO_BUTTON, 650, 382, 0, 0};
    Widget temperature_right_keyboard_6 = {NO_BUTTON, 448, 416, 0, 0};
    Widget temperature_right_keyboard_7 = {NO_BUTTON, 498, 416, 0, 0};
    Widget temperature_right_keyboard_8 = {NO_BUTTON, 549, 416, 0, 0};
    Widget temperature_right_keyboard_9 = {NO_BUTTON, 599, 416, 0, 0};
    Widget temperature_right_keyboard_0 = {NO_BUTTON, 650, 416, 0, 0};
    Widget temperature_right_keyboard_backspace = {NO_BUTTON, 448, 450, 0, 0};
    Widget temperature_right_keyboard_recet = {NO_BUTTON, 549, 450, 0, 0};
    Widget temperature_right_keyboard_enter_top = {NO_BUTTON, 700, 382, 0, 0};
    Widget temperature_right_keyboard_enter_bottom = {NO_BUTTON, 650, 450, 0, 0};
    Widget vacuum_on_off_button = {NO_BUTTON, 527, 495, 0, 0};
    Widget pressure_displ_preset = {NO_BUTTON, 630, 81, TEMPER_DISPL_SIZE_X, TEMPER_DISPL_SIZE_Y};
    Widget pressure_displ_current = {NO_BUTTON, 630, 129, TEMPER_DISPL_SIZE_X, TEMPER_DISPL_SIZE_Y};
    Widget pressure_displ_input = {NO_BUTTON, 714, 362, TEMPER_INPUT_SIZE_X, TEMPER_INPUT_SIZE_Y};
    Widget right_time_colon_char = {NO_BUTTON, 594, 269};
    
    // добавляем начальные изображения в виджеты
    background_frame.tile_area(backgr_tile, display_width, display_height);             // заливаем фон одним цветом (повторяющаяся картинка)
    background_frame.set_rectangle();                                                   // отрисовываем рамки (повторяющиеся элементы)
    thermometer_icon.add_img_to_wgt(CONSTANT_IMG, img_thermometer_icon_crossed, 0, 0);  // добавляем изображение во внутренний вектор виджета
    heating_string.add_img_to_wgt(CONSTANT_IMG, img_heating_string, 0, 0);
    heating_timer_start_stop_btn.add_img_to_wgt(BTN_RELEASED_IMG, img_heating_timer_start_button, 0, 0);
    temperature_display.add_img_to_wgt(CONSTANT_IMG, img_temperature_display, 0, 0);
    left_time_up_arrow_1.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_1.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_up_arrow_1.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    left_time_up_arrow_2.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_2.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_up_arrow_2.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    left_time_up_arrow_3.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_3.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_up_arrow_3.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    left_time_up_arrow_4.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_4.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_up_arrow_4.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    left_time_figure_1.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_1.add_img_to_wgt(CONSTANT_IMG, img_time_hour_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    left_time_figure_2.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_2.add_img_to_wgt(CONSTANT_IMG, img_time_hour_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    left_time_figure_3.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_3.add_img_to_wgt(CONSTANT_IMG, img_time_minute_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    left_time_figure_4.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_4.add_img_to_wgt(CONSTANT_IMG, img_time_minute_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    left_time_colon_char.add_img_to_wgt(CONSTANT_IMG, img_time_colon_char, 0, 0);
    left_time_down_arrow_1.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_1.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    left_time_down_arrow_1.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    left_time_down_arrow_2.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_2.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    left_time_down_arrow_2.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    left_time_down_arrow_3.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_3.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    left_time_down_arrow_3.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    left_time_down_arrow_4.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    left_time_down_arrow_4.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_4.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    left_temperature_entering_background.add_img_to_wgt(CONSTANT_IMG, img_temperature_entering_background, 0, 0);
    temperature_left_keyboard_0.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_0_prsd, 0, 0);
    temperature_left_keyboard_0.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_0_rlsd, 0, 0);
    temperature_left_keyboard_1.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_1_prsd, 0, 0);
    temperature_left_keyboard_1.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_1_rlsd, 0, 0);
    temperature_left_keyboard_2.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_2_prsd, 0, 0);
    temperature_left_keyboard_2.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_2_rlsd, 0, 0);
    temperature_left_keyboard_3.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_3_prsd, 0, 0);
    temperature_left_keyboard_3.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_3_rlsd, 0, 0);
    temperature_left_keyboard_4.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_4_prsd, 0, 0);
    temperature_left_keyboard_4.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_4_rlsd, 0, 0);
    temperature_left_keyboard_5.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_5_prsd, 0, 0);
    temperature_left_keyboard_5.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_5_rlsd, 0, 0);
    temperature_left_keyboard_6.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_6_prsd, 0, 0);
    temperature_left_keyboard_6.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_6_rlsd, 0, 0);
    temperature_left_keyboard_7.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_7_prsd, 0, 0);
    temperature_left_keyboard_7.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_7_rlsd, 0, 0);
    temperature_left_keyboard_8.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_8_prsd, 0, 0);
    temperature_left_keyboard_8.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_8_rlsd, 0, 0);
    temperature_left_keyboard_9.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_9_prsd, 0, 0);
    temperature_left_keyboard_9.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_9_rlsd, 0, 0);
    temperature_left_keyboard_backspace.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_backspace_prsd, 0, 0);
    temperature_left_keyboard_backspace.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_backspace_rlsd, 0, 0);
    temperature_left_keyboard_cancel.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_cancel_prsd, 0, 0);
    temperature_left_keyboard_cancel.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_cancel_rlsd, 0, 0);
    temperature_left_keyboard_enter_top.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_enter_top_prsd, 0, 0);
    temperature_left_keyboard_enter_top.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_enter_top_rlsd, 0, 0);
    temperature_left_keyboard_enter_bottom.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_enter_bottom_prsd, 0, 0);
    temperature_left_keyboard_enter_bottom.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_enter_bottom_rlsd, 0, 0);
    heating_on_off_button.add_img_to_wgt(BTN_RELEASED_IMG, img_slide_button_off, 0, 0);
    heating_on_off_button.add_img_to_wgt(BTN_BLOCKED_IMG, img_slide_button_blocked, 0, 0);
    
    
    pressure_sensor_icon.add_img_to_wgt(CONSTANT_IMG, img_pressure_sensor_icon_crossed, 0, 0);
    /*
    vacuum_pump_string_1.add_img_to_wgt(CONSTANT_IMG, img_vacuum_pump_string_1, 0, 0);
    vacuum_pump_string_2.add_img_to_wgt(CONSTANT_IMG, img_vacuum_pump_string_2, 0, 0);
    atm_string.add_img_to_wgt(CONSTANT_IMG, img_atm_string, 0, 0);
    vacuum_timer_start_stop_btn.add_img_to_wgt(BTN_RELEASED_IMG, img_heating_timer_start_button, 0, 0);
    //pressure_display.add_img_to_wgt(CONSTANT_IMG, img_pressure_display, 0, 0);
    right_time_up_arrow_1.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    right_time_up_arrow_1.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    right_time_up_arrow_1.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    right_time_up_arrow_2.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    right_time_up_arrow_2.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    right_time_up_arrow_2.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    right_time_up_arrow_3.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    right_time_up_arrow_3.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    right_time_up_arrow_3.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    right_time_up_arrow_4.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    right_time_up_arrow_4.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    right_time_up_arrow_4.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_up_arrow_blocked, 0, 0);
    right_time_figure_1.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    right_time_figure_1.add_img_to_wgt(CONSTANT_IMG, img_time_hour_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    right_time_figure_2.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    right_time_figure_2.add_img_to_wgt(CONSTANT_IMG, img_time_hour_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    right_time_figure_3.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    right_time_figure_3.add_img_to_wgt(CONSTANT_IMG, img_time_minute_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    right_time_figure_4.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    right_time_figure_4.add_img_to_wgt(CONSTANT_IMG, img_time_minute_char, HOUR_CHAR_COORD_X, HOUR_CHAR_COORD_Y);
    right_time_down_arrow_1.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    right_time_down_arrow_1.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    right_time_down_arrow_1.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    right_time_down_arrow_2.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    right_time_down_arrow_2.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    right_time_down_arrow_2.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    right_time_down_arrow_3.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    right_time_down_arrow_3.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    right_time_down_arrow_3.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    right_time_down_arrow_4.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    right_time_down_arrow_4.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    right_time_down_arrow_4.add_img_to_wgt(BTN_BLOCKED_IMG, img_time_down_arrow_blocked, 0, 0);
    //temperature_right_entering_background.add_img_to_wgt(CONSTANT_IMG, img_pressure_entering_background, 0, 0);
    //temperature_right_keyboard_0.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_0_prsd, 0, 0);
    //temperature_right_keyboard_0.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_0_rlsd, 0, 0);
    //temperature_right_keyboard_1.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_1_prsd, 0, 0);
    //temperature_right_keyboard_1.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_1_rlsd, 0, 0);
    //temperature_right_keyboard_2.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_2_prsd, 0, 0);
    //temperature_right_keyboard_2.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_2_rlsd, 0, 0);
    //temperature_right_keyboard_3.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_3_prsd, 0, 0);
    //temperature_right_keyboard_3.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_3_rlsd, 0, 0);
    //temperature_right_keyboard_4.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_4_prsd, 0, 0);
    //temperature_right_keyboard_4.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_4_rlsd, 0, 0);
    //temperature_right_keyboard_5.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_5_prsd, 0, 0);
    //temperature_right_keyboard_5.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_5_rlsd, 0, 0);
    //temperature_right_keyboard_6.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_6_prsd, 0, 0);
    //temperature_right_keyboard_6.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_6_rlsd, 0, 0);
    //temperature_right_keyboard_7.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_7_prsd, 0, 0);
    //temperature_right_keyboard_7.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_7_rlsd, 0, 0);
    //temperature_right_keyboard_8.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_8_prsd, 0, 0);
    //temperature_right_keyboard_8.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_8_rlsd, 0, 0);
    //temperature_right_keyboard_9.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_9_prsd, 0, 0);
    //temperature_right_keyboard_9.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_9_rlsd, 0, 0);
    //temperature_right_keyboard_backspace.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_backspace_prsd, 0, 0);
    //temperature_right_keyboard_backspace.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_backspace_rlsd, 0, 0);
    //temperature_right_keyboard_recet.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_cancel_prsd, 0, 0);
    //temperature_right_keyboard_recet.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_cancel_rlsd, 0, 0);
    //temperature_right_keyboard_enter_top.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_enter_top_prsd, 0, 0);
    //temperature_right_keyboard_enter_top.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_enter_top_rlsd, 0, 0);
    //temperature_right_keyboard_enter_bottom.add_img_to_wgt(BTN_PRESSED_IMG, img_keyboard_enter_bottom_prsd, 0, 0);
    //temperature_right_keyboard_enter_bottom.add_img_to_wgt(BTN_RELEASED_IMG, img_keyboard_enter_bottom_rlsd, 0, 0);
    vacuum_on_off_button.add_img_to_wgt(BTN_RELEASED_IMG, img_slide_button_off, 0, 0);
    right_time_colon_char.add_img_to_wgt(CONSTANT_IMG, img_time_colon_char, 0, 0);
    */

    widgets_vector.clear();     // инициализруем нулём вектор виджетов
    switch (display_mode)       // тут будет переключение между сервисным и пользовательским режимом (сейчас неактуально)
    {
    case STANDARD_USER_MODE:
    {
        widgets_vector.push_back(background_frame);                             // 0    номер виджета в общем векторе
        widgets_vector.push_back(thermometer_icon);                             // 1
        widgets_vector.push_back(heating_string);                               // 2
        widgets_vector.push_back(heating_timer_start_stop_btn);                 // 3
        widgets_vector.push_back(temperature_display);                          // 4
        widgets_vector.push_back(left_time_up_arrow_1);                         // 5
        widgets_vector.push_back(left_time_up_arrow_2);                         // 6
        widgets_vector.push_back(left_time_up_arrow_3);                         // 7
        widgets_vector.push_back(left_time_up_arrow_4);                         // 8
        widgets_vector.push_back(left_time_figure_1);                           // 9
        widgets_vector.push_back(left_time_figure_2);                           // 10
        widgets_vector.push_back(left_time_figure_3);                           // 11
        widgets_vector.push_back(left_time_figure_4);                           // 12
        widgets_vector.push_back(left_time_colon_char);                         // 13
        widgets_vector.push_back(left_time_down_arrow_1);                       // 14
        widgets_vector.push_back(left_time_down_arrow_2);                       // 15
        widgets_vector.push_back(left_time_down_arrow_3);                       // 16
        widgets_vector.push_back(left_time_down_arrow_4);                       // 17
        widgets_vector.push_back(left_temperature_entering_background);         // 18
        widgets_vector.push_back(temperature_left_keyboard_1);                  // 19
        widgets_vector.push_back(temperature_left_keyboard_2);                  // 20
        widgets_vector.push_back(temperature_left_keyboard_3);                  // 21
        widgets_vector.push_back(temperature_left_keyboard_4);                  // 22
        widgets_vector.push_back(temperature_left_keyboard_5);                  // 23
        widgets_vector.push_back(temperature_left_keyboard_6);                  // 24
        widgets_vector.push_back(temperature_left_keyboard_7);                  // 25
        widgets_vector.push_back(temperature_left_keyboard_8);                  // 26
        widgets_vector.push_back(temperature_left_keyboard_9);                  // 27
        widgets_vector.push_back(temperature_left_keyboard_0);                  // 28
        widgets_vector.push_back(temperature_left_keyboard_backspace);          // 29
        widgets_vector.push_back(temperature_left_keyboard_cancel);             // 30
        widgets_vector.push_back(temperature_left_keyboard_enter_top);          // 31
        widgets_vector.push_back(temperature_left_keyboard_enter_bottom);       // 32
        widgets_vector.push_back(heating_on_off_button);                        // 33
        widgets_vector.push_back(temperature_displ_preset);                     // 34
        widgets_vector.push_back(temperature_displ_current);                    // 35
        widgets_vector.push_back(temperature_displ_input);                      // 36
        
        widgets_vector.push_back(pressure_sensor_icon);                         // 37
        widgets_vector.push_back(vacuum_timer_start_stop_btn);                  // 38
        widgets_vector.push_back(pressure_display);                             // 39
        widgets_vector.push_back(vacuum_pump_string_1);                         // 40
        widgets_vector.push_back(vacuum_pump_string_2);                         // 41
        widgets_vector.push_back(right_time_up_arrow_1);                        // 42
        widgets_vector.push_back(right_time_up_arrow_2);                        // 43
        widgets_vector.push_back(right_time_up_arrow_3);                        // 44
        widgets_vector.push_back(right_time_up_arrow_4);                        // 45
        widgets_vector.push_back(right_time_figure_1);                          // 46
        widgets_vector.push_back(right_time_figure_2);                          // 47
        widgets_vector.push_back(right_time_figure_3);                          // 48
        widgets_vector.push_back(right_time_figure_4);                          // 49
        widgets_vector.push_back(right_time_down_arrow_1);                      // 50
        widgets_vector.push_back(right_time_down_arrow_2);                      // 51
        widgets_vector.push_back(right_time_down_arrow_3);                      // 52
        widgets_vector.push_back(right_time_down_arrow_4);                      // 53
        widgets_vector.push_back(temperature_right_entering_background);        // 54
        widgets_vector.push_back(temperature_right_keyboard_1);                 // 55
        widgets_vector.push_back(temperature_right_keyboard_2);                 // 56
        widgets_vector.push_back(temperature_right_keyboard_3);                 // 57
        widgets_vector.push_back(temperature_right_keyboard_4);                 // 58
        widgets_vector.push_back(temperature_right_keyboard_5);                 // 59
        widgets_vector.push_back(temperature_right_keyboard_6);                 // 60
        widgets_vector.push_back(temperature_right_keyboard_7);                 // 61
        widgets_vector.push_back(temperature_right_keyboard_8);                 // 62
        widgets_vector.push_back(temperature_right_keyboard_9);                 // 63
        widgets_vector.push_back(temperature_right_keyboard_0);                 // 64
        widgets_vector.push_back(temperature_right_keyboard_backspace);         // 65
        widgets_vector.push_back(temperature_right_keyboard_recet);             // 66
        widgets_vector.push_back(temperature_right_keyboard_enter_top);         // 67
        widgets_vector.push_back(temperature_right_keyboard_enter_bottom);      // 68
        widgets_vector.push_back(vacuum_on_off_button);                         // 69
        widgets_vector.push_back(pressure_displ_preset);                        // 70
        widgets_vector.push_back(pressure_displ_current);                       // 71
        widgets_vector.push_back(pressure_displ_input);                         // 72
        widgets_vector.push_back(right_time_colon_char);                        // 73
        widgets_vector.push_back(atm_string);                                   // 74

        break;
    }
    case SIMPLIFIED_USER_MODE:
    {

        break;
    }
    case SERVICE_MODE:
    {

        break;
    }
    }
    init_widgets_size();
}

// начальная отрисовка всех виджетов
void OvenDisplay::draw_all_widgets(void)
{
    for(vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)       // проходимся по вектору виджетов
    {
        // отрисовываем константные изображения
        widgets_vector[i].draw_img_vector(widgets_vector[i].constant_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        if(widgets_vector[i].button_name != NO_BUTTON)          // отрисовывам кнопки в ненажатом состоянии
        {
            widgets_vector[i].draw_img_vector(widgets_vector[i].released_btn_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        }
    }
}

// если размер виджета не задан вручную, автоматически берём размер первого изображения в виджете 
void OvenDisplay::init_widgets_size(void)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)                  // проходимся по вектору виджетов
    {
        if ((widgets_vector[i].wgt_width == 0) && (widgets_vector[i].wgt_height == 0))      // если размер виджета равен нулю
        {
            if (widgets_vector[i].released_btn_images.size() != 0)                          // если виджет является кнопкой
            {
                widgets_vector[i].wgt_width = widgets_vector[i].released_btn_images[0].image_struct.width;      // берём размеры изображения кнопки
                widgets_vector[i].wgt_height = widgets_vector[i].released_btn_images[0].image_struct.height;
            }
            else if (widgets_vector[i].constant_images.size() != 0)                         // если виджет не является кнопкой
            {
                widgets_vector[i].wgt_width = widgets_vector[i].constant_images[0].image_struct.width;      // берём размеры первого константного изображения
                widgets_vector[i].wgt_height = widgets_vector[i].constant_images[0].image_struct.height;
            }
        }
    }
}

// обработчик нажатия кнопки
void OvenDisplay::handle_button_press(Buttons_list pressed_button)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)                          // проходимся по вектору виджетов
    {
        if ((pressed_button == widgets_vector[i].button_name) && (widgets_vector[i].btn_locked))    // если данная кнопка заблокирована - выходим из функции
        {
            return;
        }
    }

    switch (pressed_button)         // вызываем функции, не связанные с отрисовкой кнопки
    {
    case NO_BUTTON:                 // проверяем, какая кнопка нажата
    {

        break;
    }
    case HEATING_TIMER_START_STOP:
    {
        break;
    }
    case LEFT_UP_ARROW_1:
    {
        if (!(main_device.process_timer_left.timer_enabled))                    // если левый таймер не включён
        {
            change_time_figure(INCREMENT, widgets_vector[9], &main_device.process_timer_left.hours_high_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);    // меняем цифру в таймере
        }
        break;
    }
    case LEFT_UP_ARROW_2:
    {
        if (!(main_device.process_timer_left.timer_enabled))
        {
            change_time_figure(INCREMENT, widgets_vector[10], &main_device.process_timer_left.hours_low_digit,
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_UP_ARROW_3:
    {
        if (!(main_device.process_timer_left.timer_enabled))
        {
            change_time_figure(INCREMENT, widgets_vector[11], &main_device.process_timer_left.minutes_high_digit, \
                                numbers_45_font_vector, SIX_DIGIT_MAX_NUMBER);
        }
        break;
    }
    case LEFT_UP_ARROW_4:
    {
        if (!(main_device.process_timer_left.timer_enabled))
        {
            change_time_figure(INCREMENT, widgets_vector[12], &main_device.process_timer_left.minutes_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_1:
    {
        if (!(main_device.process_timer_left.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[9], &main_device.process_timer_left.hours_high_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_2:
    {
        if (!(main_device.process_timer_left.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[10], &main_device.process_timer_left.hours_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_3:
    {
        if (!(main_device.process_timer_left.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[11], &main_device.process_timer_left.minutes_high_digit, \
                                numbers_45_font_vector, SIX_DIGIT_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_4:
    {
        if (!(main_device.process_timer_left.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[12], &main_device.process_timer_left.minutes_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_KEYBOARD_1:
    {
        widgets_vector[36].temper_input_add_number(1, main_device.input_temperature);       // добавляем введённую цифру к отображаемому значению
        break;
    }
    case LEFT_KEYBOARD_2:
    {
        widgets_vector[36].temper_input_add_number(2, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_3:
    {
        widgets_vector[36].temper_input_add_number(3, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_4:
    {
        widgets_vector[36].temper_input_add_number(4, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_5:
    {
        widgets_vector[36].temper_input_add_number(5, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_6:
    {
        widgets_vector[36].temper_input_add_number(6, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_7:
    {
        widgets_vector[36].temper_input_add_number(7, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_8:
    {
        widgets_vector[36].temper_input_add_number(8, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_9:
    {
        widgets_vector[36].temper_input_add_number(9, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_0:
    {
        widgets_vector[36].temper_input_add_number(0, main_device.input_temperature);
        break;
    }
    case LEFT_KEYBOARD_BACKSPACE:
    {
        widgets_vector[36].temper_input_backspace(main_device.input_temperature);           // стираем цифру из отображаемого значения
        break;
    }
    case LEFT_KEYBOARD_RECET:
    {
        widgets_vector[36].temper_input_reset(main_device.input_temperature);               // сбрасываем в ноль отображаемое значение
        break;
    }
    case LEFT_KEYBOARD_ENTER_TOP:
    {
        enter_related_event_left();                                      // общее событие для двух половин кнопки enter
        return;
        break;
    }
    case LEFT_KEYBOARD_ENTER_BOTTOM:
    {
        enter_related_event_left();
        return;
        break;
    }
    case HEATING_ON_OFF:
    {
        if (main_device.process_timer_left.timer_enabled)                   // если левый таймер включён
        {
            main_device.process_timer_left.stop_process_timer(LEFT_SIDE);   // выключаем таймер
        }
        else
        {
            main_device.process_timer_left.start_process_timer(LEFT_SIDE);  // иначе включаем таймер
        }

        if (main_device.heating_is_enabled)                             // включаем / выключаем нагрев
        {
            main_device.stop_heating();
        }
        else
        {
            main_device.start_heating();
        }

        if (main_device.vacuum_is_enabled)
        {
            main_device.stop_vacuum();
        }

        return;
        break;
    }
    // всё то же самое для правой части дисплея (вауумный насос)
    case VACUUM_TIMER_START_STOP:
    {
        if (main_device.process_timer_right.timer_enabled)
        {
            main_device.process_timer_right.stop_process_timer(RIGHT_SIDE);
        }
        else
        {
            main_device.process_timer_right.start_process_timer(RIGHT_SIDE);
        }
        return;
        break;
    }
    case RIGHT_UP_ARROW_1:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(INCREMENT, widgets_vector[46], &main_device.process_timer_right.hours_high_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_UP_ARROW_2:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(INCREMENT, widgets_vector[47], &main_device.process_timer_right.hours_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_UP_ARROW_3:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(INCREMENT, widgets_vector[48], &main_device.process_timer_right.minutes_high_digit, \
                                numbers_45_font_vector, SIX_DIGIT_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_UP_ARROW_4:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(INCREMENT, widgets_vector[49], &main_device.process_timer_right.minutes_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_DOWN_ARROW_1:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[46], &main_device.process_timer_right.hours_high_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_DOWN_ARROW_2:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[47], &main_device.process_timer_right.hours_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_DOWN_ARROW_3:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[48], &main_device.process_timer_right.minutes_high_digit, \
                                numbers_45_font_vector, SIX_DIGIT_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_DOWN_ARROW_4:
    {
        if (!(main_device.process_timer_right.timer_enabled))
        {
            change_time_figure(DECREMENT, widgets_vector[49], &main_device.process_timer_right.minutes_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case RIGHT_KEYBOARD_1:
    {
        widgets_vector[72].temper_input_add_number(1, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_2:
    {
        widgets_vector[72].temper_input_add_number(2, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_3:
    {
        widgets_vector[72].temper_input_add_number(3, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_4:
    {
        widgets_vector[72].temper_input_add_number(4, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_5:
    {
        widgets_vector[72].temper_input_add_number(5, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_6:
    {
        widgets_vector[72].temper_input_add_number(6, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_7:
    {
        widgets_vector[72].temper_input_add_number(7, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_8:
    {
        widgets_vector[72].temper_input_add_number(8, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_9:
    {
        widgets_vector[72].temper_input_add_number(9, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_0:
    {
        widgets_vector[72].temper_input_add_number(0, main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_BACKSPACE:
    {
        widgets_vector[72].temper_input_backspace(main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_RECET:
    {
        widgets_vector[72].temper_input_reset(main_device.input_pressure);
        break;
    }
    case RIGHT_KEYBOARD_ENTER_TOP:
    {
        enter_related_event_right();
        return;
        break;
    }
    case RIGHT_KEYBOARD_ENTER_BOTTOM:
    {
        enter_related_event_right();
        return;
        break;
    }
    case VACUUM_ON_OFF:
    {
        break;
    }
    
    default:
    {
        
        break;
    }
    }
    // отрисовываем кнопки
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)      // проходимся по всем виджетам
    {
        if (widgets_vector[i].button_name != NO_BUTTON)                         // если виджет является кнопкой
        {
            if (pressed_button != NO_BUTTON)                                // если нажата какая-либо кнопка
            {
                if (pressed_button == widgets_vector[i].button_name)        // если нажата данная кнопка
                {
                        
                    widgets_vector[i].button_is_pressed = true;             // выставляем флаг нажатия
                    widgets_vector[i].draw_img_vector(widgets_vector[i].pressed_btn_images, \
                                                        widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);  // отрисовываем нажатую кнопку
                }
                else        // если не нажата данная кнопка
                {
                    if (widgets_vector[i].button_is_pressed)                // если данная кнопка была нажата ранее
                    {
                        widgets_vector[i].button_is_pressed = false;        // выставляем флаг, что кнопка не нажата
                        widgets_vector[i].draw_img_vector(widgets_vector[i].released_btn_images, \
                                                            widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);      // отрисовываем отпущенную кнопку
                    }
                }
            }
            else            // если не нажата никакая кнопка
            {
                if (widgets_vector[i].button_is_pressed)                    // если данная кнопка была нажата ранее
                {
                    widgets_vector[i].button_is_pressed = false;            // выставляем флаг, что кнопка не нажата
                    widgets_vector[i].draw_img_vector(widgets_vector[i].released_btn_images, \
                                                        widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);          // отрисовываем отпущенную кнопку
                }
            }
        }
    }
}

// общее событие для двух половинок кнопки enter (левая часто экрана)
void OvenDisplay::enter_related_event_left(void)
{
    if (main_device.input_temperature < TEMPERATURE_CRITICAL)       // если не превышена критическая температура нагревателя
    {
        widgets_vector[36].temper_input_enter(main_device.input_temperature, main_device.preset_temperature, 34);   // вводим температуру с клавиатуры
    }
    else
    {
        widgets_vector[36].temper_input_enter(TEMPERATURE_CRITICAL, main_device.preset_temperature, 34);            // иначе вводим критическую температуру
    }

    if (main_device.heating_is_enabled)                             // если идёт нагрев
    {
        main_device.start_heating();                                // подаём команду "начать нагрев" (чтобы продолжать греть при вводе новой температуры)
    }
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)      // проходимся по всем виджетам
    {
        // если нажата одна из половинок кнопки enter, отрисовываем нажатие обеих
        if ((widgets_vector[i].button_name == LEFT_KEYBOARD_ENTER_TOP) || (widgets_vector[i].button_name == LEFT_KEYBOARD_ENTER_BOTTOM))
        {
            widgets_vector[i].button_is_pressed = true;
            widgets_vector[i].draw_img_vector(widgets_vector[i].pressed_btn_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        }
    }
}

// общее событие для двух половинок кнопки enter (правая часть экрана)
void OvenDisplay::enter_related_event_right(void)
{
    widgets_vector[72].temper_input_enter(main_device.input_pressure, main_device.preset_pressure, 70);
    if (main_device.vacuum_is_enabled)
    {
        main_device.start_vacuum();
    }
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        if ((widgets_vector[i].button_name == RIGHT_KEYBOARD_ENTER_TOP) || (widgets_vector[i].button_name == RIGHT_KEYBOARD_ENTER_BOTTOM))
        {
            widgets_vector[i].button_is_pressed = true;
            widgets_vector[i].draw_img_vector(widgets_vector[i].pressed_btn_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        }
    }
}

// определяем нажатую кнопку
uint16_t OvenDisplay::identify_pressed_btn(uint16_t pressing_coord_x, uint16_t pressing_coord_y)
{
    pressing_coord_x += CALIBRATION_OFFSET_X;       // добавляем калибровочное смещение, ибо тачскрин и дисплей могут не совпадать
    pressing_coord_y += CALIBRATION_OFFSET_Y;

    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)          // проходимся по вектору виджетов
    {
        if (widgets_vector[i].button_name != NO_BUTTON)                             // если данный вектор являетс кнопкой
        {
        if ((WITHIN(pressing_coord_x, widgets_vector[i].wgt_coord_x, \
        (widgets_vector[i].wgt_coord_x + widgets_vector[i].wgt_width))) && \
             (WITHIN(pressing_coord_y, widgets_vector[i].wgt_coord_y, \
             (widgets_vector[i].wgt_coord_y + widgets_vector[i].wgt_height))))      // если текущие координаты нажатия находятся внутри кнопки
        {
        return (uint16_t)widgets_vector[i].button_name;                             // возвращаем номер кнопки (определён через enum)
        }
        }
    }
    return 0;
}

// заполняем определённую область повторяющимся изображением
void Widget::tile_area(tImage image_to_tile, uint16_t area_width, uint16_t area_hight)
{
    uint16_t repetitions_x = area_width/image_to_tile.width;            // вычисляем количество повторений по X и Y
    uint16_t repetitions_y = area_width/image_to_tile.height;
    for (int i = 0; i <= repetitions_y; i++)                            // проходимся по горизонтали
    {
        for (int ii = 0; ii <= repetitions_x; ii++)                     // проходимся по вертикали
        {
            add_img_to_wgt(CONSTANT_IMG, image_to_tile, (image_to_tile.width * ii), (image_to_tile.height * i));    // отрисовываем фоновое изображение
        }
    }
}

// отрисовываем рамки
void Widget::set_rectangle(void)
{
    for (int i = 0; i < 26; i++)                                    // верхняя большая горизонтальная линия (левая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_horiz_line, ((i*10) + 117), 29);
    }
    for (int i = 0; i < 35; i++)                                    // нижняя горизонтальная линия (левая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_horiz_line, ((i*10) + 33), 568);
    }
    for (int i = 0; i < 7; i++)                                     // верхняя малая горизонтальная линия (левая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_horiz_line, ((i*10) + 33), 116);
    }

    for (int i = 0; i < 53; i++)                                    // правая вертикальная линия (левая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_vertic_line, 383, (i*10) + 38);
    }
    for (int i = 0; i < 44; i++)                                    // левая большая вертикальная линия (левая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_vertic_line, 23, (i*10) + 126);
    }    
    for (int i = 0; i < 8; i++)                                     // левая малая вертикальная линия (левая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_vertic_line, 107, (i*10) + 38);
    }

    for (int i = 0; i < 26; i++)                                    // верхняя большая горизонтальная линия (правая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_horiz_line, ((i*10) + 117 + RIGHT_COLUMN_OFFSET), 29);
    }
    for (int i = 0; i < 35; i++)                                    // нижняя горизонтальная линия (правая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_horiz_line, ((i*10) + 33 + RIGHT_COLUMN_OFFSET), 568);
    }
    for (int i = 0; i < 7; i++)                                     // верхняя малая горизонтальная линия (правая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_horiz_line, ((i*10) + 33 + RIGHT_COLUMN_OFFSET), 116);
    }

    for (int i = 0; i < 53; i++)                                    // правая вертикальная линия (правая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_vertic_line, 383 + RIGHT_COLUMN_OFFSET, (i*10) + 38);
    }
    for (int i = 0; i < 44; i++)                                    // левая большая вертикальная линия (правая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_vertic_line, 23 + RIGHT_COLUMN_OFFSET, (i*10) + 126);
    }    
    for (int i = 0; i < 8; i++)                                     // левая малая вертикальная линия (правая часть экрана)
    {
        add_img_to_wgt(CONSTANT_IMG, rect_vertic_line, 107 + RIGHT_COLUMN_OFFSET, (i*10) + 38);
    }

    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 107, 29);        // скруглённые углы слева
    add_img_to_wgt(CONSTANT_IMG, rect_top_right_round, 377, 29);
    add_img_to_wgt(CONSTANT_IMG, rect_down_right_round, 377, 561);
    add_img_to_wgt(CONSTANT_IMG, rect_down_left_round, 23,561);
    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 23, 116);
    add_img_to_wgt(CONSTANT_IMG, rect_internal_round, 100, 109);

    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 107 + RIGHT_COLUMN_OFFSET, 29);      // скруглённые углы справа
    add_img_to_wgt(CONSTANT_IMG, rect_top_right_round, 377 + RIGHT_COLUMN_OFFSET, 29);
    add_img_to_wgt(CONSTANT_IMG, rect_down_right_round, 377 + RIGHT_COLUMN_OFFSET, 561);
    add_img_to_wgt(CONSTANT_IMG, rect_down_left_round, 23 + RIGHT_COLUMN_OFFSET, 561);
    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 23 + RIGHT_COLUMN_OFFSET, 116);
    add_img_to_wgt(CONSTANT_IMG, rect_internal_round, 100 + RIGHT_COLUMN_OFFSET, 109);
}

// отрисовываем изображения, перечисленные в векторе
void Widget::draw_img_vector(vector<ImageObj> img_vector_to_draw, uint16_t parent_wgt_coord_x, uint16_t parent_wgt_coord_y)
{
    for(vector<ImageObj>::size_type ii = 0; ii != img_vector_to_draw.size(); ii++)  // проходимся по вектору изображений
    {
    TFT_DrawBitmap_d(img_vector_to_draw[ii].img_coord_x + parent_wgt_coord_x, \
                    img_vector_to_draw[ii].img_coord_y + parent_wgt_coord_y, \
                    img_vector_to_draw[ii].image_struct.data, \
                    img_vector_to_draw[ii].image_struct.width, \
                    img_vector_to_draw[ii].image_struct.height, \
                    MAIN_LAYER);                                                    // вызываем встроенную команду marlin
    }
}

// заменяем изображение в виджете
void Widget::change_image_in_widget(tImage image_to_output, uint16_t img_out_coord_x, uint16_t img_out_coord_y)
{
    changeable_images.clear();      // очищаем вектор изменяемых изображений в виджете
    add_img_to_wgt(CHANGEABLE_IMG, image_to_output, img_out_coord_x, img_out_coord_y);  // добавляем изображение в вектор изменяемых изображений
    draw_img_vector(changeable_images, wgt_coord_x, wgt_coord_y);                       // отрисовываем изменяемые изображения
}

// определяем координаты изображения в центре виджета
uint16_t Widget::img_center_x(tImage img_to_center)
{
    uint16_t centered_coord_x = (wgt_width/2) - img_to_center.width/2;
    return centered_coord_x;
}

// блокируем кнопку
void Widget::lock_button(void)
{
    button_is_pressed = false;                                          // убираем флаг нажатия
    draw_img_vector(blocked_btn_images, wgt_coord_x, wgt_coord_y);      // меняем изображение кнопки
    btn_locked = true;                                                  // включаем флаг блокировки
}

// разблокируем кнопку
void Widget::unlock_button(void)
{
    button_is_pressed = false;                                          // убираем флаг нажатия
    draw_img_vector(released_btn_images, wgt_coord_x, wgt_coord_y);     // меняем изображение кнопки
    btn_locked = false;                                                 // выключаем флаг блокировки
}

// выравниваем строку символов по правой стороне
uint16_t Widget::string_align_right_x(vector<tImage>& img_nmbrs_vect, uint8_t fnt_space, vector<uint8_t>& value_to_align)
{
    uint16_t string_summ_size = 0;                              // суммарная ширина строки
    for (int i = 0; i < value_to_align.size(); i++)             // проходимся по всем символам строки
    {
        string_summ_size += (img_nmbrs_vect[value_to_align[i]].width + fnt_space);      // прибавляем к суммарной ширине ширину символа и пробел между символами
    }
    if (string_summ_size <= wgt_width)                                                  // если не превышена ширина виджета
    {
        string_summ_size = wgt_width - string_summ_size;                                // получаем координату начала строки в виджете
    }
    else
    {
        string_summ_size = 0;                                                           // иначе обнуляем ширину строки
    }
    return string_summ_size;                                                            // возвращаем ширину строки
}

// конвертируем числовое значение в массив цифр (по разрядам)
void Widget::convert_value_to_int_arr(vector<uint8_t>& value_int_vect,  uint32_t value_to_conv)
{
    uint16_t number_of_decimal_digits = 0;              // количество десячиных разрядов
    uint32_t dec_divider_buf_1 = 1;                     // десятка в степени, на неё делим
    uint32_t prev_digits_buf = 0;                       // буфер старших разрядов, его вычитаем

    if (value_to_conv > 0)                              // если наше значение больше нуля
    {
        while (dec_divider_buf_1 <= value_to_conv)      // проходимся по всем разрядам
        {
            number_of_decimal_digits++;                 // инкрементируем количество разрядов
            dec_divider_buf_1 *= 10;                    // умножаем на 10 десятку в степени
        }
        dec_divider_buf_1 /= 10;                        // делим на 10 десятку в степени (чтобы был первый разряд)
    }
    else
    {
        number_of_decimal_digits = 1;                   // если значение равно нулю, количество разрядов равно 1
    }

    value_int_vect.resize(number_of_decimal_digits);    // меняем размер вектора для нужного количества разрядов 
    for (uint16_t i = number_of_decimal_digits; i > 0; i--)     // проходимся по каждому десятичному разряду 
    {
        value_int_vect[i - 1] = value_to_conv/(dec_divider_buf_1) - prev_digits_buf;    // делим текущий разряд на десятку в степени, затем вычитаем более старшие разряды
        dec_divider_buf_1 /= 10;                        // уменьшаем на разряд десятку в степени
        prev_digits_buf += (value_int_vect[i - 1]);     // прибавляем текущий разряд к буферу старших разрядов
        prev_digits_buf *= 10;                          // домножаем старшие разряды на 10
    }
}

// изменить числовое значение в виджете (оно является строкой)
void Widget::change_value_in_wgt(Alignment numbers_align, uint8_t font_space, vector<tImage>& img_font, uint32_t value_to_displ)
{
    vector<uint8_t> value_digits_array;                                 // создаём массив разрядов
    convert_value_to_int_arr(value_digits_array, value_to_displ);       // конвертируем число в массив разрядов
    int16_t init_coord_x = 0;                                           // начальная координата строки в виджете

    while (init_coord_x <= (wgt_width - (img_font[IMG_BACKGR_IN_VECT].width)))  // пока координата строки меньше ширины виджета за вычетом ширины одного символа
    {
        change_image_in_widget(img_font[IMG_BACKGR_IN_VECT], init_coord_x, 0);  // отрисовываем фон символа
        init_coord_x += img_font[IMG_BACKGR_IN_VECT].width;                     // прибавляем к начальной координате ширину фона символа
    }
    init_coord_x = wgt_width - img_font[IMG_BACKGR_IN_VECT].width + 4;          // начальная координата равна ширине виджета за вычетом ширины одного символа + 4 пикселя (вправо)
    change_image_in_widget(img_font[IMG_BACKGR_IN_VECT], init_coord_x, 0);      // отрисовываем крайний правый фон символа

    switch (numbers_align)
    {
    case ALIGN_LEFT:                // если выравнивание по левой стороне
    {
        init_coord_x = 0;           // начинаем с начала виджета
        break;
    }
    case ALIGN_RIGHT:               // если выравнивание по правой стороне
    {
        init_coord_x = string_align_right_x(img_font, font_space, value_digits_array);      // определяем координату строки в виджете
        break;
    }
    }
    uint16_t img_string_size_buf = 0;                                                       // буфер размера строки
    for (vector<uint8_t>::size_type i = value_digits_array.size(); i > 0; i--)              // проходимся по массиву разрядов
    {
        change_image_in_widget(img_font[value_digits_array[i - 1]], (init_coord_x + img_string_size_buf), 0);   // отрисовываем разряд
        img_string_size_buf += (img_font[value_digits_array[i - 1]].width + font_space);                        // прибавляем к размеру строки ширину символа + ширину пробела
    }
}

// добавляем разряд к вводимому числу
void Widget::temper_input_add_number(uint8_t num_to_enter, uint16_t &variable_to_change)
{
    if (variable_to_change <= 99)                               // можем ввести не более трёх разрядов
    {
        variable_to_change *= 10;                               // домножаем на 10 имеющееся число
        variable_to_change += num_to_enter;                     // прибавляем новый разряд
        change_value_in_wgt(ALIGN_RIGHT, FONT_11_GAP_PIX, oven_display.numbers_11_font_vector, variable_to_change);     // меняем число в виджете
    }
}

// удаляем младший разряд из вводимого числа
void Widget::temper_input_backspace(uint16_t &variable_to_change)
{
    if (variable_to_change > 0)         // если число больше нуля
    {
        variable_to_change /= 10;       // делим число на десять
        change_value_in_wgt(ALIGN_RIGHT, FONT_11_GAP_PIX, oven_display.numbers_11_font_vector, variable_to_change);     // меняем число в виджете
    }
}

// сбрасываем вводимое число
void Widget::temper_input_reset(uint16_t &variable_to_change)
{
    if (variable_to_change != 0)        // если число не равно нулю
    {
        variable_to_change = 0;         // приравниваем к нулю
        change_value_in_wgt(ALIGN_RIGHT, FONT_11_GAP_PIX, oven_display.numbers_11_font_vector, variable_to_change);     // меняем число в виджете
    }
}

// вводимое значение становится заданным
void Widget::temper_input_enter(uint16_t changed_variable, uint16_t &variable_to_write, uint16_t wgt_to_output_numbr)
{
    variable_to_write = changed_variable;       // отправляем изменённую переменную в целевую переменную
    oven_display.widgets_vector[wgt_to_output_numbr].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, oven_display.numbers_30_font_vector, variable_to_write); // отрисовываем
}

void Widget::check_slider_lock(void)
{
    if (main_device.process_timer_left.time_not_zero())             // если введённое время не равно нулю
    {
        if (btn_locked)                          // если кнопка-слайдер заблокирована
        {
            unlock_button();                     // разблокируем кнопку
        }
    }
    else                                                            // если введённое время равно нулю
    {
        if (!(btn_locked))                       // если кнопка-слайдер не заблокирована
        {
            lock_button();                       // блокируем кнопку
        }
    }
}

// меняем цифру в таймере
void OvenDisplay::change_time_figure(Decr_Incr chng_type, Widget& figure_widget, uint32_t* digit, vector<tImage>& nmbrs_img_vect, uint8_t digit_max_value)
{
    switch (chng_type)
    {
        case DECREMENT:                     // если декрементируем
        {
            if (*digit > 0)                 // если текущий разряд больше 0
            {
                *digit = *digit - 1;        // декрементируем
            }
            else
            {
                *digit = digit_max_value;   // иначе присваиваем максимальное значение
            }
            break;
        }
        case INCREMENT:                     // с инкрементном наоборот
        {
            if (*digit < digit_max_value)
            {
                *digit = *digit + 1;
            }
            else
            {
                *digit = 0;
            }
            break;
        }
    }
    replace_time_figure(figure_widget, nmbrs_img_vect[*digit]);     // отрисовываем введённую цифру

    widgets_vector[33].check_slider_lock();
}

// отрисовываем цифру в таймере
void OvenDisplay::replace_time_figure(Widget& figure_widget, tImage& nmbr_img)
{
    figure_widget.change_image_in_widget(numbers_45_font_vector[IMG_BACKGR_IN_VECT], \
                                        figure_widget.img_center_x(numbers_45_font_vector[IMG_BACKGR_IN_VECT]), TIME_FIGURE_Y_OFFSET);  // отрисовываем фон символа
    figure_widget.change_image_in_widget(nmbr_img, figure_widget.img_center_x(nmbr_img), TIME_FIGURE_Y_OFFSET);                         // отрисовываем символ
}

// инициализируем массивы изображений цифр (номер в массиве соответствует самой цифре)
void OvenDisplay::init_fonts_img_vect(void)
{
    // шрифт 45 пикселей, тёмный фон
    numbers_45_font_vector.push_back(numbers_45_0);
    numbers_45_font_vector.push_back(numbers_45_1);
    numbers_45_font_vector.push_back(numbers_45_2);
    numbers_45_font_vector.push_back(numbers_45_3);
    numbers_45_font_vector.push_back(numbers_45_4);
    numbers_45_font_vector.push_back(numbers_45_5);
    numbers_45_font_vector.push_back(numbers_45_6);
    numbers_45_font_vector.push_back(numbers_45_7);
    numbers_45_font_vector.push_back(numbers_45_8);
    numbers_45_font_vector.push_back(numbers_45_9);
    numbers_45_font_vector.push_back(numbers_45_background);

    // шрифт 30 пикселей, тёмный фон
    numbers_30_font_vector.push_back(numbers_30_0);
    numbers_30_font_vector.push_back(numbers_30_1);
    numbers_30_font_vector.push_back(numbers_30_2);
    numbers_30_font_vector.push_back(numbers_30_3);
    numbers_30_font_vector.push_back(numbers_30_4);
    numbers_30_font_vector.push_back(numbers_30_5);
    numbers_30_font_vector.push_back(numbers_30_6);
    numbers_30_font_vector.push_back(numbers_30_7);
    numbers_30_font_vector.push_back(numbers_30_8);
    numbers_30_font_vector.push_back(numbers_30_9);
    numbers_30_font_vector.push_back(numbers_30_background);

    // шрифт 11 пикселей, оранжевый фон
    numbers_11_font_vector.push_back(numbers_11_0);
    numbers_11_font_vector.push_back(numbers_11_1);
    numbers_11_font_vector.push_back(numbers_11_2);
    numbers_11_font_vector.push_back(numbers_11_3);
    numbers_11_font_vector.push_back(numbers_11_4);
    numbers_11_font_vector.push_back(numbers_11_5);
    numbers_11_font_vector.push_back(numbers_11_6);
    numbers_11_font_vector.push_back(numbers_11_7);
    numbers_11_font_vector.push_back(numbers_11_8);
    numbers_11_font_vector.push_back(numbers_11_9);
    numbers_11_font_vector.push_back(numbers_11_background);
}

// начальная отрисовка отображаемых значений
void OvenDisplay::init_displayed_values(void)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)      // отрисовываем цифры в таймерах
    {
        if (
            (i == 9)  || \
            (i == 10) || \
            (i == 11) || \
            (i == 12) 
            /* || \
            (i == 46) || \
            (i == 47) || \
            (i == 48) || \
            (i == 49)
            */
            )
        {
            widgets_vector[i].change_image_in_widget(numbers_45_0, widgets_vector[i].img_center_x(numbers_45_0), TIME_FIGURE_Y_OFFSET);
        }
    }
    widgets_vector[34].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, numbers_30_font_vector, main_device.preset_temperature);   // заданная температура
    widgets_vector[35].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, numbers_30_font_vector, main_device.current_temperature);  // текущая температура
    widgets_vector[36].change_value_in_wgt(ALIGN_RIGHT, FONT_11_GAP_PIX, numbers_11_font_vector, main_device.input_temperature);    // вводимая температура (над клавиатурой)

    if (SHOW_ALL_SENSORS)
    {
        widgets_vector[70].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, numbers_30_font_vector, main_device.preset_pressure);  // то же самое для правого значения
        widgets_vector[71].change_value_in_wgt(ALIGN_RIGHT, FONT_30_GAP_PIX, numbers_30_font_vector, main_device.current_pressure);
        widgets_vector[72].change_value_in_wgt(ALIGN_RIGHT, FONT_11_GAP_PIX, numbers_11_font_vector, main_device.input_pressure);
    }
}

// переключить двоеточие в таймере
void OvenDisplay::blink_clock_colon(Side_of_screen screen_side)
{
    switch (screen_side)    // в какой половине дисплея работает таймер
    {
    case (LEFT_SIDE):
    {
        if (left_colon_displayed)   // если двоеточие отображается
        {
            widgets_vector[13].change_image_in_widget(img_time_colon_backgr, 0, 0);    // отрисовываем фон
            left_colon_displayed = false;                                              // переключаем флаг
        }
        else
        {
            widgets_vector[13].change_image_in_widget(img_time_colon_char, 0, 0);       // отрисовываем двоеточие
            left_colon_displayed = true;                                                // переключаем флаг
        }
        break;
    }
    case RIGHT_SIDE:
    {
        if (right_colon_displayed)
        {
            widgets_vector[73].change_image_in_widget(img_time_colon_backgr, 0, 0);
            right_colon_displayed = false;
        }
        else
        {
            widgets_vector[73].change_image_in_widget(img_time_colon_char, 0, 0);
            right_colon_displayed = true;
        }
        break;
    }
    }
}

// заблокировать стрелки таймера
void OvenDisplay::lock_arrows(Side_of_screen screen_side)
{
    switch (screen_side)        // в какой половине дисплея
    {
        case LEFT_SIDE:
        {
        for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)  // проходимся по всем виджетам стрелок
        {
            if (
                (i == 5) || \
                (i == 6) || \
                (i == 7) || \
                (i == 8) || \
                (i == 14) || \
                (i == 15) || \
                (i == 16) || \
                (i == 17)
                )
            {
                widgets_vector[i].lock_button();                                // блокируем
            }
        }
            break;
        }
        case RIGHT_SIDE:
        {
        for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
        {
            if (
                (i == 42) || \
                (i == 43) || \
                (i == 44) || \
                (i == 45) || \
                (i == 50) || \
                (i == 51) || \
                (i == 52) || \
                (i == 53)
                )
                {
                    widgets_vector[i].lock_button();
                }
        }
        break;
        }
    }
}

// разблокировать стрелки таймера
void OvenDisplay::unlock_arrows(Side_of_screen screen_side)
{
    switch (screen_side)
    {
    case LEFT_SIDE:
    {
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        if (
            (i == 5) || \
            (i == 6) || \
            (i == 7) || \
            (i == 8) || \
            (i == 14) || \
            (i == 15) || \
            (i == 16) || \
            (i == 17)
            )
        {
            widgets_vector[i].unlock_button();
        }
    }
    break;
    }
    case RIGHT_SIDE:
    {
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        if (
            (i == 42) || \
            (i == 43) || \
            (i == 44) || \
            (i == 45) || \
            (i == 50) || \
            (i == 51) || \
            (i == 52) || \
            (i == 53)
            )
        {
            widgets_vector[i].unlock_button();
        }
    }
    }
    break;
    }
}

// проверка, что отсчитали нужное количество семплов для данной кнопки
bool OvenDisplay::bounce_filter_passed(Buttons_list pressed_button)
{
    if (pressed_button != bounce_btn_buff)              // если текущая кнопка не равна кнопке, для которой считали дребезг
    {
        bounce_btn_buff = pressed_button;               // начинаем считать дребезг для текущей кнопки
        bounce_sample_counter = 0;                      // обнуляем счетчик семплов
    }
    if (bounce_sample_counter >= BOUNCE_MAX_SAMPLES)    // если досчитали до порогового количесвта семплов
    {
        bounce_sample_counter = BOUNCE_MAX_SAMPLES;     // заглушка, чтобы не считать бесконечно
        return true;                                    // нажатие кнопки произошло
    }
    else
    {
        return false;                                   // нажатие кнопки не произошло
    }
}