#include "ltdc.h"



uint16_t X_SIZE = 800;
uint16_t Y_SIZE = 600;
LCD_DrawPropTypeDef lcdprop;
const uint8_t *ch;

void TFT_FillScreen(uint32_t color, uint8_t layer)
{
 uint32_t i;
 uint32_t n = hltdc.LayerCfg[layer].ImageHeight*hltdc.LayerCfg[layer].ImageWidth;
 for(i=0;i<n;i++)
 {
  *(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (i*4)) = color;
 }
}

void TFT_ScreenRandom(uint8_t layer)
{
	uint32_t i;
	 uint32_t n = hltdc.LayerCfg[layer].ImageHeight*hltdc.LayerCfg[layer].ImageWidth;
	 for(i=0;i<n;i++)
	 {
		uint32_t x = rand() & 0xff;
		x |= (rand() & 0xff) << 8;
		x |= (rand() & 0xff) << 16;
		x |= (rand() & 0xff) << 24;
		// x |= (rand() & 0xff) << 32;
		*(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (i*4)) = x;
	 }
}
void TFT_ScreenPicture(uint8_t layer)
{
/*
	uint32_t i;
	 uint32_t n = hltdc.LayerCfg[layer].ImageHeight*hltdc.LayerCfg[layer].ImageWidth;
	 for(i=0;i<n;i++)
	 {
		*(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (i*4))  = (uint32_t)&image_data_Image4[i];
	 }
*/
}

uint32_t TFTmiddleColor(uint32_t c, uint32_t bc){
	uint32_t color = 0xFF000000>>24;
	 color = ((((((c & 0xFF000000)>>24) + ((bc & 0xFF000000)>>24))/2)<<24) |\
			 	 	  (((((c & 0x00FF0000)>>16) + ((bc & 0x00FF0000)>>16))/2)<<16) |\
					  (((((c & 0x0000FF00)>>8 ) + ((bc & 0x0000FF00)>>8 ))/2)<<8 ) |\
					  (((((c & 0x000000FF)	 ) + ((bc & 0x000000FF)	   ))/2)));
	return color;
}
void TFT_DrawPunktir(uint16_t x, uint16_t y,uint16_t w, uint16_t h, uint8_t r,uint32_t color,uint32_t bcolor, uint8_t layer, uint16_t t,uint8_t f){

}
void TFT_DrawRectangle(uint16_t x, uint16_t y,uint16_t w, uint16_t h, uint8_t r,uint32_t color,uint32_t bcolor, uint8_t layer, uint16_t t,uint8_t f, uint8_t p){
/*
 *
 *  2       3
 * 1         4
 *
 *
 *
 * 8         5
 *  7       6
 *

 * */
	// uint8_t r1 = r;
	uint32_t color_org = color;
	for(uint8_t t1 = 0;t1<t;t1++){
		if(t1 == 0 || ((!f)&&(t1 == t-1))){
			color = TFTmiddleColor(color, bcolor);
		}else{color = color_org;}
		if(r!=0){
	//		r = r1 - t1;
			int16_t f1 = 1 - r;
			int16_t ddF_x = 1;
			int16_t ddF_y = -2 * r;
			int16_t x1 = 0;
			int16_t y1 = r;
			while (x1 < y1) {
				if (f1 >= 0) {
					y1--;
					ddF_y += 2;
					f1 += ddF_y;
				}
				x1++;
				ddF_x += 2;
				f1 += ddF_x;
				TFT_DrawPixel(x - y1+r+t1, y - x1+r+t1, color,layer);			/*1*/
				TFT_DrawPixel(x - x1+r+t1, y - y1+r+t1, color,layer);			/*2*/
				TFT_DrawPixel(x+w + x1-r-t1, y - y1+r+t1, color,layer);			/*3*/
				TFT_DrawPixel(x+w + y1-r-t1, y - x1+r+t1, color,layer);			/*4*/
				TFT_DrawPixel(x+w + y1-r-t1, y+h + x1-r-t1, color,layer);		/*5*/
				TFT_DrawPixel(x+w + x1-r-t1, y+h + y1-r-t1, color,layer);		/*6*/
				TFT_DrawPixel(x - x1+r+t1, y+h + y1-r-t1, color,layer);			/*7*/
				TFT_DrawPixel(x - y1+r+t1, y+h + x1-r-t1, color,layer); 		/*8*/
				if(t1 > 0 && t1 < t){
					TFT_DrawPixel(x - y1+r+t1, y - x1+r+t1-1, color,layer);			/*1*/
					TFT_DrawPixel(x - x1+r+t1-1, y - y1+r+t1, color,layer);			/*2*/
					TFT_DrawPixel(x+w + x1-r-t1+1, y - y1+r+t1, color,layer);			/*3*/
					TFT_DrawPixel(x+w + y1-r-t1, y - x1+r+t1-1, color,layer);			/*4*/
					TFT_DrawPixel(x+w + y1-r-t1, y+h + x1-r-t1+1, color,layer);		/*5*/
					TFT_DrawPixel(x+w + x1-r-t1+1, y+h + y1-r-t1, color,layer);		/*6*/
					TFT_DrawPixel(x - x1+r+t1-1, y+h + y1-r-t1, color,layer);			/*7*/
					TFT_DrawPixel(x - y1+r+t1, y+h + x1-r-t1+1, color,layer); 		/*8*/
				}

			}
		}
		if(p){
			uint8_t C = 11;
			for(uint8_t P = 1;P<C+1;P++){
				TFT_DrawFLatLine(x+t1+r+((x+w-t1-r) - (x+t1+r))/C*(P-1), y+t1, x+t1+r+((x+w-t1-r) - (x+t1+r))/C*P, y+t1, color, layer);
				TFT_DrawFLatLine(x+w-t1, y+t1+r, x+w-t1, y+h-t1-r, color, layer);
				TFT_DrawFLatLine(x+t1+r+((x+w-t1-r) - (x+t1+r))/C*(P-1), y+h-t1, x+t1+r+((x+w-t1-r) - (x+t1+r))/C*P, y+h-t1, color, layer);
				TFT_DrawFLatLine(x+t1, y+h-t1-r, x+t1, y+t1+r, color, layer);
				P++;
			}
		}else{
			TFT_DrawFLatLine(x+t1+r, y+t1, x+w-t1-r, y+t1, color, layer);
			TFT_DrawFLatLine(x+w-t1, y+t1+r, x+w-t1, y+h-t1-r, color, layer);
			TFT_DrawFLatLine(x+w-t1-r, y+h-t1, x+t1+r, y+h-t1, color, layer);
			TFT_DrawFLatLine(x+t1, y+h-t1-r, x+t1, y+t1+r, color, layer);
		}
	}
}


void TFT_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t color, uint8_t layer)
{
 *(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (4*(Ypos*hltdc.LayerCfg[layer].ImageWidth + Xpos))) = color;
}


void TFT_Draw8Pixel(uint16_t Xpos, uint16_t Ypos, uint8_t pixels, uint8_t layer)
{
	uint32_t color;
	for(int i=0;i<8;i++){		
		int mask =  1 << i;
		int masked_i = pixels & mask;
		int thebit = masked_i >> i;
		if(thebit){
			color = 0xFFFFFFFF;
		}
		else{
			color = 0xFF000000;
		}
		*(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (4*(Ypos*hltdc.LayerCfg[layer].ImageWidth + Xpos+8-i))) = color;
	} 
}

void TFT_FontsInit(void)
{
  lcdprop.BackColor=LCD_COLOR_BLUE;
  lcdprop.TextColor=LCD_COLOR_WHITE;
  lcdprop.pFont[0] = &Font24;
  lcdprop.pFont[1] = &Font20;
  lcdprop.pFont[2] = &Font16;
  lcdprop.pFont[3] = &Font12;
  lcdprop.pFont[4] = &Font8;
}

void TFT_SetFont(sFONT *fonts)
{
//   lcdprop.pFont=fonts;
}

void TFT_SetTextColor(uint32_t color)
{
  lcdprop.TextColor=color;
}

void TFT_SetBackColor(uint32_t color)
{
  lcdprop.BackColor=color;
}

uint16_t UTF_strlen(const char *s){
	uint16_t size = 0;
	while(*s!=0){
		if(*s>127){*s++;size++;}	
		size++;
		*s++;
	}
	return size;
}

void TFT_DrawChar(uint8_t font, uint16_t x, uint16_t y, uint16_t c,uint8_t clearTR, uint8_t layer, uint8_t backLayer,uint32_t bc)
{
	uint32_t i = 0, j = 0;
	uint16_t height, width;
	uint8_t offset;
	uint8_t *pchar;
	uint32_t line;
	if(c>3000){
		c = (c & 0x1F00)>>2 | (c & 0x3F);
	}
	if(c==1025)c=138;
	if(c==1105)c=139;
	if(c==8212)c=45;
	if(c>900){
		// if(c==1045){
		// 	ch = &lcdprop.pFont[font]->table[(c-900) * lcdprop.pFont[font]->Height * ((lcdprop.pFont[font]->Width + 7) / 8)];	
		// }
		// else{			
			ch = &lcdprop.pFont[font]->table[(c-900) * lcdprop.pFont[font]->Height * ((lcdprop.pFont[font]->Width + 7) / 8)];	
		// }
	}else{
		ch = &lcdprop.pFont[font]->table[(c) * lcdprop.pFont[font]->Height * ((lcdprop.pFont[font]->Width + 7) / 8)];	
	}
	height = lcdprop.pFont[font]->Height;
	width = lcdprop.pFont[font]->Width;
	offset = 8 *((width + 7)/8) - width ;
	for(i = 0; i < height; i++)
	{
		pchar = ((uint8_t *)ch + (width + 7)/8 * i);
		switch((width + 7)/8)
		{
		  case 1: line = pchar[0]; break;
		  case 2: line = (pchar[0]<< 8) | pchar[1]; break;
		  case 3:
		  default:
		    line = (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
		    break;
		}
		for (j = 0; j < width; j++)
		{
			if(line & (1 << (width- j + offset- 1)))
			{
				TFT_DrawPixel((x + j), y, lcdprop.TextColor,layer);
			}
			else
			{
				TFT_DrawPixel((x + j), y, bc?bc:*(uint32_t*) (hltdc.LayerCfg[backLayer].FBStartAdress + (4*(y*hltdc.LayerCfg[backLayer].ImageWidth + (x + j)))),layer);
			}
		}
		y++;
	}
	if(clearTR){TFT_Stroke(font, x,y,clearTR,1,layer,backLayer);}
}
void TFT_Stroke(uint8_t font, uint16_t x, uint16_t y, uint16_t to_clear, uint16_t len, uint8_t toLayer, uint8_t fromLayer){
	uint8_t height = lcdprop.pFont[font]->Height;
	uint8_t width = lcdprop.pFont[font]->Width;
	uint16_t clearLength = to_clear*width;
	// x=x+(width*(len-1));
	for(int j=y-height;j<y;j++){
		for(int i=x;i<x+clearLength;i++){
			uint32_t k = j*hltdc.LayerCfg[toLayer].ImageWidth + i;
			// *(uint32_t*) (hltdc.LayerCfg[MAIN_LAYER].FBStartAdress + (k*4)) = 0xFFFFFFFF;
			*(uint32_t*) (hltdc.LayerCfg[toLayer].FBStartAdress + (k*4)) = *(uint32_t*)(hltdc.LayerCfg[fromLayer].FBStartAdress + (k*4));
		}
	}
}
void TFT_DrawNumers(uint16_t x, uint16_t y, const char *s,uint8_t clearTR, uint8_t layer, uint8_t backLayer, uint32_t bc){
	uint8_t len = strlen(s);
	x = x-len*17;
	for(uint8_t i=0;i<len;i++){
		if(*(s) == '1'){LTDC_IconFromLayer(x+i*19, y, 13, 20, bc,NUM_PIC_ONE_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '2'){LTDC_IconFromLayer(x+i*19, y, 16, 20, bc,NUM_PIC_TWO_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '3'){LTDC_IconFromLayer(x+i*19, y, 16, 20, bc,NUM_PIC_THREE_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '4'){LTDC_IconFromLayer(x+i*19, y, 18, 20, bc,NUM_PIC_FOUR_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '5'){LTDC_IconFromLayer(x+i*19, y, 17, 20, bc,NUM_PIC_FIVE_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '6'){LTDC_IconFromLayer(x+i*19, y, 17, 20, bc,NUM_PIC_SIX_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '7'){LTDC_IconFromLayer(x+i*19, y, 15, 20, bc,NUM_PIC_SEVEN_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '8'){LTDC_IconFromLayer(x+i*19, y, 17, 20, bc,NUM_PIC_EIGHT_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '9'){LTDC_IconFromLayer(x+i*19, y, 17, 20, bc,NUM_PIC_NINE_START, FLASH_LAYER1, layer,0,0);}
		if(*(s) == '0'){LTDC_IconFromLayer(x+i*19, y, 17, 20, bc,NUM_PIC_ZERO_START, FLASH_LAYER1, layer,0,0);}
		*(s++);
		// TFT_DrawChar(f,x + (lcdprop.pFont[font]->Width)*i, y, *(s++),0,layer, backLayer,bc);
	}
	if(clearTR){
		
	}
}

// uint16_t UTF_strlen(const char *s){
// 	uint16_t size = 0;
// 	while(*s!=0){
// 		if(*s>127){*s++;size++;}	
// 		size++;
// 		*s++;
// 	}
// 	return size;
// }

size_t t_shortlen(const char* s){
	size_t size = 0;
	while(*s!=0){
		if(*s>127){*s++;}	
		size++;
		*s++;
	}
	return size;
}
void TFT_DrawText(uint8_t font, uint16_t x, uint16_t y, const char *s,uint8_t clearTR, uint8_t layer, uint8_t backLayer, uint32_t bc){
	uint8_t len = t_shortlen(s);
	if(clearTR){TFT_Stroke(font, x - lcdprop.pFont[font]->Width,y+lcdprop.pFont[font]->Height,clearTR,len,layer,backLayer);}
	for(uint8_t i=0;i<len;i++){
		if(*s>127){
			unsigned short c = (*s++)<<8 | (*s++);
			TFT_DrawChar(font, x + (lcdprop.pFont[font]->Width)*i, y, c,0,layer, backLayer,bc);
		}else{
			TFT_DrawChar(font, x + (lcdprop.pFont[font]->Width)*i, y, *(s++),0,layer, backLayer,bc);
		}
	}
}
void TFT_DrawTextArr(uint8_t font, uint16_t x, uint16_t y, uint16_t s[],uint8_t clearTR, uint8_t layer, uint8_t backLayer, uint32_t bc){
	uint8_t len = strlen(s);
	if(clearTR){TFT_Stroke(font, x - lcdprop.pFont[font]->Width,y+lcdprop.pFont[font]->Height,clearTR,t_shortlen(s),layer,backLayer);}
	int i=0;
	while(*s!=0){
		TFT_DrawChar(font, x + (lcdprop.pFont[font]->Width)*i, y, *(s),0,layer, backLayer,bc);
		*s++;
		i++;
	}
}
void TFT_LoadFromLayer(uint8_t layerID, uint8_t tolayer){
	 uint32_t i;
	 uint32_t n = hltdc.LayerCfg[tolayer].ImageHeight*hltdc.LayerCfg[tolayer].ImageWidth;
	 for(i=0;i<n;i++)
	 {
	  *(uint32_t*) (hltdc.LayerCfg[tolayer].FBStartAdress + (i*4)) = *(uint32_t*)(hltdc.LayerCfg[layerID].FBStartAdress + (i*4));
	 }
}
void TFT_DrawRainbow(uint8_t layer){
	uint32_t n,c;
	//w = hltdc.LayerCfg[MAIN_LAYER].ImageWidth;
	//h = hltdc.LayerCfg[MAIN_LAYER].ImageHeight;

	//	Bit    5  4  3  2  1  0
	//	Data   R  R  G  G  B  B

	//	Red	   0  0  0  0  0  0       0x00   8
	//	Red	   0  1  0  0  0  0       0x01   8
	//	Red	   1  0  0  0  0  0       0x02   16
	//	Red	   1  1  0  0  0  0       0x03   223
	//  Green  0  0  0  0  0  0       0x00
	//  Green  0  0  0  1  0  0       0x01
	//  Green  0  0  1  0  0  0       0x02
	//  Green  0  0  1  1  0  0       0x03
	//  Blue   0  0  0  0  0  0		  0x00
	//  Blue   0  0  0  0  0  1		  0x01
	//  Blue   0  0  0  0  1  0		  0x02
	//  Blue   0  0  0  0  1  1		  0x03
	uint16_t xfull = 0;
	uint8_t achannel = 255;
	for(n=0; n<480000; n++){
		// xfull++;
		if(xfull == 800){
			achannel--;
			xfull = 0;
		}
		uint32_t red, green, blue;
		blue = 0;
		green = 0;
		red = 0;
		c=0x00000000;
		for(blue = 0; blue<255; blue++){

			c |= (blue & 0xFF);
			c |= (green & 0xFF) << 8;
			c |= (red & 0xFF) << 16;
			c |= (achannel & 0xFF) << 24;

			*(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (n*4)) = c;
			n++;
		}
		blue = 0;
		green = 0;
		red = 0;
		//a = 0xFF;
		c=0x00000000;
		for(green = 0; green<255; green++){
			c |= (blue & 0xFF);
			c |= (green & 0xFF) << 8;
			c |= (red & 0xFF) << 16;
			c |= (achannel & 0xFF) << 24;

			*(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (n*4)) = c;
			n++;
		}
		blue = 0;
		green = 0;
		red = 0;
		//a = 0xFF;
		c=0x00000000;
		for(red = 0; red<255; red++){

			c |= (blue & 0xFF);
			c |= (green & 0xFF) << 8;
			c |= (red & 0xFF) << 16;
			c |= (achannel & 0xFF) << 24;

			*(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (n*4)) = c;
			n++;
		}

		n = n + (34);
		xfull = xfull+800;
		//a=a+1;
	}

}
void TFT_DrawFilledRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer){
	for(uint16_t j=y1;j<y2;j++){
		for(uint16_t i=x1;i<x2;i++){
			TFT_DrawPixel(i,j,color,layer);
		}
	}
}

void TFT_DrawFLatLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer){
	if(x1!=x2){
		if(x1>x2){uint16_t x3 = x1; x1=x2; x2=x3;}
		for(uint16_t i=x1;i<=x2;i++){
			TFT_DrawPixel(i,y1,color,layer);
		}
	}else{
		if(y1>y2){uint16_t y3 = y1; y1=y2; y2=y3;}
		for(uint16_t j=y1;j<=y2;j++){
			TFT_DrawPixel(x1,j,color,layer);
		}
	}
}
void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t layer)
{
	int steep = abs(y2-y1)>abs(x2-x1);
	if(steep)
	{
		ltdc_swap(x1,y1);
		ltdc_swap(x2,y2);
	}
	if(x1>x2)
	{
		ltdc_swap(x1,x2);
		ltdc_swap(y1,y2);
	}
	int dx,dy;
	dx=x2-x1;
	dy=abs(y2-y1);
	int err=dx/2;
	int ystep;
	if(y1<y2) ystep=1;
	else ystep=-1;
	for(;x1<=x2;x1++)
	{
		if(steep) TFT_DrawPixel(y1,x1,color,layer);
		else TFT_DrawPixel(x1,y1,color,layer);
		err-=dy;
		if(err<0)
		{
			y1 += ystep;
			err=dx;
		}
	}
}

void TFT_DrawBitmap_d(uint32_t Xpos, uint32_t Ypos, const uint8_t *bitmap, uint16_t width, uint16_t height,  uint8_t layer){
	 uint32_t y,x,p=Xpos,c=0, len=strlen(bitmap);
  /* Set the address */
  uint32_t  address = hltdc.LayerCfg[layer].FBStartAdress +  Ypos*X_SIZE*4;
  /* Bypass the bitmap header */
  for(y=0;y<height;y++){
    //for(x=1;x<width+1;x++){
		for(x=0;x<width;x++){
		c=0;
		c=0x00000000;
		c |= (bitmap[((y*width)+x)*4+3]);
		c |= (bitmap[((y*width)+x)*4+2]) << 8;
		c |= (bitmap[((y*width)+x)*4+1]) << 16;
		c |= (bitmap[((y*width)+x)*4+0]) << 24;

      *(uint32_t*) (address + (y+x+p)*4) = c;//TTZ_COL_ORAN;
      if((y+x)>=(X_SIZE*Y_SIZE)) return;
    }
	p=p+X_SIZE-1;
  }
  return;
}

void TFT_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp, uint8_t layer)
{
//	TFT_FillScreen(0xFFFF0000,0);
//	char tststr[40] = "test string";


  uint32_t index = 0, ix=0, width = 0, height = 0, bit_pixel = 0;
  uint32_t address;
  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (pbmp + 10);
  index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;
  //TFT_DrawText(font, 10, 50,index);
  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;
  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16;
  /* Read bit/pixel */
  bit_pixel = *(uint16_t *) (pbmp + 28);
  /* Set the address */
  address = hltdc.LayerCfg[layer].FBStartAdress + (((X_SIZE*Ypos) + Xpos)*4);
  /* Bypass the bitmap header */
  pbmp += (index + (width * (height - 1) * (bit_pixel/8)));
  if ((bit_pixel/8) == 4)
  {
    //TFT_FillScreen(0xFFFF0000,0);
  }
  else if ((bit_pixel/8) == 2)
  {
    //TFT_FillScreen(0xFF00FF00,0);
  }
  else
  {
	  /* Convert picture to ARGB8888 pixel format */
	  for(index=0; index < height; index++)
	  {
		  for(ix=0; ix < width; ix++)
		  {

			  uint32_t pxl = convertTo6bit(*(uint32_t *)(pbmp+(ix*3)));

			  *(uint32_t *) (address+(ix*4)) = pxl;
		  }
		  address+= (X_SIZE*4);
		  pbmp -= width*(bit_pixel/8);
	  }
  }
  bit_pixel = 0;



}

void TFT_DrawFullBitmap32(const uint32_t *bitmap, uint8_t layer){
  for(uint16_t j=0;j<800;j++){
    uint16_t i0 = 0;
    for(uint16_t i=0;i<2400;i++){
      uint8_t p0,p1,p2,p3;
      p0 = *(bitmap++);
      p1 = *(bitmap++);
      p2 = *(bitmap++);
      p3 = *(bitmap++);
      uint32_t p = (p0 ) | (p1 << 8) | (p2 << 16) | (p3 << 24);
       *(uint32_t*) (hltdc.LayerCfg[layer].FBStartAdress + (4*((j)*hltdc.LayerCfg[layer].ImageWidth + i0))) = (p | 0xFF000000) & 0xFFFFFFFF;
      i++;
      i++;
      i++;
      i0++;
    }
  }
}

uint32_t colorTo6bit(uint32_t pix){
	if(pix>192){
		return 0x22;
	}
	if(pix>128){
		return 0x12;
	}
	if(pix>64){
		return 0xA;
	}
	return 0x0;

}
uint32_t convertTo6bit(uint32_t pbmp){
	uint32_t px, R,G,B, pR,pG,pB;
	px = 0;

	R = (pbmp & 0x00ff0000) >> 16;
	G = (pbmp & 0x0000ff00) >> 8;
	B = (pbmp & 0x000000ff);

	pR = colorTo6bit(R);
	pG = colorTo6bit(G);
	pB = colorTo6bit(B);

	px |= (pB & 0xFF);
	px |= (pG & 0xFF) << 8;
	px |= (pR & 0xFF) << 16;
	px |= (0xFF & 0xFF) << 24;

	return px;
}
void TFT_fillCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color, uint8_t layer)
{
  int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TFT_DrawPixel(x0, y0 + r, color,layer);
    TFT_DrawPixel(x0, y0 - r, color,layer);
    TFT_DrawPixel(x0 + r, y0, color,layer);
    TFT_DrawPixel(x0 - r, y0, color,layer);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TFT_DrawPixel(x0 + x, y0 + y, color,layer);
        TFT_DrawPixel(x0 - x, y0 + y, color,layer);
        TFT_DrawPixel(x0 + x, y0 - y, color,layer);
        TFT_DrawPixel(x0 - x, y0 - y, color,layer);

        TFT_DrawPixel(x0 + y, y0 + x, color,layer);
        TFT_DrawPixel(x0 - y, y0 + x, color,layer);
        TFT_DrawPixel(x0 + y, y0 - x, color,layer);
        TFT_DrawPixel(x0 - y, y0 - x, color,layer);
    }
}
void TFT_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color, uint8_t layer) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TFT_DrawPixel(x0, y0 + r, color,layer);
    TFT_DrawPixel(x0, y0 - r, color,layer);
    TFT_DrawPixel(x0 + r, y0, color,layer);
    TFT_DrawPixel(x0 - r, y0, color,layer);
    TFT_DrawLine(x0 - r, y0, x0 + r, y0, color,layer);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TFT_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color,layer);
        TFT_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color,layer);

        TFT_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color,layer);
        TFT_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color,layer);
    }
}
uint32_t ColorRandom(void){
	return (uint32_t)((rand() & 0xff) | (rand() & 0xff)<<8 | (rand() & 0xff)<<16 | ( 0xff)<<24);
}

void TFT_DrawMenuItem(uint8_t font, uint8_t col, const char *s, uint8_t layer,uint32_t color,uint32_t bc){
	uint16_t x0=50+col*150, x1 = x0+100, y0 = 50, y1=150;
	if(col > 4){y0 = 200;y1=300;}
	
	TFT_DrawLine(x0, y0, x0, y1, color, layer);
	TFT_DrawLine(x0, y1, x1, y1, color, layer);
	TFT_DrawLine(x1, y1, x1, y0, color, layer);
	TFT_DrawLine(x1, y0, x0, y0, color, layer);
	TFT_DrawText(font,x0, y0, s,0,layer, 1,bc);
};

void TFT_TestFunction(void){
	TFT_DrawPixel(0, 0, 0,0);
}
void TFT_TestFunction2(void){
	uint16_t i=0;
	while(*(uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (4*(0*hltdc.LayerCfg[0].ImageWidth + i)))==0x0){
		i++;
	}
	TFT_DrawPixel(i, 0, LCD_COLOR_RANDOM,0);
	
}

HAL_StatusTypeDef RewriteMemory(SDRAM_HandleTypeDef *hsdram,uint32_t fromAddr,uint32_t toAddr){
	__IO uint32_t *pSdramAddressTo = (uint32_t *)toAddr;
	__IO uint32_t *pSdramAddressFrom = (uint32_t *)fromAddr;
  uint32_t tmp = 0U;
	/* Process Locked */
  __HAL_LOCK(hsdram);
  
  /* Check the SDRAM controller state */
  tmp = hsdram->State;
  
  if(tmp == HAL_SDRAM_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if((tmp == HAL_SDRAM_STATE_PRECHARGED) || (tmp == HAL_SDRAM_STATE_WRITE_PROTECTED))
  {
    return  HAL_ERROR; 
  }
  
  
  /* Write data to memory */
  for(uint32_t n=0;n<480000;n++)
  {
    *(__IO uint32_t *)pSdramAddressTo = *(__IO uint32_t *)pSdramAddressFrom;

    pSdramAddressTo++;            
    pSdramAddressFrom++;            
  }
  
  /* Process Unlocked */
  __HAL_UNLOCK(hsdram);    
	return HAL_OK;
}