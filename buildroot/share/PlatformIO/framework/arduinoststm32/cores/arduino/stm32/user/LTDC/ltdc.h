#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include "fonts.h"

#define LCD_COLOR_BLUE ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT ((uint32_t)0xFF000000)
#define swap(a,b) {int16_t t=a;a=b;b=t;}
#define convert24to32(x) (x|0xFF000000)

extern LTDC_HandleTypeDef hltdc;


typedef struct
{
	uint32_t TextColor;
	uint32_t BackColor;
	sFONT *pFont;
}LCD_DrawPropTypeDef;

typedef enum
{
	CENTER_MODE = 0x01, /* Center mode */
	RIGHT_MODE = 0x02, /* Right mode */
	LEFT_MODE = 0x03 /* Left mode */
}Text_AlignModeTypdef;

//void MX_LTDC_Init(void);
void TFT_FillScreen(uint32_t color);
void TFT_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t color);
void TFT_FontsInit(void);
void TFT_SetFont(sFONT *fonts);
void TFT_SetTextColor(uint32_t color);
void TFT_SetBackColor(uint32_t color);
void TFT_DrawChar(uint16_t x, uint16_t y, const uint8_t c);
// void TFT_DrawText(uint16_t x, uint16_t y, char (*s)[]);
void TFT_ScreenRandom(void);
void TFT_DrawRainbow(void);
void TFT_ScreenPicture(void);
void TFT_DrawLine(uint16_t x1, uint16_t y1,	uint16_t x2, uint16_t y2, uint32_t color);
void TFT_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp);
uint32_t convertTo6bit(uint32_t pbmp);
uint32_t colorTo6bit(uint32_t pix);
