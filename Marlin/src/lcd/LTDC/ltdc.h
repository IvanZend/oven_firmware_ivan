#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdlib.h>
// #include "main.h"
#include "mx_init.h"
#include "fonts.h"
#include "../dogm/ststusScreen_dimensions.h"

#ifdef __cplusplus
 extern "C" {
#endif 

#define MAIN_LAYER 			0
#define BUFFER_LAYER 		15
#define FLASH_LAYER1 		16
#define EXTRA_LAYER 		0


#define LAYER_HOME 			1
#define LAYER_CAL 			3
#define LAYER_OPT 			5
#define LAYER_LIGHT 		7
#define LAYER_CAM			9
#define LAYER_MAT			11
#define LAYER_AXIS			13

#define LAYER_HOME_2 		2
#define LAYER_CAL_2 		4
#define LAYER_OPT_2 		6
#define LAYER_LIGHT_2 		8
#define LAYER_CAM_2			10
#define LAYER_MAT_2			12
#define LAYER_AXIS_2		14

#define TFT_FONT24				0  	//24h 	17w
#define TFT_FONT20				1	//20h	14w
#define TFT_FONT16				2	//16h	11w
#define TFT_FONT12				3	//12h	7w
#define TFT_FONT8				4	//8h	5w

#define LCD_FRAME_BUFFER(N) ((uint32_t)(0xC0000000 + (N)*0x1D4C00))

#define LCD_COLOR_RANDOM 		((uint32_t)ColorRandom())
#define LCD_COLOR_BLUE 			((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN 		((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED 			((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN 			((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA 		((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW 		((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE 	((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN 	((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED 		((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN 	((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA 	((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW 	((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE 		((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN 	((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED 		((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN 		((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA 	((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW 	((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE 		((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY 	((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY 			((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY 		((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK 		((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN 		((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE 		((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT 	((uint32_t)0xFF000000)


#define TTZ_COL_GREY ((uint32_t)0xFF615E5F)
#define TTZ_COL_D_GR ((uint32_t)0xFF4F4C4D)
#define TTZ_COL_ORAN ((uint32_t)0xFFF58220)
#define TTZ_COL_D_OR ((uint32_t)0xFFA85E20)
#define TTZ_COL_BAGR ((uint32_t)0xFF231F20)
#define TTZ_COL_GREE ((uint32_t)0xFF00FF00)


#define swap(a,b) {int16_t t=a;a=b;b=t;}
#define convert24to32(x) (x|0xFF000000)


#define FONT_W(N)    (lcdprop.pFont[N]->Width)
#define FONT_H(N)    (lcdprop.pFont[N]->Height)

extern LTDC_HandleTypeDef hltdc;
extern UART_HandleTypeDef huart2;

// extern sFONT Font16ASCII;

typedef struct
{
	uint32_t TextColor;
	uint32_t BackColor;
	sFONT *pFont[5];// = {&Font24,&Font20,&Font16,&Font12,&Font8}
}LCD_DrawPropTypeDef;

typedef enum
{
	CENTER_MODE = 0x01, /* Center mode */
	RIGHT_MODE = 0x02, /* Right mode */
	LEFT_MODE = 0x03 /* Left mode */
}Text_AlignModeTypdef;

//void MX_LTDC_Init(void);
void TFT_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t color, uint8_t layer);

void TFT_Draw8Pixel(uint16_t Xpos, uint16_t Ypos, uint8_t pixels, uint8_t layer);
void TFT_FontsInit(void);
void TFT_SetFont(sFONT *fonts);
void TFT_SetTextColor(uint32_t color);
void TFT_SetBackColor(uint32_t color);
uint16_t UTF_strlen(const char *s);
size_t t_shortlen(const char * str);
void TFT_DrawChar(uint8_t font, uint16_t x, uint16_t y, uint16_t c,uint8_t clearTR, uint8_t layer, uint8_t backLayer, uint32_t backColor);
void TFT_DrawText(uint8_t font, uint16_t x, uint16_t y, const char *s,uint8_t clearTR, uint8_t layer, uint8_t backLayer, uint32_t backColor);
void TFT_DrawTextArr(uint8_t font, uint16_t x, uint16_t y, uint16_t s[],uint8_t clearTR, uint8_t layer, uint8_t backLayer, uint32_t backColor);
void TFT_DrawNumers(uint16_t x, uint16_t y, const char *s,uint8_t clearTR, uint8_t layer, uint8_t backLayer, uint32_t backColor);
void TFT_Stroke(uint8_t font, uint16_t x, uint16_t y, uint16_t to_clear, uint16_t len, uint8_t toLayer, uint8_t fromLayer);
uint32_t TFTmiddleColor(uint32_t c, uint32_t bc);
void TFT_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint32_t color,uint32_t bcolor, uint8_t layer, uint16_t t,uint8_t f, uint8_t p);
void TFT_DrawPunktir(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint32_t color,uint32_t bcolor, uint8_t layer, uint16_t t,uint8_t f);
void TFT_ScreenRandom(uint8_t layer);
void TFT_ScreenPicture(uint8_t layer);
void TFT_DrawLine(uint16_t x1, uint16_t y1,	uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer);
void TFT_DrawFLatLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer);
void TFT_DrawBitmap(uint32_t Xpos, uint32_t Ypos, const uint8_t *pbmp, uint8_t layer);
void TFT_DrawFullBitmap32(const uint32_t *pbmp, uint8_t layer);
void TFT_LoadFromLayer(uint8_t fromlayer, uint8_t tolayer);
uint32_t convertTo6bit(uint32_t pbmp);
uint32_t colorTo6bit(uint32_t pix);

void TFT_DrawMenuItem(uint8_t font, uint8_t col, const char *s, uint8_t layer,uint32_t color, uint32_t backColor);
uint32_t ColorRandom(void);
void TFT_TestFunction(void);
void TFT_TestFunction2(void);

void TFT_FillScreen(uint32_t color, uint8_t layer);
void TFT_DrawRainbow(uint8_t layer);

void TFT_fillCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color, uint8_t layer);
void TFT_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color, uint8_t layer);
void TFT_DrawFilledRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer);

void LTDC_IconFromLayer(uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint32_t toColor, uint32_t startBit, uint8_t fromLayer, uint8_t toLayer, uint8_t rotate, int inv);
void LTDC_drawSetParams(uint16_t x,uint16_t y, uint8_t tolayer, int active);
void LTDC_drawHomeDeviceRect(uint16_t x,uint16_t y, uint8_t tolayer, int active);
void LTDC_drawCalibDeviceRect(uint16_t x,uint16_t y, uint8_t tolayer, int active);
void LTDC_drawOptionsRectBot(uint16_t x,uint16_t y,uint32_t c,uint32_t bc,uint8_t layer);
void LTDC_drawOptionsRectTop(uint16_t x,uint16_t y,uint32_t c,uint32_t bc,uint8_t layer);
void LTDC_drawOptExtruder(uint16_t x,uint16_t y, uint8_t tolayer, int active);
void LTDC_drawFilExtruder(uint16_t x,uint16_t y, uint8_t tolayer, int active);

HAL_StatusTypeDef RewriteMemory(SDRAM_HandleTypeDef *hsdram,uint32_t fromAddr,uint32_t toAddr);

#ifdef __cplusplus
}
#endif