/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * queue.cpp - The G-code command queue
 */

#include "render.h"
Render render;

#include "gcode.h"

#include "../lcd/ultralcd.h"
#include "../sd/cardreader.h"
#include "../module/planner.h"
#include "../module/temperature.h"
#include "../MarlinCore.h"
#include "net.h"
#include "LTDC.h"

#if ENABLED(PRINTER_EVENT_LEDS)
  #include "../feature/leds/printer_event_leds.h"
#endif

#if ENABLED(BINARY_FILE_TRANSFER)
  #include "../feature/binary_protocol.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif


extern char str1[100];
/**
 * GCode line number handling. Hosts may opt to include line numbers when
 * sending commands to Marlin, and lines will be checked for sequentiality.
 * M110 N<int> sets the current line number.
 */
long rnd_gcode_N, Render::last_N;

/**
 * GCode Command Queue
 * A simple ring buffer of BUFSIZE command strings.
 *
 * Commands are copied into this buffer by the command injectors
 * (immediate, serial, sd card) and they are processed sequentially by
 * the main loop. The gcode.process_next_command method parses the next
 * command and hands off execution to individual handler functions.
 */
uint8_t Render::length = 0,  // Count of commands in the queue
        Render::index_r = 0, // Ring buffer read position
        Render::index_w = 0; // Ring buffer write position

char Render::command_buffer[BUFSIZE][MAX_CMD_SIZE];

/*
 * The port that the command was received on
 */
#if NUM_SERIAL > 1
  int16_t Render::port[BUFSIZE];
#endif

/**
 * Serial command injection
 */

// Number of characters read in the current line of serial input
static int serial_count[NUM_SERIAL] = { 0 };

bool rnd_send_ok[BUFSIZE];

/**
 * Next Injected Command pointer. nullptr if no commands are being injected.
 * Used by Marlin internally to ensure that commands initiated from within
 * are enqueued ahead of any pending serial or sd card commands.
 */
static PGM_P injected_commands_P = nullptr;

/**
 * Clear the Marlin command queue
 */
void Render::clear() {
  index_r = index_w = length = 0;
}


/**
 * Copy a command from RAM into the main command buffer.
 * Return true if the command was successfully added.
 * Return false for a full buffer, or if the 'command' is a comment.
 */
bool Render::_enqueue(const char* cmd) {
  if (*cmd == ';' || length >= BUFSIZE) return false;
  // strcpy(command_buffer[index_w], cmd);
  
  return true;
}

#define ISEOL(C) ((C) == '\n' || (C) == '\r')

/**
 * Enqueue with Serial Echo
 * Return true if the command was consumed
 */
bool Render::enqueue_one(const char* cmd) {

  //SERIAL_ECHOPGM("enqueue_one(\"");
  //SERIAL_ECHO(cmd);
  //SERIAL_ECHOPGM("\") \n");

  if (*cmd == 0 || ISEOL(*cmd)) return true;

  if (_enqueue(cmd)) {
    SERIAL_ECHO_MSG(STR_ENQUEUEING, cmd, "\"");
    return true;
  }
  return false;
}


/**
 * Enqueue one or many commands to run from program memory.
 * Do not inject a comment or use leading spaces!
 * Aborts the current queue, if any.
 * Note: process_injected_command() will be called to drain any commands afterwards
 */
void Render::inject_P(PGM_P const pgcode) { injected_commands_P = pgcode; }

/**
 * Enqueue and return only when commands are actually enqueued.
 * Never call this from a G-code handler!
 */
void Render::enqueue_one_now(const char* cmd) { while (!enqueue_one(cmd)) idle(); }

/**
 * Attempt to enqueue a single G-code command
 * and return 'true' if successful.
 */
bool Render::process_string(PGM_P const pgcode) {
  size_t i = 0;
  PGM_P p = pgcode;
  char c;
  while ((c = pgm_read_byte(&p[i])) && c != '\n') i++;
  char cmd[i + 1];
  memcpy_P(cmd, p, i);
  cmd[i] = '\0';
  return _enqueue(cmd);
}

/**
 * Enqueue from program memory and return only when commands are actually enqueued
 * Never call this from a G-code handler!
 */
void Render::enqueue_now_P(PGM_P const pgcode) {
  size_t i = 0;
  PGM_P p = pgcode;
  for (;;) {
    char c;
    while ((c = pgm_read_byte(&p[i])) && c != '\n') i++;
    char cmd[i + 1];
    memcpy_P(cmd, p, i);
    cmd[i] = '\0';
    enqueue_one_now(cmd);
    if (!c) break;
    p += i + 1;
  }
}

/**
 * Handle a line being completed. For an empty line
 * keep sensor readings going and watchdog alive.
 */
bool Render::process_line_done(uint8_t &sis, char (&buff)[MAX_CMD_SIZE], int &ind) {
  str1[ind] = 0;
  buff[ind] = 0;
  if (ind) { ind = 0; return false; }
  thermalManager.manage_heater();
  return true;
}


void Render::render_char(const char c, uint8_t &sis, char (&buff)[MAX_CMD_SIZE], int &ind, int &nZ, int &nY, int &nX, int &num) {

  buff[ind++] = c;
  TestFuncForReference((int)c);
  // if(c == 'Z')nZ = 2;
  // if(c == 'Y')nY = 2;
  // if(c == 'X')nX = 2;

}
/*---------------------------------------------------------*/
int Render::char_check_num(char c){
  if((c>= 48 && c<=57) || c==46){return 1;}
  return 0;
}
/*---------------------------------------------------------*/
void Render::getval_char(char * buf, int &Xval, int &Yval, int &Zval){
  uint8_t b = 0;
  char val[6] = {0,0,0,0,0,0};
  int nextZ = 0, nextY = 0, nextX = 0, i = 0;
  double d = 0;
  while(buf[b++]!=0){
    if(buf[b] == 'Z'){nextZ = 2; continue;}
    if(buf[b] == 'Y'){nextY = 2; continue;}
    if(buf[b] == 'X'){nextX = 2; continue;}
    if(char_check_num(buf[b]) && (nextZ||nextY||nextX)){
      val[i++] = buf[b];
    }else if(nextZ||nextY||nextX){
      d = atof(val);
      if(nextX)Xval = (int)d;
      if(nextY)Yval = (int)d;
      if(nextZ)Zval = (int)d;
      TestFuncForReference((int)d);
      nextZ = 0; nextY = 0; nextX = 0; i = 0;
    }
  }
}
/*---------------------------------------------------------*/
int Render::check_height_temp(){

  return 0;
}
/*---------------------------------------------------------*/
int Render::char_get_numbers(uint32_t pos){
  char val[6] = {0,0,0,0,0,0};
  int i = 0;
  char c = get_char(pos++);
  while(char_check_num(c)){
    val[i++] = c;
    c = get_char(pos++);
  }
  float d = 10 * atof(val);
  return (int)d;
}
/*---------------------------------------------------------*/
int Render::get_min_x(int &Xmin, int &Xmax){
  int val = 0;
  TCHAR buf[MAX_CMD_SIZE];
  f_rewind(&FileToRender);
  while(f_eof(&FileToRender) == 0){
    f_gets(buf, sizeof(buf), &FileToRender);
    if(buf[0] != ';'){
      for(uint32_t pos = 0;pos<card.shortlen(buf);pos++){
        if(buf[pos] == 'X'){
          char read_data[6] = {(char)buf[pos+1],(char)buf[pos+2],(char)buf[pos+3],(char)buf[pos+4],(char)buf[pos+5],(char)buf[pos+6]};
          float d = 10 * atof(read_data);
          val = (int)d;
          if(val != 0 && val<Xmin)Xmin = val;
          if(val != 0 && val>Xmax)Xmax = val;
        }
      }
    }
  }

  return 0;
}
/*---------------------------------------------------------*/
int Render::get_max_height(){
  uint32_t pos = 0;
  char c = get_char(FileToRender.obj.objsize);
  while(c != 'Z'){
    c = get_char(FileToRender.obj.objsize-pos++);
  }
  int val = char_get_numbers(FileToRender.obj.objsize-pos+2);
  return val;
}
/*---------------------------------------------------------*/
int Render::find_gcode_char(PGM_P const pgcode, int len, char g) {
  while(len>=0){
    if(pgcode[len] == g)return len;
    len--;
  }
  return 0;
}
/*---------------------------------------------------------*/
int Render::get_val_after(PGM_P const buf, uint32_t pos){
  char val[6] = {0,0,0,0,0,0};
  int i = 0;
  while(char_check_num(buf[pos]))val[i++] = buf[pos++];
  float d = 10*atof(val);
  return (int)d;
}
/*---------------------------------------------------------*/
void Render::render_line(int Xmin, int Xmax, int Z, int layer){
      // LTDC_IconFromLayer(GEAR_PIC_X, GEAR_PIC_Y, GEAR_PIC_WIDTH, GEAR_PIC_HEIGHT,0xFFFFFFFF, GEAR_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
  TFT_DrawFLatLine(GEAR_PIC_X+Xmin,332-layer,GEAR_PIC_X+Xmax,332-layer,LCD_COLOR_RED,LAYER_HOME);
}
char Render::get_char(uint32_t pos) {
/*---------------------------------------------------------*/
  uint8_t read_data[1];
  unsigned int ByteRead = 0;
  f_lseek(&FileToRender, pos);
  f_read(&FileToRender, read_data, 1, &ByteRead);
  return (char)read_data[0];
}
/*---------------------------------------------------------*/
void Render::render_clone(){
  for(int y=82;y<82+251;y++){
    for(int x=89;x<89+294;x++){
      *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME_2].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME_2].ImageWidth + x))) = *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME].ImageWidth + x)));
    }
  }
}
/*---------------------------------------------------------*/
int Render::get_gcode_height() {

  // FileToRender

  
  /*****************************************************************/
	FRESULT res;
	uint8_t readData[1];
	unsigned int ByteRead = 0;


  uint32_t read_pos = 0;
  res = f_lseek (&FileToRender, read_pos);	
  if(res != FR_OK)return -1;

  res = f_read(&FileToRender, readData, 1, &ByteRead);
  if(res != FR_OK)return -1;


  /*****************************************************************/
  int pos = 0, Xmax = 0, Xmin = 2500, Ymax = 0, Ymin = 2500, Zmax = 0, Zmin = 2500, nextZ = 0, nextY = 0, nextX = 0, num = 0, i = 0;

  int Xval = 0, Yval = 0, Zval = 0;

  TFT_DrawRectangle(/*x,y*/89,82, /*width, height*/294,251, /*round corner*/8,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_HOME,/*thikness in px*/126,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/89,82, /*width, height*/294,251, /*round corner*/8,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_HOME_2,/*thikness in px*/126,/*fill?*/1,/*punktir?*/0);
    
  // double d = 0;
  char txData[100];
  sprintf(txData,"echo: Render started, time: %d  \r\n",HAL_GetTick());
  HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), 10);

  int height = get_max_height();// 10x height
  int X1 = 2500, X2 = 0;
  get_min_x(X1, X2);
  float hk = 250/((float)height); //GEAR_PIC_HEIGHT = 211
  float xk = 300/((float)X2-(float)X1);

  float k = hk>xk?xk:hk;

  /*int layers = 0;*/
  // TestFuncForReference((int)leftX);

  sprintf(txData,"echo: Size calculated, time: %d  \r\n",HAL_GetTick());
  HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), 10);
  bool card_eof = (read_pos >= FileToRender.obj.objsize);
  // char buf[MAX_CMD_SIZE];
  // char val[6] = {0,0,0,0,0,0};
  // bool is_comment = false;
  int val = 0;
  int Xprev = 0;
  f_rewind(&FileToRender);
  TCHAR buf[MAX_CMD_SIZE];
  while(f_eof(&FileToRender) == 0){
    f_gets(buf, sizeof(buf), &FileToRender);
    if(buf[0] != ';'){
      uint32_t cur_pos = FileToRender.fptr;
      for(uint32_t pos = 0;pos<card.shortlen(buf);pos++){
        if(buf[pos] == 'Z'){
          char read_data[6] = {(char)buf[pos+1],(char)buf[pos+2],(char)buf[pos+3],(char)buf[pos+4],(char)buf[pos+5],(char)buf[pos+6]};
          float d = 10 * atof(read_data);
          Zval = (int)d;//char_get_numbers(cur_pos-card.shortlen(buf)+pos+1);
          pos = MAX_CMD_SIZE;
        }
        if(buf[pos] == 'E'){
          for(uint32_t pos = 0;pos<card.shortlen(buf);pos++){
            if(buf[pos] == 'X'){
              char read_data[6] = {(char)buf[pos+1],(char)buf[pos+2],(char)buf[pos+3],(char)buf[pos+4],(char)buf[pos+5],(char)buf[pos+6]};
              float d = 10 * atof(read_data);
              val = (int)d;
              if(Xprev==0)Xprev = val;
              render_line((int)(((val>Xprev?val:Xprev)-X1)*k), (int)(((val<Xprev?val:Xprev)-X1)*k),0, (int)(k*Zval) /*layers++*/);
              Xprev = val;
            }
          }
          pos = MAX_CMD_SIZE;
        }
      }
      // f_lseek(&FileToRender, cur_pos);
    }


  }
  
  sprintf(txData,"echo: Image rendered, time: %d  \r\n",HAL_GetTick());
  HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), 10);

  render_clone();
  return 0;
}
