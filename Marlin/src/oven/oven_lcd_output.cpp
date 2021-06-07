#include <cstdint>
#include "ltdc.h"
#include "oven_lcd_output.h"
#include "images/rocket.h"

void oven_test_draw(void)
{
    TFT_FillScreen(0xFFFFFFFF, MAIN_LAYER);
    TFT_DrawBitmap_d(100, 100, rocket.data, rocket.width, rocket.height, MAIN_LAYER);
}

void oven_display_draw_all(void)
{
    
}