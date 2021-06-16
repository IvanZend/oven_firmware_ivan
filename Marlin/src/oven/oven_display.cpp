#include "ltdc.h"
#include "oven_display.h"
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
#include "images/img_time_down_arrow.h"
#include "images/img_time_figure_rect.h"
#include "images/img_time_hour_char.h"
#include "images/img_time_minute_char.h"
#include "images/img_time_up_arrow.h"
#include "images/img_vacuum_pump_string.h"

Widget background_frame;
Widget thermometer_icon;
Widget heating_string;
Widget heating_timer_start_button;
Widget heating_timer_stop_button;
Widget temperature_display;
Widget time_up_arrow_1;
Widget time_up_arrow_2;
Widget time_up_arrow_3;
Widget time_up_arrow_4;
Widget time_figure_1;
Widget time_figure_2;
Widget time_figure_3;
Widget time_figure_4;
Widget time_colon_char;
Widget time_down_arrow_1;
Widget time_down_arrow_2;
Widget time_down_arrow_3;
Widget time_down_arrow_4;
Widget temperature_left_entering_background;
Widget temperature_left_keyboard_figure_1;
Widget temperature_left_keyboard_figure_2;
Widget temperature_left_keyboard_figure_3;
Widget temperature_left_keyboard_figure_4;
Widget temperature_left_keyboard_figure_5;
Widget temperature_left_keyboard_figure_6;
Widget temperature_left_keyboard_figure_7;
Widget temperature_left_keyboard_figure_8;
Widget temperature_left_keyboard_figure_9;
Widget temperature_left_keyboard_figure_0;
Widget temperature_left_keyboard_backspace;
Widget temperature_left_keyboard_recet;
Widget temperature_left_keyboard_enter_top;          // кнопка Enter имеет сложную форму, потому составлена из двух виджетов
Widget temperature_left_keyboard_enter_bottom;
Widget heating_on_off_button;

Widget pressure_sensor_icon;
Widget vacuum_pump_string;
Widget vacuum_timer_start_button;
Widget vacuum_timer_stop_button;
Widget pressure_display;
Widget time_up_arrow_5;
Widget time_up_arrow_6;
Widget time_up_arrow_7;
Widget time_up_arrow_8;
Widget time_figure_5;
Widget time_figure_6;
Widget time_figure_7;
Widget time_figure_8;
Widget time_down_arrow_5;
Widget time_down_arrow_6;
Widget time_down_arrow_7;
Widget time_down_arrow_8;
Widget temperature_right_entering_background;
Widget temperature_right_keyboard_figure_1;
Widget temperature_right_keyboard_figure_2;
Widget temperature_right_keyboard_figure_3;
Widget temperature_right_keyboard_figure_4;
Widget temperature_right_keyboard_figure_5;
Widget temperature_right_keyboard_figure_6;
Widget temperature_right_keyboard_figure_7;
Widget temperature_right_keyboard_figure_8;
Widget temperature_right_keyboard_figure_9;
Widget temperature_right_keyboard_figure_0;
Widget temperature_right_keyboard_backspace;
Widget temperature_right_keyboard_recet;
Widget temperature_right_keyboard_enter_top;          // кнопка Enter имеет сложную форму, потому составлена из двух виджетов
Widget temperature_right_keyboard_enter_bottom;
Widget vacuum_on_off_button;

Widget simplified_mode_dummy_text;
Widget service_mode_dummy_text;

void Widget::add_image_to_widget(tImage image_generated, uint16_t coord_x, uint16_t coord_y)
{
    ImageObj tmp_img_obj;
    tmp_img_obj.coord_x = coord_x;
    tmp_img_obj.coord_y = coord_y;
    tmp_img_obj.image_struct = image_generated;
    constant_images.push_back(tmp_img_obj);
}

OvenDisplay::OvenDisplay(void)
{
    display_mode = DEFAULT_DISPLAY_MODE;
    display_width = DISPLAY_WIDTH;
    display_height = DISPLAY_HEIGHT;
    draw_all_completed = false;
    //init_widgets(display_mode, widgets_vector);
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
    thermometer_icon.add_image_to_widget(img_thermometer_icon_crossed, 29, 32);
    heating_string.add_image_to_widget(img_heating_string, 120, 42);
    heating_timer_start_button.add_image_to_widget(img_heating_timer_start_button, 50,127);
    heating_timer_stop_button.add_image_to_widget(img_heating_timer_stop_button, 125, 127);
    temperature_display.add_image_to_widget(img_temperature_display, 200, 50);
    time_up_arrow_1.add_image_to_widget(img_time_up_arrow, 69, 200);
    time_up_arrow_2.add_image_to_widget(img_time_up_arrow, 143, 200);
    time_up_arrow_3.add_image_to_widget(img_time_up_arrow, 238, 200);
    time_up_arrow_4.add_image_to_widget(img_time_up_arrow, 312, 200);
    time_figure_1.add_image_to_widget(img_time_figure_rect, 49, 230);
    time_figure_2.add_image_to_widget(img_time_figure_rect, 124, 230);
    time_figure_3.add_image_to_widget(img_time_figure_rect, 218, 230);
    time_figure_4.add_image_to_widget(img_time_figure_rect, 292, 230);
    time_colon_char.add_image_to_widget(img_time_colon_char, 201, 269);
    time_down_arrow_1.add_image_to_widget(img_time_down_arrow, 69, 320);
    time_down_arrow_2.add_image_to_widget(img_time_down_arrow, 143, 320);
    time_down_arrow_3.add_image_to_widget(img_time_down_arrow, 238, 320);
    time_down_arrow_4.add_image_to_widget(img_time_down_arrow, 312, 320);
    temperature_left_entering_background.add_image_to_widget(img_temperature_entering_background, 50, 357);
    heating_on_off_button.add_image_to_widget(img_slide_button_off, 132, 495);

    widgets_vector.clear();
    switch (display_mode)
    {
    case STANDARD_USER_MODE:
    {
        widgets_vector.push_back(background_frame);
        widgets_vector.push_back(thermometer_icon);
        widgets_vector.push_back(heating_string);
        widgets_vector.push_back(heating_timer_start_button);
        widgets_vector.push_back(heating_timer_stop_button);
        widgets_vector.push_back(temperature_display);
        widgets_vector.push_back(time_up_arrow_1);
        widgets_vector.push_back(time_up_arrow_2);
        widgets_vector.push_back(time_up_arrow_3);
        widgets_vector.push_back(time_up_arrow_4);
        widgets_vector.push_back(time_figure_1);
        widgets_vector.push_back(time_figure_2);
        widgets_vector.push_back(time_figure_3);
        widgets_vector.push_back(time_figure_4);
        widgets_vector.push_back(time_colon_char);
        widgets_vector.push_back(time_down_arrow_1);
        widgets_vector.push_back(time_down_arrow_2);
        widgets_vector.push_back(time_down_arrow_3);
        widgets_vector.push_back(time_down_arrow_4);
        widgets_vector.push_back(temperature_left_entering_background);
        /*
        widgets_vector.push_back(temperature_left_keyboard_figure_1);
        widgets_vector.push_back(temperature_left_keyboard_figure_2);
        widgets_vector.push_back(temperature_left_keyboard_figure_3);
        widgets_vector.push_back(temperature_left_keyboard_figure_4);
        widgets_vector.push_back(temperature_left_keyboard_figure_5);
        widgets_vector.push_back(temperature_left_keyboard_figure_6);
        widgets_vector.push_back(temperature_left_keyboard_figure_7);
        widgets_vector.push_back(temperature_left_keyboard_figure_8);
        widgets_vector.push_back(temperature_left_keyboard_figure_9);
        widgets_vector.push_back(temperature_left_keyboard_figure_0);
        widgets_vector.push_back(temperature_left_keyboard_backspace);
        widgets_vector.push_back(temperature_left_keyboard_recet);
        widgets_vector.push_back(temperature_left_keyboard_enter_top);
        widgets_vector.push_back(temperature_left_keyboard_enter_bottom);
        */
        widgets_vector.push_back(heating_on_off_button);
        /*
        widgets_vector.push_back(pressure_sensor_icon);
        widgets_vector.push_back(vacuum_pump_string);
        widgets_vector.push_back(vacuum_timer_start_button);
        widgets_vector.push_back(vacuum_timer_stop_button);
        widgets_vector.push_back(pressure_display);
        widgets_vector.push_back(time_up_arrow_5);
        widgets_vector.push_back(time_up_arrow_6);
        widgets_vector.push_back(time_up_arrow_7);
        widgets_vector.push_back(time_up_arrow_8);
        widgets_vector.push_back(time_figure_5);
        widgets_vector.push_back(time_figure_6);
        widgets_vector.push_back(time_figure_7);
        widgets_vector.push_back(time_figure_8);
        widgets_vector.push_back(time_down_arrow_5);
        widgets_vector.push_back(time_down_arrow_6);
        widgets_vector.push_back(time_down_arrow_7);
        widgets_vector.push_back(time_down_arrow_8);
        widgets_vector.push_back(temperature_right_entering_background);
        widgets_vector.push_back(temperature_right_keyboard_figure_1);
        widgets_vector.push_back(temperature_right_keyboard_figure_2);
        widgets_vector.push_back(temperature_right_keyboard_figure_3);
        widgets_vector.push_back(temperature_right_keyboard_figure_4);
        widgets_vector.push_back(temperature_right_keyboard_figure_5);
        widgets_vector.push_back(temperature_right_keyboard_figure_6);
        widgets_vector.push_back(temperature_right_keyboard_figure_7);
        widgets_vector.push_back(temperature_right_keyboard_figure_8);
        widgets_vector.push_back(temperature_right_keyboard_figure_9);
        widgets_vector.push_back(temperature_right_keyboard_figure_0);
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
}

void OvenDisplay::draw_all_widgets(vector<Widget>& widgets_vector_to_draw)
{
    for(vector<Widget>::size_type i = 0; i != widgets_vector_to_draw.size(); i++)
    {
        for(vector<ImageObj>::size_type ii = 0; ii != widgets_vector_to_draw[i].constant_images.size(); ii++)
        {
            TFT_DrawBitmap_d(widgets_vector_to_draw[i].constant_images[ii].coord_x, \
                            widgets_vector_to_draw[i].constant_images[ii].coord_y, \
                            widgets_vector_to_draw[i].constant_images[ii].image_struct.data, \
                            widgets_vector_to_draw[i].constant_images[ii].image_struct.width, \
                            widgets_vector_to_draw[i].constant_images[ii].image_struct.height, \
                            MAIN_LAYER);
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
            add_image_to_widget(image_to_tile, (image_to_tile.width * ii), (image_to_tile.height * i));
        }
    }
}

void Widget::set_rectangle(void)
{
    for (int i = 0; i < 26; i++)                                    // верхняя большая горизонтальная линия (левая часть экрана)
    {
        add_image_to_widget(rect_horiz_line, ((i*10) + 117), 29);
    }
    for (int i = 0; i < 35; i++)                                    // нижняя горизонтальная линия (левая часть экрана)
    {
        add_image_to_widget(rect_horiz_line, ((i*10) + 33), 568);
    }
    for (int i = 0; i < 7; i++)                                     // верхняя малая горизонтальная линия (левая часть экрана)
    {
        add_image_to_widget(rect_horiz_line, ((i*10) + 33), 116);
    }

    for (int i = 0; i < 53; i++)                                    // правая вертикальная линия (левая часть экрана)
    {
        add_image_to_widget(rect_vertic_line, 383, (i*10) + 38);
    }
    for (int i = 0; i < 44; i++)                                    // левая большая вертикальная линия (левая часть экрана)
    {
        add_image_to_widget(rect_vertic_line, 23, (i*10) + 126);
    }    
    for (int i = 0; i < 8; i++)                                     // левая малая вертикальная линия (левая часть экрана)
    {
        add_image_to_widget(rect_vertic_line, 107, (i*10) + 38);
    }

    for (int i = 0; i < 26; i++)                                    // верхняя большая горизонтальная линия (правая часть экрана)
    {
        add_image_to_widget(rect_horiz_line, ((i*10) + 117 + RIGHT_COLUMN_OFFSET), 29);
    }
    for (int i = 0; i < 35; i++)                                    // нижняя горизонтальная линия (правая часть экрана)
    {
        add_image_to_widget(rect_horiz_line, ((i*10) + 33 + RIGHT_COLUMN_OFFSET), 568);
    }
    for (int i = 0; i < 7; i++)                                     // верхняя малая горизонтальная линия (правая часть экрана)
    {
        add_image_to_widget(rect_horiz_line, ((i*10) + 33 + RIGHT_COLUMN_OFFSET), 116);
    }

    for (int i = 0; i < 53; i++)                                    // правая вертикальная линия (правая часть экрана)
    {
        add_image_to_widget(rect_vertic_line, 383 + RIGHT_COLUMN_OFFSET, (i*10) + 38);
    }
    for (int i = 0; i < 44; i++)                                    // левая большая вертикальная линия (правая часть экрана)
    {
        add_image_to_widget(rect_vertic_line, 23 + RIGHT_COLUMN_OFFSET, (i*10) + 126);
    }    
    for (int i = 0; i < 8; i++)                                     // левая малая вертикальная линия (правая часть экрана)
    {
        add_image_to_widget(rect_vertic_line, 107 + RIGHT_COLUMN_OFFSET, (i*10) + 38);
    }

    add_image_to_widget(rect_top_left_corner, 107, 29);
    add_image_to_widget(rect_top_right_round, 377, 29);
    add_image_to_widget(rect_down_right_round, 377, 561);
    add_image_to_widget(rect_down_left_round, 23,561);
    add_image_to_widget(rect_top_left_corner, 23, 116);
    add_image_to_widget(rect_internal_round, 100, 109);

    add_image_to_widget(rect_top_left_corner, 107 + RIGHT_COLUMN_OFFSET, 29);
    add_image_to_widget(rect_top_right_round, 377 + RIGHT_COLUMN_OFFSET, 29);
    add_image_to_widget(rect_down_right_round, 377 + RIGHT_COLUMN_OFFSET, 561);
    add_image_to_widget(rect_down_left_round, 23 + RIGHT_COLUMN_OFFSET, 561);
    add_image_to_widget(rect_top_left_corner, 23 + RIGHT_COLUMN_OFFSET, 116);
    add_image_to_widget(rect_internal_round, 100 + RIGHT_COLUMN_OFFSET, 109);

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

