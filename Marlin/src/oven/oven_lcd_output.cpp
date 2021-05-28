#include <cstdint>
#include "ltdc.h"
#include "oven_lcd_output.h"
#include "images/rocket.h"

void oven_test_draw(void)
{
    TFT_DrawBitmap(100, 100, rocket.data, 0);
};