#ifndef H_OVEN_DISPLAY
#define H_OVEN_DISPLAY

#include <cstdint>
#include <string>
#include <vector>

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

typedef struct 
{
const uint8_t *data;
uint16_t width;
uint16_t height;
uint8_t dataSize;
} tImage;

enum Display_mode
{
    STANDARD_USER_MODE,
    SIMPLIFIED_USER_MODE,
    SERVICE_MODE
};

typedef struct 
{
    uint16_t coord_x, coord_y;      // координаты изображения внутри виджета
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
 * Может содержать изображения, рамки, текст (символы или строки). 
*/
class Widget
{
    public:
    bool widget_is_button;
    uint16_t coord_x, coord_y, width, height;
    vector<ImageObj> constant_images;
    vector<ImageObj> pressed_button_images;
    //vector<ImageObj> constant_rectangles;
    //vector<ImageObj> pressed_button_rectangles;
    //vector<TextObj>  constant_text_strings;
    //vector<TextObj>  pressed_button_text_stringsr;
    //vector<Widget*>  related_oblects;           // Нужно для обработки кнопок сложной формы, состоящих из нескольких виджетов.
    void add_image_to_vector(tImage image_generated, uint16_t coord_x, uint16_t coord_y, vector<ImageObj>& images_vector);
};

class OvenDisplay 
{
    public:
    OvenDisplay(void);          // конструктор
    Display_mode display_mode;
    bool draw_all_completed;    // Флаг, что весь дисплей был отрисован. Переключаем при запуске и при смене режима (DEFAULT_DISPLAY_MODE)
    vector<Widget> widgets_vector;
    void test_draw(void);       // тестовая картинка, выводим изображение ракеты
    void init_widgets(Display_mode current_mode, vector<Widget>& widgets_vector_to_init);
    void draw_all_widgets(vector<Widget>& widgets_vector_to_draw);        // отрисовываем весь дисплей
};

void system_menu_layout_draw(void);

#endif