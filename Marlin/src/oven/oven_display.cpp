#include "ltdc.h"
#include "oven_display.h"
#include "oven_device.h"

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
#include "images/img_temperature_entering_background.h"
#include "images/img_thermometer_icon_crossed.h"
#include "images/img_thermometer_icon_enabled.h"
#include "images/img_time_colon_char.h"
#include "images/img_time_down_arrow_pressed.h"
#include "images/img_time_down_arrow_released.h"
#include "images/img_time_figure_rect.h"
#include "images/img_time_hour_char.h"
#include "images/img_time_minute_char.h"
#include "images/img_time_up_arrow_pressed.h"
#include "images/img_time_up_arrow_released.h"
#include "images/img_vacuum_pump_string.h"
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

Widget background_frame = {NO_BUTTON, 0, 0};
Widget thermometer_icon = {NO_BUTTON, 29, 32};
Widget heating_string = {NO_BUTTON, 120, 42};
Widget heating_timer_start_stop_btn = {HEATING_TIMER_START_STOP, 50, 127, 0, 0};
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

Widget simplified_mode_dummy_text = {NO_BUTTON, 0, 0, 0, 0};
Widget service_mode_dummy_text = {NO_BUTTON, 0, 0, 0, 0};

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
    }
}

OvenDisplay::OvenDisplay(void)
{
    display_mode = DEFAULT_DISPLAY_MODE;
    display_width = DISPLAY_WIDTH;
    display_height = DISPLAY_HEIGHT;
    draw_all_completed = false;
    previous_button = NO_BUTTON;
    init_widgets();
}

void OvenDisplay::test_draw(void)
{
    TFT_DrawBitmap_d(100, 100, rocket.data, rocket.width, rocket.height, MAIN_LAYER);
}

void OvenDisplay::init_widgets(void)
{
    background_frame.tile_area(backgr_tile, display_width, display_height);
    background_frame.set_rectangle();
    thermometer_icon.add_img_to_wgt(CONSTANT_IMG, img_thermometer_icon_crossed, 0, 0);
    heating_string.add_img_to_wgt(CONSTANT_IMG, img_heating_string, 0, 0);
    heating_timer_start_stop_btn.add_img_to_wgt(BTN_RELEASED_IMG, img_heating_timer_start_button, 0, 0);
    temperature_display.add_img_to_wgt(CONSTANT_IMG, img_temperature_display, 0, 0);
    left_time_up_arrow_1.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_1.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_up_arrow_2.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_2.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_up_arrow_3.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_3.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_up_arrow_4.add_img_to_wgt(BTN_RELEASED_IMG, img_time_up_arrow_released, 0, 0);
    left_time_up_arrow_4.add_img_to_wgt(BTN_PRESSED_IMG, img_time_up_arrow_pressed, 0, 0);
    left_time_figure_1.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_2.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_3.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_figure_4.add_img_to_wgt(CONSTANT_IMG, img_time_figure_rect, 0, 0);
    left_time_colon_char.add_img_to_wgt(CONSTANT_IMG, img_time_colon_char, 0, 0);
    left_time_down_arrow_1.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_1.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    left_time_down_arrow_2.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_2.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    left_time_down_arrow_3.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_3.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
    left_time_down_arrow_4.add_img_to_wgt(BTN_RELEASED_IMG, img_time_down_arrow_released, 0, 0);
    left_time_down_arrow_4.add_img_to_wgt(BTN_PRESSED_IMG, img_time_down_arrow_pressed, 0, 0);
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
        widgets_vector.push_back(background_frame);
        widgets_vector.push_back(thermometer_icon);
        widgets_vector.push_back(heating_string);
        widgets_vector.push_back(heating_timer_start_stop_btn);
        widgets_vector.push_back(temperature_display);
        widgets_vector.push_back(left_time_up_arrow_1);
        widgets_vector.push_back(left_time_up_arrow_2);
        widgets_vector.push_back(left_time_up_arrow_3);
        widgets_vector.push_back(left_time_up_arrow_4);
        widgets_vector.push_back(left_time_figure_1);
        widgets_vector.push_back(left_time_figure_2);
        widgets_vector.push_back(left_time_figure_3);
        widgets_vector.push_back(left_time_figure_4);
        widgets_vector.push_back(left_time_colon_char);
        widgets_vector.push_back(left_time_down_arrow_1);
        widgets_vector.push_back(left_time_down_arrow_2);
        widgets_vector.push_back(left_time_down_arrow_3);
        widgets_vector.push_back(left_time_down_arrow_4);
        widgets_vector.push_back(left_temperature_entering_background);
        widgets_vector.push_back(temperature_left_keyboard_1);
        widgets_vector.push_back(temperature_left_keyboard_2);
        widgets_vector.push_back(temperature_left_keyboard_3);
        widgets_vector.push_back(temperature_left_keyboard_4);
        widgets_vector.push_back(temperature_left_keyboard_5);
        widgets_vector.push_back(temperature_left_keyboard_6);
        widgets_vector.push_back(temperature_left_keyboard_7);
        widgets_vector.push_back(temperature_left_keyboard_8);
        widgets_vector.push_back(temperature_left_keyboard_9);
        widgets_vector.push_back(temperature_left_keyboard_0);
        widgets_vector.push_back(temperature_left_keyboard_backspace);
        widgets_vector.push_back(temperature_left_keyboard_cancel);
        widgets_vector.push_back(temperature_left_keyboard_enter_top);
        widgets_vector.push_back(temperature_left_keyboard_enter_bottom);
        widgets_vector.push_back(heating_on_off_button);
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
        widgets_vector.push_back(simplified_mode_dummy_text);
        break;
    }
    case SERVICE_MODE:
    {
        widgets_vector.push_back(service_mode_dummy_text);
        break;
    }
    }

    init_widgets_size();
    init_img_changed_flag();
    init_buttons_state();
}

void OvenDisplay::draw_all_widgets(vector<Widget>& widgets_vector_to_draw)
{
    for(vector<Widget>::size_type i = 0; i != widgets_vector_to_draw.size(); i++)
    {
        for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector_to_draw[i].constant_images.size(); ii++)
        {
            TFT_DrawBitmap_d(widgets_vector_to_draw[i].constant_images[ii].img_coord_x + widgets_vector_to_draw[i].wgt_coord_x, \
                            widgets_vector_to_draw[i].constant_images[ii].img_coord_y + widgets_vector_to_draw[i].wgt_coord_y, \
                            widgets_vector_to_draw[i].constant_images[ii].image_struct.data, \
                            widgets_vector_to_draw[i].constant_images[ii].image_struct.width, \
                            widgets_vector_to_draw[i].constant_images[ii].image_struct.height, \
                            MAIN_LAYER);
        }
        for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector_to_draw[i].changeable_images.size(); ii++)
        {
            TFT_DrawBitmap_d(widgets_vector_to_draw[i].changeable_images[ii].img_coord_x + widgets_vector_to_draw[i].wgt_coord_x, \
                            widgets_vector_to_draw[i].changeable_images[ii].img_coord_y + widgets_vector_to_draw[i].wgt_coord_y, \
                            widgets_vector_to_draw[i].changeable_images[ii].image_struct.data, \
                            widgets_vector_to_draw[i].changeable_images[ii].image_struct.width, \
                            widgets_vector_to_draw[i].changeable_images[ii].image_struct.height, \
                            MAIN_LAYER);
        }
        if(widgets_vector_to_draw[i].button_name != NO_BUTTON)          // отрисовывам кнопки в ненажатом состоянии
        {
            for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector_to_draw[i].released_btn_images.size(); ii++)
            {
                TFT_DrawBitmap_d(widgets_vector_to_draw[i].released_btn_images[ii].img_coord_x + widgets_vector_to_draw[i].wgt_coord_x, \
                                widgets_vector_to_draw[i].released_btn_images[ii].img_coord_y + widgets_vector_to_draw[i].wgt_coord_y, \
                                widgets_vector_to_draw[i].released_btn_images[ii].image_struct.data, \
                                widgets_vector_to_draw[i].released_btn_images[ii].image_struct.width, \
                                widgets_vector_to_draw[i].released_btn_images[ii].image_struct.height, \
                                MAIN_LAYER);
            }
        }
    }
}

void OvenDisplay::update_all_widgets(vector<Widget>& widgets_vector_to_update)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector_to_update.size(); i++)
    {
        if (widgets_vector_to_update[i].wgt_img_changed)
        {
            widgets_vector_to_update[i].wgt_img_changed = false;
            for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector_to_update[i].changeable_images.size(); ii++)
            {
            TFT_DrawBitmap_d(widgets_vector_to_update[i].changeable_images[ii].img_coord_x + widgets_vector_to_update[i].wgt_coord_x, \
                            widgets_vector_to_update[i].changeable_images[ii].img_coord_y + widgets_vector_to_update[i].wgt_coord_y, \
                            widgets_vector_to_update[i].changeable_images[ii].image_struct.data, \
                            widgets_vector_to_update[i].changeable_images[ii].image_struct.width, \
                            widgets_vector_to_update[i].changeable_images[ii].image_struct.height, \
                            MAIN_LAYER);
            }
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
        }
    }
}

void OvenDisplay::init_img_changed_flag(void)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        widgets_vector[i].wgt_img_changed = false;
    }
}

void OvenDisplay::init_buttons_state(void)
{
    for (vector<Widget>::size_type i = 0; i != widgets_vector.size(); i++)
    {
        widgets_vector[i].button_is_pressed = false;
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

        break;
    }
    case LEFT_UP_ARROW_1:
    {

        break;
    }
    case LEFT_UP_ARROW_2:
    {

        break;
    }
    case LEFT_UP_ARROW_3:
    {

        break;
    }
    case LEFT_UP_ARROW_4:
    {

        break;
    }
    case LEFT_DOWN_ARROW_1:
    {

        break;
    }
    case LEFT_DOWN_ARROW_2:
    {

        break;
    }
    case LEFT_DOWN_ARROW_3:
    {

        break;
    }
    case LEFT_DOWN_ARROW_4:
    {

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
        break;
    }
    case LEFT_KEYBOARD_ENTER_BOTTOM:
    {
        enter_related_event();
        break;
    }
    case HEATING_ON_OFF:
    {
        if(heater_600.device_enabled)
        {
            heater_600.device_enabled = false;
            heating_on_off_button.change_image_in_widget(img_slide_button_off, 0, 0);
        }
        else
        {
            heater_600.device_enabled = true;
            heating_on_off_button.change_image_in_widget(img_slide_button_on, 0, 0);
        }
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
            if (pressed_button != NO_BUTTON)                                    // если нажата какая-либо кнопка
            {
                if (pressed_button == widgets_vector[i].button_name)            // если нажата данная кнопка
                {
                    widgets_vector[i].button_is_pressed = true;
                    if (widgets_vector[i].pressed_btn_images.size() != 0)
                    {
                        for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector[i].pressed_btn_images.size(); ii++)
                        {
                        TFT_DrawBitmap_d(widgets_vector[i].pressed_btn_images[ii].img_coord_x + widgets_vector[i].wgt_coord_x, \
                                        widgets_vector[i].pressed_btn_images[ii].img_coord_y + widgets_vector[i].wgt_coord_y, \
                                        widgets_vector[i].pressed_btn_images[ii].image_struct.data, \
                                        widgets_vector[i].pressed_btn_images[ii].image_struct.width, \
                                        widgets_vector[i].pressed_btn_images[ii].image_struct.height, \
                                        MAIN_LAYER);
                        }
                    }
                    if (widgets_vector[i].changeable_images.size() != 0)
                        {
                        for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector[i].changeable_images.size(); ii++)
                        {
                        TFT_DrawBitmap_d(widgets_vector[i].changeable_images[ii].img_coord_x + widgets_vector[i].wgt_coord_x, \
                                        widgets_vector[i].changeable_images[ii].img_coord_y + widgets_vector[i].wgt_coord_y, \
                                        widgets_vector[i].changeable_images[ii].image_struct.data, \
                                        widgets_vector[i].changeable_images[ii].image_struct.width, \
                                        widgets_vector[i].changeable_images[ii].image_struct.height, \
                                        MAIN_LAYER);
                        }
                    }
                }
                else        // если не нажата данная кнопка
                {
                    if (widgets_vector[i].button_is_pressed)        // если данная кнопка была нажата ранее
                    {
                        widgets_vector[i].button_is_pressed = false;
                        for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector[i].released_btn_images.size(); ii++)
                        {
                        TFT_DrawBitmap_d(widgets_vector[i].released_btn_images[ii].img_coord_x + widgets_vector[i].wgt_coord_x, \
                                        widgets_vector[i].released_btn_images[ii].img_coord_y + widgets_vector[i].wgt_coord_y, \
                                        widgets_vector[i].released_btn_images[ii].image_struct.data, \
                                        widgets_vector[i].released_btn_images[ii].image_struct.width, \
                                        widgets_vector[i].released_btn_images[ii].image_struct.height, \
                                        MAIN_LAYER);
                        }
                    }
                }
            }
            else            // если не нажата никакая кнопка
            {
                if (widgets_vector[i].button_is_pressed)            // если данная кнопка была нажата ранее
                {
                    widgets_vector[i].button_is_pressed = false;
                    for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector[i].released_btn_images.size(); ii++)
                    {
                    TFT_DrawBitmap_d(widgets_vector[i].released_btn_images[ii].img_coord_x + widgets_vector[i].wgt_coord_x, \
                                    widgets_vector[i].released_btn_images[ii].img_coord_y + widgets_vector[i].wgt_coord_y, \
                                    widgets_vector[i].released_btn_images[ii].image_struct.data, \
                                    widgets_vector[i].released_btn_images[ii].image_struct.width, \
                                    widgets_vector[i].released_btn_images[ii].image_struct.height, \
                                    MAIN_LAYER);
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
            for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector[i].pressed_btn_images.size(); ii++)
            {
            TFT_DrawBitmap_d(widgets_vector[i].pressed_btn_images[ii].img_coord_x + widgets_vector[i].wgt_coord_x, \
                            widgets_vector[i].pressed_btn_images[ii].img_coord_y + widgets_vector[i].wgt_coord_y, \
                            widgets_vector[i].pressed_btn_images[ii].image_struct.data, \
                            widgets_vector[i].pressed_btn_images[ii].image_struct.width, \
                            widgets_vector[i].pressed_btn_images[ii].image_struct.height, \
                            MAIN_LAYER);
            }
         }
     }
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

void Widget::change_image_in_widget(tImage image_to_add, uint16_t img_out_coord_x, uint16_t img_out_coord_y)
{
    changeable_images.clear();
    add_img_to_wgt(CHANGEABLE_IMG, image_to_add, img_out_coord_x, img_out_coord_y);
}

/*
uint16_t Widget::img_center_x(tImage img_to_center)
{
    uint16_t centered_coord_x = (wgt_coord_x + wgt_width/2) - img_to_center.width/2;
    return centered_coord_x;
}
*/

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

