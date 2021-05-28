// #include "ltdc.h"
#include "ltdc.h"
#include "../dogm/ststusScreen_dimensions.h"
// #include "../lcd/dogm/icons/icon_termo_white.h"

// uint16_t X_SIZE = 800;
// uint16_t Y_SIZE = 600;
// LCD_DrawPropTypeDef lcdprop;
// const uint8_t *ch;
void LTDC_IconFromLayer(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t toColor, uint32_t startBit, uint8_t fromLayer, uint8_t toLayer, uint8_t rotate, int inv){
	int kX 	= 0;
	int kX_	= 1;
	int kY 	= 1;
	switch(rotate){
		case 0: break;
		case 2: kY = -1; y   = y+h; break;
		case 1: kY =  0; kX  = -1;  kX_ = 0;   x=x-w; y=y+h; break;
		case 3: kY =  0; kX  =  1;  kX_ = 0;   break;
	}
	uint32_t k=0;
	for(uint16_t j=0;j<h;j++){
		for(uint16_t i=0;i<w;i++){
			uint8_t pix = *(uint8_t*) (hltdc.LayerCfg[fromLayer].FBStartAdress + startBit + k);			
			uint8_t p1 = (uint8_t)((toColor>>16 & 0xFF)*(pix/255.f));
			uint8_t p2 = (uint8_t)((toColor>>8 & 0xFF)*(pix/255.f));
			uint8_t p3 = (uint8_t)((toColor & 0xFF)*(pix/255.f));
			float kb_ = 1-pix/255.f;

			// float k1 = (float)(p1/(toColor>>16 & 0xFF));
			// float k2 = (float)(p2/(toColor>>8 & 0xFF));
			// float k3 = (float)(p3/(toColor & 0xFF));
			uint32_t back = *(uint32_t*) (hltdc.LayerCfg[toLayer].FBStartAdress + (4*((y + (kY)*j + (kX)*i)*hltdc.LayerCfg[toLayer].ImageWidth + kX_*(x+i) + kX*(x+j))));
			uint8_t b1 = (uint8_t)((back>>16 & 0xFF)*kb_);
			uint8_t b2 = (uint8_t)((back>>8 & 0xFF)*kb_);
			uint8_t b3 = (uint8_t)((back & 0xFF)*kb_);
			// if(pix/255.f != 1){			
			// 	b1 = back>>16 & 0xFF;
			// 	b2 = back>>8 & 0xFF;
			// 	b3 = back & 0xFF;
			// }
			uint8_t c1 = 0x0;
			uint8_t c2 = 0x0;
			uint8_t c3 = 0x0;
			if(!inv){
				c1 = (uint8_t)(p1 + (255.f-p1)/255.f*b1);// ? 255 : (b1 + (255.f-pix)/255.f*pix));
				c2 = (uint8_t)(p2 + (255.f-p2)/255.f*b2);// ? 255 : (b2 + (255.f-pix)/255.f*pix));
				c3 = (uint8_t)(p3 + (255.f-p3)/255.f*b3);// ? 255 : (b3 + (255.f-pix)/255.f*pix));
			}else{
				c1 = (uint8_t)((p1+b1)>>1);// ? 255 : (b1 + (255.f-pix)/255.f*pix));
				c2 = (uint8_t)((p2+b2)>>1);// ? 255 : (b2 + (255.f-pix)/255.f*pix));
				c3 = (uint8_t)((p3+b3)>>1);// ? 255 : (b3 + (255.f-pix)/255.f*pix));
			}
			*(uint32_t*) (hltdc.LayerCfg[toLayer].FBStartAdress + (4*((y + (kY)*j + (kX)*i)*hltdc.LayerCfg[toLayer].ImageWidth + kX_*(x+i) + kX*(x+j)))) =  (uint32_t)(0xFF000000 | (c1<< 16) | (c2<< 8) | c3 );
			k++;
		}
	}
}
void LTDC_drawOptExtruder(uint16_t x,uint16_t y, uint8_t tolayer, int active){
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/90,43, /*round corner*/8,/*color*/active ? TTZ_COL_ORAN : TTZ_COL_BAGR,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/90,43, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/2 ,/*fill?*/active ? 1 : 0,/*punktir?*/0);
	LTDC_IconFromLayer(x+8, y+8, 47, 31, active ? 0xFFFFFFFF:TTZ_COL_ORAN,AXIS_EXTRUDER_SMALL_OFF_PIC_START, FLASH_LAYER1, tolayer,0,0);   
}
void LTDC_drawFilExtruder(uint16_t x,uint16_t y, uint8_t tolayer, int active){
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/46,46, /*round corner*/23,/*color*/active ? TTZ_COL_ORAN : TTZ_COL_BAGR,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/23,/*fill?*/1,/*punktir?*/0);
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/46,46, /*round corner*/23,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/2 ,/*fill?*/active ? 1 : 0,/*punktir?*/0);
	LTDC_IconFromLayer(x+9, y+12, 47, 31, active ? 0xFFFFFFFF:TTZ_COL_ORAN,AXIS_EXTRUDER_SMALL_OFF_PIC_START, FLASH_LAYER1, tolayer,0,0);   
}
void LTDC_drawSetParams(uint16_t x,uint16_t y, uint8_t tolayer, int active){
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/85,60, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/2,/*fill?*/1,/*punktir?*/0);
	TFT_DrawFLatLine(x+3, y+30, x+80, y+30, TTZ_COL_ORAN, tolayer);       
	TFT_DrawRectangle(/*x,y*/x+6,y+10, /*width, height*/14,14, /*round corner*/7,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/7,/*fill?*/1,/*punktir?*/0);
	TFT_DrawRectangle(/*x,y*/x+6,y+39, /*width, height*/14,14, /*round corner*/7,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/7,/*fill?*/1,/*punktir?*/0);
	TFT_SetTextColor(TTZ_COL_ORAN);
	TFT_DrawChar(TFT_FONT16,x+9, y+12, '3',0,tolayer, tolayer,0x0);
	TFT_DrawChar(TFT_FONT16,x+9, y+40, 'T',0,tolayer, tolayer,0x0);
	TFT_SetTextColor(LCD_COLOR_WHITE);
}
void LTDC_drawCalibDeviceRect(uint16_t x,uint16_t y, uint8_t tolayer, int active){
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/211,95, /*round corner*/0,/*color*/active?TTZ_COL_D_OR:TTZ_COL_GREY,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/49,/*fill?*/1,/*punktir?*/0);
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
	TFT_DrawFLatLine(x+104, y+9, x+104, y+87, TTZ_COL_ORAN, tolayer);    
	TFT_DrawFLatLine(x+105, y+9, x+105, y+87, TTZ_COL_ORAN, tolayer); 
	LTDC_drawSetParams(x+115,y+18,tolayer, active);
}
void LTDC_drawHomeDeviceRect(uint16_t x,uint16_t y, uint8_t tolayer, int active){
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/241,96, /*round corner*/0,/*color*/active?TTZ_COL_D_OR:TTZ_COL_GREY,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/49,/*fill?*/1,/*punktir?*/0);
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/241,96, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,tolayer,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
	TFT_DrawFLatLine(x+114, y+9, x+114, y+87, TTZ_COL_ORAN, tolayer);    
	TFT_DrawFLatLine(x+115, y+9, x+115, y+87, TTZ_COL_ORAN, tolayer);   
	LTDC_drawSetParams(x+126,y+18,tolayer, active); 
	LTDC_IconFromLayer(x+215, y+22, 21, 21, 0xFFFFFFFF,EDIT_INC_START, FLASH_LAYER1, tolayer,0,0);	
}
void LTDC_drawOptionsRectBot(uint16_t x,uint16_t y,uint32_t c,uint32_t bc,uint8_t layer){
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/90,90, /*round corner*/8,/*color*/c,/*back_color*/bc,layer,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
	TFT_DrawRectangle(/*x,y*/x,y+74, /*width, height*/90,16, /*round corner*/8,/*color*/c,/*back_color*/bc,layer,/*thikness in px*/8,/*fill?*/1,/*punktir?*/0);
	TFT_DrawFLatLine(x+7, y+82,x+82, y+82, c, layer);    
}
void LTDC_drawOptionsRectTop(uint16_t x,uint16_t y,uint32_t c,uint32_t bc,uint8_t layer){
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/90,90, /*round corner*/8,/*color*/c,/*back_color*/bc,layer,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
	TFT_DrawRectangle(/*x,y*/x,y, /*width, height*/90,16, /*round corner*/8,/*color*/c,/*back_color*/bc,layer,/*thikness in px*/8,/*fill?*/1,/*punktir?*/0);
	TFT_DrawFLatLine(x+7, y+8,x+82, y+8, c, layer);    
}
// void LTDC_drawSDmenu(uint8_t layer){
// 	TFT_DrawRectangle(/*x,y*/150,100, /*width, height*/500,400, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/layer,layer,/*thikness in px*/2,/*fill?*/1,/*punktir?*/0);

// }