#include "ltdc.h"
#include "oven_display.h"
#include "../core/macros.h"
#include "oven_core.h"



/*
Widget pressure_sensor_icon = {false, 0, 0, 0, 0};
Widget vacuum_pump_string = {false, 0, 0, 0, 0};
Widget vacuum_timer_start_button = {false, 0, 0, 0, 0};
Widget vacuum_timer_stop_button = {false, 0, 0, 0, 0};
Widget pressure_display = {false, 0, 0, 0, 0};
Widget left_time_up_arrow_5 = {false, 0, 0, 0, 0};
Widget left_time_up_arrow_6 = {false, 0, 0, 0, 0};
Widget left_time_up_arrow_7 = {false, 0, 0, 0, 0};
Widget left_time_up_arrow_8 = {false, 0, 0, 0, 0};
Widget left_time_figure_5 = {false, 0, 0, 0, 0};
Widget left_time_figure_6 = {false, 0, 0, 0, 0};
Widget left_time_figure_7 = {false, 0, 0, 0, 0};
Widget left_time_figure_8 = {false, 0, 0, 0, 0};
Widget left_time_down_arrow_5 = {false, 0, 0, 0, 0};
Widget left_time_down_arrow_6 = {false, 0, 0, 0, 0};
Widget left_time_down_arrow_7 = {false, 0, 0, 0, 0};
Widget left_time_down_arrow_8 = {false, 0, 0, 0, 0};
Widget temperature_right_entering_background = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_1 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_2 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_3 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_4 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_5 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_6 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_7 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_8 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_9 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_0 = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_backspace = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_recet = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_enter_top = {false, 0, 0, 0, 0};
Widget temperature_right_keyboard_enter_bottom = {false, 0, 0, 0, 0};
Widget vacuum_on_off_button = {false, 0, 0, 0, 0};
*/

OvenDisplay oven_display;

Widget::Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y)
{
    button_name = btn_name;
    wgt_coord_x = wgt_x;
    wgt_coord_y = wgt_y;
    wgt_width = 0;
    wgt_height = 0;
    button_is_pressed = false;
    wgt_img_changed = false;
    btn_locked = false;
}

Widget::Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y, uint16_t wgt_wdth, uint16_t wgt_hght)
{
    button_name = btn_name;
    wgt_coord_x = wgt_x;
    wgt_coord_y = wgt_y;
    wgt_width = wgt_wdth;
    wgt_height = wgt_hght;
    button_is_pressed = false;
    wgt_img_changed = false;
    btn_locked = false;
}

void Widget::add_img_to_wgt(Img_vect_list vect_to_add_type, tImage add_image_generated, uint16_t add_img_coord_x, uint16_t add_img_coord_y)
{
    ImageObj tmp_img_obj_1;
    tmp_img_obj_1.img_coord_x = add_img_coord_x;
    tmp_img_obj_1.img_coord_y = add_img_coord_y;
    tmp_img_obj_1.image_struct = add_image_generated;
    
    switch (vect_to_add_type)
    {
    case CONSTANT_IMG:
    {
        constant_images.push_back(tmp_img_obj_1);
        break;
    }
    case CHANGEABLE_IMG:
    {
        changeable_images.push_back(tmp_img_obj_1);
        break;
    }
    case BTN_PRESSED_IMG:
    {
        pressed_btn_images.push_back(tmp_img_obj_1);
        break;
    }
    case BTN_RELEASED_IMG:
    {
        released_btn_images.push_back(tmp_img_obj_1);
        break;
    }
    case BTN_BLOCKED_IMG:
        blocked_btn_images.push_back(tmp_img_obj_1);
        break;
    }
}

OvenDisplay::OvenDisplay(void)
{
    display_mode = DEFAULT_DISPLAY_MODE;
    display_width = DISPLAY_WIDTH;
    display_height = DISPLAY_HEIGHT;
    draw_all_completed = false;
    colon_displayed = true;
    previous_button = NO_BUTTON;
    bounce_sample_counter = 0;
    bounce_btn_buff = NO_BUTTON;
    init_numbrs_img_vect();
    init_widgets();
}

void OvenDisplay::test_draw(void)
{
    TFT_DrawBitmap_d(100, 100, rocket.data, rocket.width, rocket.height, MAIN_LAYER);
}

void OvenDisplay::init_widgets(void)
{
    Widget background_frame = {NO_BUTTON, 0, 0};
    Widget thermometer_icon = {NO_BUTTON, 29, 32};
    Widget heating_string = {NO_BUTTON, 120, 42};
    Widget heating_timer_start_stop_btn = {HEATING_TIMER_START_STOP, 88, 127, 0, 0};
    Widget temperature_display = {NO_BUTTON, 200, 50};
    Widget left_time_up_arrow_1 = {LEFT_UP_ARROW_1, 49, 200, 0, 0};
    Widget left_time_up_arrow_2 = {LEFT_UP_ARROW_2, 123, 200, 0, 0};
    Widget left_time_up_arrow_3 = {LEFT_UP_ARROW_3, 218, 200, 0, 0};
    Widget left_time_up_arrow_4 = {LEFT_UP_ARROW_4, 292, 200, 0, 0};
    Widget left_time_figure_1 = {NO_BUTTON, 49, 230};
    Widget left_time_figure_2 = {NO_BUTTON, 124, 230};
    Widget left_time_figure_3 = {NO_BUTTON, 218, 230};
    Widget left_time_figure_4 = {NO_BUTTON, 292, 230};
    Widget left_time_colon_char = {NO_BUTTON, 201, 269};
    Widget left_time_down_arrow_1 = {LEFT_DOWN_ARROW_1, 49, 320, 0, 0};
    Widget left_time_down_arrow_2 = {LEFT_DOWN_ARROW_2, 123, 320, 0, 0};
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

    background_frame.tile_area(backgr_tile, display_width, display_height);
    background_frame.set_rectangle();
    thermometer_icon.add_img_to_wgt(CONSTANT_IMG, img_thermometer_icon_crossed, 0, 0);
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
    left_time_figure_2.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_3.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_4.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
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

    widgets_vector.clear();
    switch (display_mode)
    {
    case STANDARD_USER_MODE:
    {
        widgets_vector.push_back(background_frame);                             // 0
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
        /*
        widgets_vector.push_back(pressure_sensor_icon);
        widgets_vector.push_back(vacuum_pump_string);
        widgets_vector.push_back(vacuum_timer_start_button);
        widgets_vector.push_back(vacuum_timer_stop_button);
        widgets_vector.push_back(pressure_display);
        widgets_vector.push_back(left_time_up_arrow_5);
        widgets_vector.push_back(left_time_up_arrow_6);
        widgets_vector.push_back(left_time_up_arrow_7);
        widgets_vector.push_back(left_time_up_arrow_8);
        widgets_vector.push_back(left_time_figure_5);
        widgets_vector.push_back(left_time_figure_6);
        widgets_vector.push_back(left_time_figure_7);
        widgets_vector.push_back(left_time_figure_8);
        widgets_vector.push_back(left_time_down_arrow_5);
        widgets_vector.push_back(left_time_down_arrow_6);
        widgets_vector.push_back(left_time_down_arrow_7);
        widgets_vector.push_back(left_time_down_arrow_8);
        widgets_vector.push_back(temperature_right_entering_background);
        widgets_vector.push_back(temperature_right_keyboard_1);
        widgets_vector.push_back(temperature_right_keyboard_2);
        widgets_vector.push_back(temperature_right_keyboard_3);
        widgets_vector.push_back(temperature_right_keyboard_4);
        widgets_vector.push_back(temperature_right_keyboard_5);
        widgets_vector.push_back(temperature_right_keyboard_6);
        widgets_vector.push_back(temperature_right_keyboard_7);
        widgets_vector.push_back(temperature_right_keyboard_8);
        widgets_vector.push_back(temperature_right_keyboard_9);
        widgets_vector.push_back(temperature_right_keyboard_0);
        widgets_vector.push_back(temperature_right_keyboard_backspace);
        widgets_vector.push_back(temperature_right_keyboard_recet);
        widgets_vector.push_back(temperature_right_keyboard_enter_top);
        widgets_vector.push_back(temperature_right_keyboard_enter_bottom);
        widgets_vector.push_back(vacuum_on_off_button);
        */

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

void OvenDisplay::draw_all_widgets(void)
{
    for(vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        widgets_vector[i].draw_img_vector(widgets_vector[i].constant_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        widgets_vector[i].draw_img_vector(widgets_vector[i].released_btn_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        if(widgets_vector[i].button_name != NO_BUTTON)          // отрисовывам кнопки в ненажатом состоянии
        {
            widgets_vector[i].draw_img_vector(widgets_vector[i].released_btn_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        }
    }
}

void OvenDisplay::update_all_widgets(void)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        if (widgets_vector[i].wgt_img_changed)
        {
            widgets_vector[i].wgt_img_changed = false;
            widgets_vector[i].draw_img_vector(widgets_vector[i].changeable_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
        }
    }
}

void OvenDisplay::init_widgets_size(void)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        if ((widgets_vector[i].wgt_width == 0) && (widgets_vector[i].wgt_height == 0))
        {
            if (widgets_vector[i].released_btn_images.size() != 0)
            {
                widgets_vector[i].wgt_width = widgets_vector[i].released_btn_images[0].image_struct.width;
                widgets_vector[i].wgt_height = widgets_vector[i].released_btn_images[0].image_struct.height;
            }
            else if (widgets_vector[i].constant_images.size() != 0)
            {
                widgets_vector[i].wgt_width = widgets_vector[i].constant_images[0].image_struct.width;
                widgets_vector[i].wgt_height = widgets_vector[i].constant_images[0].image_struct.height;
            }
        }
    }
}

void OvenDisplay::handle_button_press(Buttons_list pressed_button)
{
    switch (pressed_button)         // вызываем функции, не связанные с отрисовкой кнопки
    {
    case NO_BUTTON:
    {

        break;
    }
    case HEATING_TIMER_START_STOP:
    {
        if (main_device.heating_timer_started)
        {
            main_device.process_timer_left.stop_process_timer();
            
        }
        else
        {
            main_device.process_timer_left.start_process_timer();
        }
        return;
        break;
    }
    case LEFT_UP_ARROW_1:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(INCREMENT, widgets_vector[9], &main_device.process_timer_left.hours_high_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_UP_ARROW_2:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(INCREMENT, widgets_vector[10], &main_device.process_timer_left.hours_low_digit,
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_UP_ARROW_3:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(INCREMENT, widgets_vector[11], &main_device.process_timer_left.minutes_high_digit, \
                                numbers_45_font_vector, SIX_DIGIT_MAX_NUMBER);
        }
        break;
    }
    case LEFT_UP_ARROW_4:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(INCREMENT, widgets_vector[12], &main_device.process_timer_left.minutes_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_1:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(DECREMENT, widgets_vector[9], &main_device.process_timer_left.hours_high_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_2:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(DECREMENT, widgets_vector[10], &main_device.process_timer_left.hours_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_3:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(DECREMENT, widgets_vector[11], &main_device.process_timer_left.minutes_high_digit, \
                                numbers_45_font_vector, SIX_DIGIT_MAX_NUMBER);
        }
        break;
    }
    case LEFT_DOWN_ARROW_4:
    {
        if (!(main_device.heating_timer_started))
        {
            change_time_figure(DECREMENT, widgets_vector[12], &main_device.process_timer_left.minutes_low_digit, \
                                numbers_45_font_vector, DECIMAL_MAX_NUMBER);
        }
        break;
    }
    case LEFT_KEYBOARD_1:
    {

        break;
    }
    case LEFT_KEYBOARD_2:
    {

        break;
    }
    case LEFT_KEYBOARD_3:
    {

        break;
    }
    case LEFT_KEYBOARD_4:
    {

        break;
    }
    case LEFT_KEYBOARD_5:
    {

        break;
    }
    case LEFT_KEYBOARD_6:
    {

        break;
    }
    case LEFT_KEYBOARD_7:
    {

        break;
    }
    case LEFT_KEYBOARD_8:
    {

        break;
    }
    case LEFT_KEYBOARD_9:
    {

        break;
    }
    case LEFT_KEYBOARD_0:
    {

        break;
    }
    case LEFT_KEYBOARD_BACKSPACE:
    {

        break;
    }
    case LEFT_KEYBOARD_RECET:
    {

        break;
    }
    case LEFT_KEYBOARD_ENTER_TOP:
    {
        enter_related_event();
        return;
        break;
    }
    case LEFT_KEYBOARD_ENTER_BOTTOM:
    {
        enter_related_event();
        return;
        break;
    }
    case HEATING_ON_OFF:
    {
        if (main_device.heating_is_enabled)
        {
            main_device.heating_is_enabled = false;
            widgets_vector[33].change_image_in_widget(img_slide_button_off, 0, 0);
            widgets_vector[1].change_image_in_widget(img_thermometer_icon_crossed, 0, 0);

        }
        else
        {
            main_device.heating_is_enabled = true;
            widgets_vector[33].change_image_in_widget(img_slide_button_on, 0, 0);
            widgets_vector[1].change_image_in_widget(img_thermometer_icon_enabled, 0, 0);
        }
        return;
        break;
    }

    case VACUUM_TIMER_START:
    {

        break;
    }
    case VACUUM_TIMER_STOP:
    {

        break;
    }
    case RIGHT_UP_ARROW_1:
    {

        break;
    }
    case RIGHT_UP_ARROW_2:
    {

        break;
    }
    case RIGHT_UP_ARROW_3:
    {

        break;
    }
    case RIGHT_UP_ARROW_4:
    {

        break;
    }
    case RIGHT_DOWN_ARROW_1:
    {

        break;
    }
    case RIGHT_DOWN_ARROW_2:
    {

        break;
    }
    case RIGHT_DOWN_ARROW_3:
    {

        break;
    }
    case RIGHT_DOWN_ARROW_4:
    {

        break;
    }
    case RIGHT_KEYBOARD_1:
    {

        break;
    }
    case RIGHT_KEYBOARD_2:
    {

        break;
    }
    case RIGHT_KEYBOARD_3:
    {

        break;
    }
    case RIGHT_KEYBOARD_4:
    {

        break;
    }
    case RIGHT_KEYBOARD_5:
    {

        break;
    }
    case RIGHT_KEYBOARD_6:
    {

        break;
    }
    case RIGHT_KEYBOARD_7:
    {

        break;
    }
    case RIGHT_KEYBOARD_8:
    {

        break;
    }
    case RIGHT_KEYBOARD_9:
    {

        break;
    }
    case RIGHT_KEYBOARD_0:
    {

        break;
    }
    case RIGHT_KEYBOARD_BACKSPACE:
    {

        break;
    }
    case RIGHT_KEYBOARD_RECET:
    {

        break;
    }
    case RIGHT_KEYBOARD_ENTER_TOP:
    {

        break;
    }
    case RIGHT_KEYBOARD_ENTER_BOTTOM:
    {

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

    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)      // проходимся по всем виджетам
    {
        if (widgets_vector[i].button_name != NO_BUTTON)                         // если виджет является кнопкой
        {
            if (!(widgets_vector[i].btn_locked))                                // если данная кнопка не заблокирована
            {
                if (pressed_button != NO_BUTTON)                                // если нажата какая-либо кнопка
                {
                    if (pressed_button == widgets_vector[i].button_name)        // если нажата данная кнопка
                    {
                        
                        widgets_vector[i].button_is_pressed = true;
                        widgets_vector[i].draw_img_vector(widgets_vector[i].pressed_btn_images, \
                                                            widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
                    }
                    else        // если не нажата данная кнопка
                    {
                        if (widgets_vector[i].button_is_pressed)        // если данная кнопка была нажата ранее
                        {
                            widgets_vector[i].button_is_pressed = false;
                            widgets_vector[i].draw_img_vector(widgets_vector[i].released_btn_images, \
                                                                widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
                        }
                    }
                }
                else            // если не нажата никакая кнопка
                {
                    if (widgets_vector[i].button_is_pressed)            // если данная кнопка была нажата ранее
                    {
                        widgets_vector[i].button_is_pressed = false;
                        widgets_vector[i].draw_img_vector(widgets_vector[i].released_btn_images, \
                                                            widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
                    }
                }
            }
        }
    }
}

void OvenDisplay::enter_related_event(void)
{
     for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
     {
         if ((widgets_vector[i].button_name == LEFT_KEYBOARD_ENTER_TOP) || (widgets_vector[i].button_name == LEFT_KEYBOARD_ENTER_BOTTOM))
         {
            widgets_vector[i].button_is_pressed = true;
            widgets_vector[i].draw_img_vector(widgets_vector[i].pressed_btn_images, widgets_vector[i].wgt_coord_x, widgets_vector[i].wgt_coord_y);
         }
     }
}

uint16_t OvenDisplay::identify_pressed_btn(uint16_t pressing_coord_x, uint16_t pressing_coord_y)
{
    pressing_coord_x += CALIBRATION_OFFSET_X;
    pressing_coord_y += CALIBRATION_OFFSET_Y;

    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        if (widgets_vector[i].button_name != NO_BUTTON)
        {
        if ((WITHIN(pressing_coord_x, widgets_vector[i].wgt_coord_x, \
        (widgets_vector[i].wgt_coord_x + widgets_vector[i].wgt_width))) && \
             (WITHIN(pressing_coord_y, widgets_vector[i].wgt_coord_y, \
             (widgets_vector[i].wgt_coord_y + widgets_vector[i].wgt_height))))
        {
        return (uint16_t)widgets_vector[i].button_name;
        }
        }
    }
    return 0;
}

void Widget::tile_area(tImage image_to_tile, uint16_t area_width, uint16_t area_hight)
{
    uint16_t repetitions_x = area_width/image_to_tile.width;
    uint16_t repetitions_y = area_width/image_to_tile.height;
    for (int i = 0; i <= repetitions_y; i++)
    {
        for (int ii = 0; ii <= repetitions_x; ii++)
        {
            add_img_to_wgt(CONSTANT_IMG, image_to_tile, (image_to_tile.width * ii), (image_to_tile.height * i));
        }
    }
}

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

    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 107, 29);
    add_img_to_wgt(CONSTANT_IMG, rect_top_right_round, 377, 29);
    add_img_to_wgt(CONSTANT_IMG, rect_down_right_round, 377, 561);
    add_img_to_wgt(CONSTANT_IMG, rect_down_left_round, 23,561);
    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 23, 116);
    add_img_to_wgt(CONSTANT_IMG, rect_internal_round, 100, 109);

    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 107 + RIGHT_COLUMN_OFFSET, 29);
    add_img_to_wgt(CONSTANT_IMG, rect_top_right_round, 377 + RIGHT_COLUMN_OFFSET, 29);
    add_img_to_wgt(CONSTANT_IMG, rect_down_right_round, 377 + RIGHT_COLUMN_OFFSET, 561);
    add_img_to_wgt(CONSTANT_IMG, rect_down_left_round, 23 + RIGHT_COLUMN_OFFSET, 561);
    add_img_to_wgt(CONSTANT_IMG, rect_top_left_corner, 23 + RIGHT_COLUMN_OFFSET, 116);
    add_img_to_wgt(CONSTANT_IMG, rect_internal_round, 100 + RIGHT_COLUMN_OFFSET, 109);

}

void Widget::draw_img_vector(vector<ImageObj> img_vector_to_draw, uint16_t parent_wgt_coord_x, uint16_t parent_wgt_coord_y)
{
    for(vector<ImageObj>::size_type ii = 0; ii != img_vector_to_draw.size(); ii++)
    {
    TFT_DrawBitmap_d(img_vector_to_draw[ii].img_coord_x + parent_wgt_coord_x, \
                    img_vector_to_draw[ii].img_coord_y + parent_wgt_coord_y, \
                    img_vector_to_draw[ii].image_struct.data, \
                    img_vector_to_draw[ii].image_struct.width, \
                    img_vector_to_draw[ii].image_struct.height, \
                    MAIN_LAYER);
    }
}

void Widget::change_image_in_widget(tImage image_to_output, uint16_t img_out_coord_x, uint16_t img_out_coord_y)
{
    changeable_images.clear();
    add_img_to_wgt(CHANGEABLE_IMG, image_to_output, img_out_coord_x, img_out_coord_y);
    draw_img_vector(changeable_images, wgt_coord_x, wgt_coord_y);
}

uint16_t Widget::img_center_x(tImage img_to_center)
{
    uint16_t centered_coord_x = (wgt_width/2) - img_to_center.width/2;
    return centered_coord_x;
}

void Widget::lock_button(void)
{
    button_is_pressed = false;
    draw_img_vector(blocked_btn_images, wgt_coord_x, wgt_coord_y);
    btn_locked = true;
}

void Widget::unlock_button(void)
{
    button_is_pressed = false;
    draw_img_vector(released_btn_images, wgt_coord_x, wgt_coord_y);
    btn_locked = false;
}

void OvenDisplay::change_time_figure(Decr_Incr chng_type, Widget& figure_widget, uint32_t* digit, vector<tImage>& nmbrs_img_vect, uint8_t digit_max_value)
{
    switch (chng_type)
    {
        case DECREMENT:
        {
            if (*digit > 0)
            {
                *digit = *digit - 1;
            }
            else
            {
                *digit = digit_max_value;
            }
            break;
        }
        case INCREMENT:
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

    figure_widget.change_image_in_widget(numbers_45_background, figure_widget.img_center_x(numbers_45_background), TIME_FIGURE_Y_OFFSET);
    figure_widget.change_image_in_widget(nmbrs_img_vect[*digit], figure_widget.img_center_x(nmbrs_img_vect[*digit]), TIME_FIGURE_Y_OFFSET);
}

void OvenDisplay::replace_time_figure(Widget& figure_widget, tImage& nmbr_img)
{
    figure_widget.change_image_in_widget(numbers_45_background, figure_widget.img_center_x(numbers_45_background), TIME_FIGURE_Y_OFFSET);
    figure_widget.change_image_in_widget(nmbr_img, figure_widget.img_center_x(nmbr_img), TIME_FIGURE_Y_OFFSET);
}

void OvenDisplay::init_numbrs_img_vect(void)
{
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
}

void OvenDisplay::init_displayed_values(void)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        if (
            (i == 9) || \
            (i == 10) || \
            (i == 11) || \
            (i == 12)
            )
        {
            widgets_vector[i].change_image_in_widget(numbers_45_0, widgets_vector[i].img_center_x(numbers_45_0), TIME_FIGURE_Y_OFFSET);
        }
    }
}

void OvenDisplay::blink_clock_colon(void)
{
    if (colon_displayed)
    {
       widgets_vector[13].change_image_in_widget(img_time_colon_backgr, 0, 0);
       colon_displayed = false;
    }
    else
    {
       widgets_vector[13].change_image_in_widget(img_time_colon_char, 0, 0);
       colon_displayed = true;
    }
}

void OvenDisplay::lock_arrows_left(void)
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
            widgets_vector[i].lock_button();
        }
    }
}

void OvenDisplay::unlock_arrows_left(void)
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
}

bool OvenDisplay::bounce_filter_passed(Buttons_list pressed_button)
{
    if (pressed_button != bounce_btn_buff)
    {
        bounce_btn_buff = pressed_button;
        bounce_sample_counter = 0;
    }
    if (bounce_sample_counter >= BOUNCE_MAX_SAMPLES)
    {
        bounce_sample_counter = BOUNCE_MAX_SAMPLES;
        return true;
    }
    else
    {
        return false;
    }
}

void system_menu_layout_draw(void)
{
    TFT_FillScreen(LCD_COLOR_WHITE, MAIN_LAYER);
    //TFT_SetFont(&Font24);
    TFT_SetTextColor(LCD_COLOR_BLACK);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, SPACE_BETWEEN_LINES, {"заданная температура воздуха: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 500, SPACE_BETWEEN_LINES, {"C"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 650 - RECTANGLE_OFFSET, SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        100, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_SetTextColor(LCD_COLOR_BLUE);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 650, SPACE_BETWEEN_LINES, {"старт"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawRectangle(OFFSET_FROM_EDGE + 100 - RECTANGLE_OFFSET, 2*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        46, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 150 - RECTANGLE_OFFSET, 2*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        46, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 200 - RECTANGLE_OFFSET, 2*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        46, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 250 - RECTANGLE_OFFSET, 2*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        64, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 318 - RECTANGLE_OFFSET, 2*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        64, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 386 - RECTANGLE_OFFSET, 2*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        64, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 100, 2*SPACE_BETWEEN_LINES, {"70 80 90 100 110 120"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 650 - RECTANGLE_OFFSET, 2*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        100, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 650, 2*SPACE_BETWEEN_LINES, {"стоп"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    
    TFT_SetTextColor(LCD_COLOR_BLACK);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 3*SPACE_BETWEEN_LINES, {"заданное время нагрева: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 400, 3*SPACE_BETWEEN_LINES, {"ч"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 450, 3*SPACE_BETWEEN_LINES, {"мин"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawRectangle(OFFSET_FROM_EDGE + 100 - RECTANGLE_OFFSET, 4*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        24, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 254 - RECTANGLE_OFFSET, 4*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        24, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_SetTextColor(LCD_COLOR_BLUE);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 100, 4*SPACE_BETWEEN_LINES, {"+ 30 мин -"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_SetTextColor(LCD_COLOR_BLACK);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 5*SPACE_BETWEEN_LINES, {"статус: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 6*SPACE_BETWEEN_LINES, {"осталось времени: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 7*SPACE_BETWEEN_LINES, {"текущая температура воздуха: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 500, 7*SPACE_BETWEEN_LINES, {"C"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 8*SPACE_BETWEEN_LINES, {"текущая температура нагревателя 1: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 600, 8*SPACE_BETWEEN_LINES, {"C"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawRectangle(OFFSET_FROM_EDGE + 100 - RECTANGLE_OFFSET, 9*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        24, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 220 - RECTANGLE_OFFSET, 9*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        24, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 400 - RECTANGLE_OFFSET, 9*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        120, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 534 - RECTANGLE_OFFSET, 9*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        100, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_SetTextColor(LCD_COLOR_BLUE);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 100, 9*SPACE_BETWEEN_LINES, {"+ 10 C -"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 400, 9*SPACE_BETWEEN_LINES, {"нагрев  сброс"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_SetTextColor(LCD_COLOR_BLACK);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 10*SPACE_BETWEEN_LINES, {"текущая температура нагревателя 2: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 600, 10*SPACE_BETWEEN_LINES, {"C"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawRectangle(OFFSET_FROM_EDGE + 100 - RECTANGLE_OFFSET, 11*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        24, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 220 - RECTANGLE_OFFSET, 11*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        24, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 400 - RECTANGLE_OFFSET, 11*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        120, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 534 - RECTANGLE_OFFSET, 11*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        100, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_SetTextColor(LCD_COLOR_BLUE);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 100, 11*SPACE_BETWEEN_LINES, {"+ 10 C - "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 400, 11*SPACE_BETWEEN_LINES, {"нагрев  сброс"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0); 
    
    TFT_SetTextColor(LCD_COLOR_BLACK);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 12*SPACE_BETWEEN_LINES, {"текущее давление: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 400, 12*SPACE_BETWEEN_LINES, {"атм"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawRectangle(OFFSET_FROM_EDGE + 400 - RECTANGLE_OFFSET, 13*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        60, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 480 - RECTANGLE_OFFSET, 13*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        80, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 13*SPACE_BETWEEN_LINES, {"насос: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_SetTextColor(LCD_COLOR_BLUE);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 400, 13*SPACE_BETWEEN_LINES, {"вкл  выкл"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);

    TFT_DrawRectangle(OFFSET_FROM_EDGE + 400 - RECTANGLE_OFFSET, 14*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        60, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_DrawRectangle(OFFSET_FROM_EDGE + 480 - RECTANGLE_OFFSET, 14*SPACE_BETWEEN_LINES -  RECTANGLE_OFFSET, \
                        80, RECTANGLE_HEIGHT, ROUND_CORNER, LCD_COLOR_BLUE, LCD_COLOR_WHITE, MAIN_LAYER, \
                        RECTANGLE_THICKNESS, RECTANGLE_FILL, RECTANGLE_PUNCTIR);
    TFT_SetTextColor(LCD_COLOR_BLACK);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE, 14*SPACE_BETWEEN_LINES, {"вентиляторы: "}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
    TFT_SetTextColor(LCD_COLOR_BLUE);
    TFT_DrawText(TFT_FONT24, OFFSET_FROM_EDGE + 400, 14*SPACE_BETWEEN_LINES, {"вкл  выкл"}, 0, MAIN_LAYER, MAIN_LAYER, 0x0);
}

