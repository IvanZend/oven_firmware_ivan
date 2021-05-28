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

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "cardreader.h"


// #include "../module/USBflash.h"
#include "fatfs.h"
#include "usb_host.h"

#include "../MarlinCore.h"
#include "../lcd/ultralcd.h"
#include "../LTDC/ltdc.h"
#include "../module/planner.h"        // for synchronize
#include "../module/printcounter.h"
#include "../core/language.h"
#include "../core/w25qxx.h"
#include "../gcode/queue.h"
#include "../gcode/render.h"
#include "../module/configuration_store.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/e_parser.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../feature/pause.h"
#endif

// public:
extern    FATFS 				                    USBDISKFatFs;  
extern    ApplicationTypeDef               Appli_state;
extern    int                               FL_sel;
uint16_t CardReader::filescounter;
// TCHAR CardReader::fileNames[255][255];
int CardReader::cur_page;
card_flags_t CardReader::flag;
uint8_t CardReader::sdprinting_done_state;
char CardReader::filename[FILENAME_LENGTH], CardReader::longFilename[LONG_FILENAME_LENGTH];
int8_t CardReader::autostart_index;

FATFS 				USBDISKFatFs;           							/* File system object for USB disk logical drive */
FIL 				FileFromSD;                   							/* File object */
FIL         FileToRender;
FILINFO 				fno;                   							/* File object */
DIR 				usb_directory;
FRESULT 			usb_result;
TCHAR           filedir[255];

TCHAR         sdfiles[15][255];
uint8_t       sdfile_selected = 255;


#if ENABLED(BINARY_FILE_TRANSFER) && NUM_SERIAL > 1
  int8_t CardReader::transfer_port_index;
#endif

// private:

SdFile CardReader::root, CardReader::workDir, CardReader::workDirParents[MAX_DIR_DEPTH];
uint8_t CardReader::workDirDepth;

#if ENABLED(SDCARD_SORT_ALPHA)

  uint16_t CardReader::sort_count;
  #if ENABLED(SDSORT_GCODE)
    bool CardReader::sort_alpha;
    int CardReader::sort_folders;
    //bool CardReader::sort_reverse;
  #endif

  #if ENABLED(SDSORT_DYNAMIC_RAM)
    uint8_t *CardReader::sort_order;
  #else
    uint8_t CardReader::sort_order[SDSORT_LIMIT];
  #endif

  #if ENABLED(SDSORT_USES_RAM)

    #if ENABLED(SDSORT_CACHE_NAMES)
      uint16_t CardReader::nrFiles; // Cached total file count
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        char **CardReader::sortshort, **CardReader::sortnames;
      #else
        char CardReader::sortshort[SDSORT_LIMIT][FILENAME_LENGTH];
        char CardReader::sortnames[SDSORT_LIMIT][SORTED_LONGNAME_STORAGE];
      #endif
    #elif DISABLED(SDSORT_USES_STACK)
      char CardReader::sortnames[SDSORT_LIMIT][SORTED_LONGNAME_STORAGE];
    #endif

    #if HAS_FOLDER_SORTING
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        uint8_t *CardReader::isDir;
      #elif ENABLED(SDSORT_CACHE_NAMES) || DISABLED(SDSORT_USES_STACK)
        uint8_t CardReader::isDir[(SDSORT_LIMIT+7)>>3];
      #endif
      #define IS_DIR(n) TEST(isDir[(n) >> 3], (n) & 0x07)
    #endif

  #endif // SDSORT_USES_RAM

#endif // SDCARD_SORT_ALPHA

Sd2Card CardReader::sd2card;
SdVolume CardReader::volume;
SdFile CardReader::file;

uint8_t CardReader::file_subcall_ctr;
uint32_t CardReader::filespos[SD_PROCEDURE_DEPTH];
TCHAR CardReader::proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];

uint32_t CardReader::filesize, CardReader::sdpos;
uint32_t CardReader::rendersize, CardReader::rsdpos;

CardReader::CardReader() {
  #if ENABLED(SDCARD_SORT_ALPHA)
    sort_count = 0;
    #if ENABLED(SDSORT_GCODE)
      sort_alpha = true;
      sort_folders = FOLDER_SORTING;
      //sort_reverse = false;
    #endif
  #endif
  flag.sdprinting = flag.mounted = flag.saving = flag.logging = false;
  filesize = sdpos = 0;
  file_subcall_ctr = 0;

  workDirDepth = 0;
  ZERO(workDirParents);

  // Disable autostart until card is initialized
  autostart_index = -1;

  #if PIN_EXISTS(SDPOWER)
    OUT_WRITE(SDPOWER_PIN, HIGH); // Power the SD reader
  #endif
}

//
// Get a DOS 8.3 filename in its useful form
//
char *createFilename(char * const buffer, const dir_t &p) {
  char *pos = buffer;
  LOOP_L_N(i, 11) {
    if (p.name[i] == ' ') continue;
    if (i == 8) *pos++ = '.';
    *pos++ = p.name[i];
  }
  *pos++ = 0;
  return buffer;
}
TCHAR *createFilename_ff(TCHAR * const buffer, const DIR &p) {
  TCHAR *pos = buffer;
  LOOP_L_N(i, 11) {
    if (p.fn[i] == ' ') continue;
    if (i == 8) *pos++ = '.';
    *pos++ = p.fn[i];
  }
  *pos++ = 0;
  return buffer;
}

//
// Return 'true' if the item is a folder or G-code file
//
bool CardReader::is_dir_or_gcode(const dir_t &p) {
  uint8_t pn0 = p.name[0];

  if ( pn0 == DIR_NAME_FREE || pn0 == DIR_NAME_DELETED  // Clear or Deleted entry
    || pn0 == '.' || longFilename[0] == '.'             // Hidden file
    || !DIR_IS_FILE_OR_SUBDIR(&p)                       // Not a File or Directory
    || (p.attributes & DIR_ATT_HIDDEN)                  // Hidden by attribute
  ) return false;

  flag.filenameIsDir = DIR_IS_SUBDIR(&p);               // We know it's a File or Folder

  return (
    flag.filenameIsDir                                  // All Directories are ok
    || (p.name[8] == 'G' && p.name[9] != '~')           // Non-backup *.G* files are accepted
  );
}
bool CardReader::is_dir_or_gcode_ff(FILINFO* p		/* Pointer to file information to return */) {
  uint8_t pn0 = p->fname[0];

  if ( pn0 == DIR_NAME_FREE || pn0 == DIR_NAME_DELETED  // Clear or Deleted entry
    || pn0 == '.' || longFilename[0] == '.'             // Hidden file
    || !DIR_IS_FILE_OR_SUBDIR_FF(p)                       // Not a File or Directory
    || (p->fattrib & DIR_ATT_HIDDEN)                  // Hidden by attribute
  ) return false;

  flag.filenameIsDir = DIR_IS_SUBDIR_FF(p);               // We know it's a File or Folder
  // return (
  //   flag.filenameIsDir                                  // All Directories are ok
  //   || (p->fname[8] == 'G' && p->fname[9] != '~')           // Non-backup *.G* files are accepted
  // );
  uint8_t pointcount = 0;
  uint8_t fileisG    = false;
  for(uint8_t i=0; i<shortlen(p->fname); i++){
    if(p->fname[i] == '.')pointcount++;
    if(p->fname[i] == 0){i=255;}
  }
  if(pointcount){
    for(uint8_t i=0; i<shortlen(p->fname); i++){
      if(p->fname[i] == '.'){
        pointcount--;
        if(pointcount == 0){
          if((p->fname[i+1] == 'G' || p->fname[i+1] == 'g' ) && (p->fname[i+2] == 0))fileisG = true; 

          if((p->fname[i+1] == 'G' || p->fname[i+1] == 'g' )\
          && (p->fname[i+2] == 'C' || p->fname[i+2] == 'c' )\
          && (p->fname[i+3] == 'O' || p->fname[i+3] == 'o' )\
          && (p->fname[i+4] == 'D' || p->fname[i+4] == 'd' )\
          && (p->fname[i+5] == 'E' || p->fname[i+5] == 'e' )\
          && (p->fname[i+6] == 0))fileisG = true; 
          return fileisG;
        }
      }
    }
  }
    
  return (
    flag.filenameIsDir                                  // All Directories are ok
    || 
    fileisG
    //(fno->altname[8] == 'G' && fno->altname[10] != '~')           // Non-backup *.G* files are accepted
  );
}

//
// Get the number of (compliant) items in the folder
//
int CardReader::countItems(SdFile dir) {
  dir_t p;
  int c = 0;
  while (dir.readDir(&p, longFilename) > 0)
    c += is_dir_or_gcode(p);

  #if ENABLED(SDCARD_SORT_ALPHA) && SDSORT_USES_RAM && SDSORT_CACHE_NAMES
    nrFiles = c;
  #endif

  return c;
}

//
// Get file/folder info for an item by index
//
void CardReader::selectByIndex(SdFile dir, const uint8_t index) {
  dir_t p;
  for (uint8_t cnt = 0; dir.readDir(&p, longFilename) > 0;) {
    if (is_dir_or_gcode(p)) {
      if (cnt == index) {
        createFilename(filename, p);
        return;  // 0 based index
      }
      cnt++;
    }
  }
}

//
// Get file/folder info for an item by name
//
void CardReader::selectByName(SdFile dir, const TCHAR * const match) {
  FRESULT res;
  DIR dirFF;
  // UINT i;
  static FILINFO fno;
  for (;;) {
      res = f_readdir(&dirFF, &fno);                   /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
      if (fno.fattrib & AM_DIR) {                    /* It is a directory */
        // SERIAL_ECHO('TEST1');
          // i = strlen('/');
          // sprintf(uintToHex,&path[i], "/%s \r\n", fno.fname);
          // sprintf(&path[i], "/%s", fno.fname);
          // USART_SendToCom(&uintToHex);
          // res = scan_files(path);                    /* Enter the directory */
          if (res != FR_OK) break;
          // path[i] = 0;
      } else {                                       /* It is a file. */      
        // SERIAL_ECHO('TEST2');

      }
  }




  // dir_t p;
  // for (uint8_t cnt = 0; dir.readDir(&p, longFilename) > 0; cnt++) {
  //   if (is_dir_or_gcode(p)) {
  //     createFilename(filename, p);
  //     if (strcasecmp(match, filename) == 0) return;
  //   }
  // }
}

//
// Recursive method to list all files within a folder
//
void CardReader::printListing(SdFile parent, const char * const prepend/*=nullptr*/) {
  dir_t p;
  while (parent.readDir(&p, longFilename) > 0) {
    if (DIR_IS_SUBDIR(&p)) {

      // Get the short name for the item, which we know is a folder
      char dosFilename[FILENAME_LENGTH];
      createFilename(dosFilename, p);

      // Allocate enough stack space for the full path to a folder, trailing slash, and nul
      const bool prepend_is_empty = (!prepend || prepend[0] == '\0');
      const int len = (prepend_is_empty ? 1 : strlen(prepend)) + strlen(dosFilename) + 1 + 1;
      char path[len];

      // Append the FOLDERNAME12/ to the passed string.
      // It contains the full path to the "parent" argument.
      // We now have the full path to the item in this folder.
      strcpy(path, prepend_is_empty ? "/" : prepend); // root slash if prepend is empty
      strcat(path, dosFilename);                      // FILENAME_LENGTH characters maximum
      strcat(path, "/");                              // 1 character

      // Serial.print(path);

      // Get a new directory object using the full path
      // and dive recursively into it.
      SdFile child;
      if (!child.open(&parent, dosFilename, O_READ)) {
        SERIAL_ECHO_START();
        SERIAL_ECHOLNPAIR(STR_SD_CANT_OPEN_SUBDIR, dosFilename);
      }
      printListing(child, path);
      // close() is done automatically by destructor of SdFile
    }
    else if (is_dir_or_gcode(p)) {
      createFilename(filename, p);
      if (prepend) SERIAL_ECHO(prepend);
      SERIAL_ECHO(filename);
      SERIAL_CHAR(' ');
      SERIAL_ECHOLN(p.fileSize);
    }
  }
}
//
// Recursive method to list all files within a folder
//
FRESULT CardReader::scan_files (int* num, bool display, int pos, TCHAR* path, char* debugs, TCHAR * prepend/*=nullptr*/) {        /* Start node to be scanned (***also used as work area***) */
    FRESULT res;
    DIR dir;
    // UINT i;
    static FILINFO fno;  
    
    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
      for (;;) {
        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        // char test3[256];
        // if(res!=FR_OK){
        //   for(int j=0;j<shortlen(fno.fname);j++){
        //     test3[j] = (uint8_t)fno.fname[j];
        //   }
        // }

        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */

        if(fno.fattrib == 0xFF){
          break;
        }else if (fno.fattrib & AM_DIR){                    /* It is a directory */
          
          /*
          * path: for fatfs usage
          * prepend: for displaying and Repetier Host usage
          * debugs: char string for debugging with more user-friendly reading
          */
          uint8_t a = shortlen(path);     
          uint8_t b = shortlen(fno.fname);
          uint8_t p = shortlen(prepend);

          TCHAR fatTCH[a+1+b];
          TCHAR preTCH[p+b+1];

          TCHAR *fatTCH1 = tcharconv(fatTCH,path,0,47);
          TCHAR *fatTCH2 = tcharconv(fatTCH1,fno.fname,a+1,0);

          TCHAR *preTCH1 = tcharconv(preTCH,prepend,0,0);

          TCHAR result2[p+b+1];
          std::copy(prepend, prepend + p, result2);
          std::copy(fno.fname, fno.fname + b, result2 + p);
          result2[p+b] = 47;
          result2[p+b+1] = 0;
          /*  
          //  *********
          //  DEBUGGING
          //  *********
          char fatCH2[a+1+b];
          char preCH2[a+1+b];
          tchartochar(preCH2, result1);
          tchartochar(fatCH2, fatTCH2);
          char usart_string1[255];
          char usart_string2[255];
          sprintf(usart_string1,"%s\n\r",fatCH2);
          sprintf(usart_string2,"%s\n\r",preCH2);
          uint8_t usart_len1 = strlen(usart_string1);
          uint8_t usart_len2 = strlen(usart_string2);
          HAL_UART_Transmit(&huart2, (uint8_t *)usart_string1, usart_len1, 10);
          HAL_UART_Transmit(&huart2, (uint8_t *)usart_string2, usart_len2, 10);
          */

          char dbg[a+1+b];


          res = scan_files(num, display, pos,  fatTCH2, dbg/*text for debug*/, result2);

          if (res != FR_OK) break;

        }else if (is_dir_or_gcode_ff(&fno)){                                      /* It is a file. */
          if(prepend){
            for(size_t j=0;j<shortlen(prepend);j++){
              if(prepend[j]>127){
                SERIAL_CHAR((prepend[j]>>6) | 0xC0);
                SERIAL_CHAR((prepend[j] & 0x3F) | 0x80);
              }else{
                SERIAL_CHAR(prepend[j]);
              }
            }
          }
          for(size_t j=0;j<shortlen(fno.fname);j++){
            if(fno.fname[j]>127){
              SERIAL_CHAR((fno.fname[j]>>6) | 0xC0);
              SERIAL_CHAR((fno.fname[j] & 0x3F) | 0x80);
            }else{
              SERIAL_CHAR(fno.fname[j]);
            }
          }
          SERIAL_CHAR(' ');
          SERIAL_ECHOLN(fno.fsize);
          
          if(display){
            uint16_t disp_full[34];
            
            int i=0 , len_p = 0, len_n = 0;
            /*PATH*/
            len_p = shortlen(path);
            if(len_p>1){
              len_p++;
            }
            if(len_p>17)len_p = 17;
            uint16_t disp_path[len_p];
            for(i=0;i<len_p;i++){
              disp_path[i] = path[i];
            }
            if(shortlen(path)>17){
              disp_path[len_p-4] = 46;// "."
              disp_path[len_p-3] = 46;// "."
              disp_path[len_p-2] = 46;// "."
            }
            if(len_p>1){
              disp_path[len_p-1] = 47;// "/" 
            }
            /*NAME*/
            len_n = shortlen(fno.fname);
            len_n++;
            if(len_n>17)len_n = 17;
            uint16_t disp_name[len_n];
            for(i=0;i<len_n;i++){
              disp_name[i] = fno.fname[i];
            }
            if(shortlen(fno.fname)>17){
              disp_name[len_n-4] = 46;// "."
              disp_name[len_n-3] = 46;// "."
              disp_name[len_n-2] = 46;// "."
            }
            disp_name[len_n-1] = '\000'; 
            /*FULL*/
            for(i=0;i<len_p;i++){
              disp_full[i] = disp_path[i];
            }
            for(int j=0;j<len_n;j++){
              disp_full[i+j] = disp_name[j];
            }
            int len_f = len_p+len_n;
            len_f++;
            // if(len_f>30)len_f = 30;
            disp_full[len_f-1] = '\000'; 
            // sprintf(dispname,"%s",fno.fname);
            // pos = pos;
            if((cur_page*14 - filescounter)>=0 && (cur_page*14 - filescounter)<=14){
              int Ypos = 126+(filescounter-((cur_page-1)*14))*24;
              // if(((filescounter)-(pos - cur_page*14)>=0) && (14>=(filescounter)-(pos - cur_page*14))){
              uint8_t printcount = filescounter-(cur_page-1)*14;
              for(int a=0;a<255;a++){
                sdfiles[printcount][a] = 0;
              }

              uint8_t fileprint_path_len = shortlen(path);
              uint8_t fileprint_name_len = shortlen(fno.fname);


              for(int a=0;a<shortlen(fno.fname);a++){}
              if(fileprint_path_len>0){
                int a=0;
                while(a<fileprint_path_len){
                  if(path[a]==0)break;
                  sdfiles[printcount][a] = path[a];
                  a++;
                }
              }
              if(fileprint_path_len>1){
                sdfiles[printcount][fileprint_path_len] = 47;
                fileprint_path_len++;
              }
              int a=fileprint_path_len;
              while(a<(fileprint_path_len+fileprint_name_len)){
                if(fno.fname[a-fileprint_path_len]==0)break;
                sdfiles[printcount][a] = fno.fname[a-fileprint_path_len];
                a++;
              }

              TFT_DrawTextArr(TFT_FONT24,155,Ypos, disp_full, 0, MAIN_LAYER, MAIN_LAYER,TTZ_COL_ORAN);
              TFT_DrawFLatLine(155,Ypos-1, 645, Ypos-1, TTZ_COL_D_GR, MAIN_LAYER);
            }            
          }
          filescounter = filescounter+1;
        }
      }
      f_closedir(&dir);
    }
    

    return res;
}

size_t CardReader::shortlen(TCHAR * str){
  size_t length = 0;
	while (*str++)
		length++;
	return length;
}
void CardReader::tchartochar(char *ch, TCHAR * str){  
  uint8_t len = shortlen(str);  
      
  for(int j=0;j<len;j++){
    ch[j] = (uint8_t)str[j];
  }
  ch[len] = 0;
}
unsigned short * CardReader::tcharconv(unsigned short tostr[], unsigned short fromstr[], uint8_t start, uint16_t endsign){  
  uint8_t len = shortlen(fromstr);  
  for(int j=0;j<len;j++){
    tostr[start+j] = fromstr[j];
  }
  tostr[start+len] = endsign;
  tostr[start+len+1] = 0;
  return tostr;
}

void CardReader::select_to_print(uint16_t bt) {


  /***********************************/
  char printfile1[255];
  TCHAR printfile2[255];
  bool cyrrilic = false;
  for(int p=0;p<255;p++){
    printfile1[p] = (char)sdfiles[bt-510][p];
    printfile2[p] = sdfiles[bt-510][p];
    if(sdfiles[bt-510][p]>255)cyrrilic = true;
  }
  if(cyrrilic == true){
    card.openFileRender(NULL,printfile2);
  }
  else{
    card.openFileRender(printfile1);
  }
  /***********************************/



  if(sdfile_selected!=255){
    for(int i=0;i<=14;i++){
      TFT_DrawFLatLine(155,126+i*24-1, 645, 126+i*24-1, TTZ_COL_D_GR, MAIN_LAYER);
      TFT_DrawFLatLine(155,126+i*24+23, 645, 126+i*24-25, TTZ_COL_D_GR, MAIN_LAYER);      
    }
  }
  sdfile_selected = bt-510;
  int Ypos = 126+(bt-510)*24;

  TFT_DrawFLatLine(155,Ypos-1, 645, Ypos-1, LCD_COLOR_YELLOW, MAIN_LAYER);
  TFT_DrawFLatLine(155,Ypos+23, 645, Ypos-25, LCD_COLOR_YELLOW, MAIN_LAYER);






}
//
// List all files on the SD card
//
void CardReader::ls(bool display,int pos) {
  root.rewind();
  char dbg[256] = "";
  // printListing(root);
  TCHAR buff[1];
  buff[0] = 0;
  // strcpy((char*)buff, "/");
  if(Appli_state == APPLICATION_READY){mount();}
  if(isMounted()){
    int num = 0;
    // if(MarlinUI::currentScreen != MarlinUI::menu_sdmain)filescounter = 0;
    // cur_page = 1;
    // memset(fileNames, 0, sizeof(fileNames));
    if(cur_page<1){
      cur_page=1;
      display = false;
    }  
    if(filescounter>0 && cur_page>1){
      int pages   = filescounter/14+1;
      if(cur_page>pages){cur_page=pages;
      display = false;}
    }  
    if(pos == 0)filescounter = 0;
    if(display){
      TFT_DrawRectangle(/*x,y*/150,100, /*width, height*/500,400, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/0x0,MAIN_LAYER,/*thikness in px*/201,/*fill?*/1,/*punktir?*/0);
    }
    scan_files(&num, display, pos, buff, dbg, buff);
    
    if(display){
      int pages   = filescounter/14+1;
      cur_page=cur_page>pages?pages:cur_page;

      int digits2 = 0;
      uint16_t n2 = pages;
      while (n2 != 0) 
      {
          n2 = n2 / 10;
          ++digits2;
      }
      char files_c[10];
      sprintf(files_c,"[%d/%d]",cur_page,pages);
      TCHAR p[strlen(files_c)];
      for(size_t j=0;j<strlen(files_c);j++){
        p[j] = files_c[j];
      }
      p[strlen(files_c)] = 0;
      TFT_DrawTextArr(TFT_FONT24,155,101, p, 0, MAIN_LAYER, MAIN_LAYER,TTZ_COL_ORAN);
    }


  }
}

#if ENABLED(LONG_FILENAME_HOST_SUPPORT)

  //
  // Get a long pretty path based on a DOS 8.3 path
  //
  void CardReader::printLongPath(char * const path) {

    int i, pathLen = strlen(path);

    // SERIAL_ECHOPGM("Full Path: "); SERIAL_ECHOLN(path);

    // Zero out slashes to make segments
    for (i = 0; i < pathLen; i++) if (path[i] == '/') path[i] = '\0';

    SdFile diveDir = root; // start from the root for segment 1
    for (i = 0; i < pathLen;) {

      if (path[i] == '\0') i++; // move past a single nul

      char *segment = &path[i]; // The segment after most slashes

      // If a segment is empty (extra-slash) then exit
      if (!*segment) break;

      // Go to the next segment
      while (path[++i]) { }

      // SERIAL_ECHOPGM("Looking for segment: "); SERIAL_ECHOLN(segment);

      // Find the item, setting the long filename
      diveDir.rewind();
      selectByName(diveDir, segment);

      // Print /LongNamePart to serial output
      SERIAL_CHAR('/');
      SERIAL_ECHO(longFilename[0] ? longFilename : "???");

      // If the filename was printed then that's it
      if (!flag.filenameIsDir) break;

      // SERIAL_ECHOPGM("Opening dir: "); SERIAL_ECHOLN(segment);

      // Open the sub-item as the new dive parent
      SdFile dir;
      if (!dir.open(&diveDir, segment, O_READ)) {
        SERIAL_EOL();
        SERIAL_ECHO_START();
        SERIAL_ECHOPAIR(STR_SD_CANT_OPEN_SUBDIR, segment);
        break;
      }

      diveDir.close();
      diveDir = dir;

    } // while i<pathLen

    SERIAL_EOL();
  }

#endif // LONG_FILENAME_HOST_SUPPORT

//
// Echo the DOS 8.3 filename (and long filename, if any)
//
void CardReader::printFilename() {
  if (file.isOpen()) {
    char dosFilename[FILENAME_LENGTH];
    file.getDosName(dosFilename);
    SERIAL_ECHO(dosFilename);
    #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
      selectFileByName(dosFilename);
      if (longFilename[0]) {
        SERIAL_ECHO(' ');
        SERIAL_ECHO(longFilename);
      }
    #endif
  }
  else
    SERIAL_ECHOPGM("(no file)");

  SERIAL_EOL();
}

void CardReader::mount() {
  if(flag.mounted == true && Appli_state == APPLICATION_READY){

  }else if(Appli_state != APPLICATION_READY){
    flag.mounted = false;
  }else{
    flag.mounted = false;

    if(f_mount(&USBDISKFatFs, (TCHAR const*)USBHPath, 0) == FR_OK)
    {
      FRESULT res;
      DIR dir;
      // UINT i;
      res = f_opendir(&dir, (TCHAR*)"/"); 
      if(res == FR_OK){
        flag.mounted = true;
      }else{
        ui.set_status(GET_TEXT(MSG_FORMAT_UNSUPPORTED));
      }
    }
  }

/////////////////////////////////////////////////////

/////////////////////////////////////////////////////


  
  // if (root.isOpen()) root.close();

  // if (!sd2card.init(SPI_SPEED, SDSS)
  //   #if defined(LCD_SDSS) && (LCD_SDSS != SDSS)
  //     && !sd2card.init(SPI_SPEED, LCD_SDSS)
  //   #endif
  // ) SERIAL_ECHO_MSG(STR_SD_INIT_FAIL);
/*/////////////////////////////////////////////////////
  else if (!volume.init(&sd2card))
    SERIAL_ERROR_MSG(STR_SD_VOL_INIT_FAIL);

  else if (!root.openRoot(&volume))
    SERIAL_ERROR_MSG(STR_SD_OPENROOT_FAIL);
/////////////////////////////////////////////////////*/
  // else {
  //   flag.mounted = true;
  //   SERIAL_ECHO_MSG(STR_SD_CARD_OK);
  //   #if ENABLED(SDCARD_EEPROM_EMULATION)
  //     settings.first_load();
  //   #endif
  // }



  // if(f_mount(&USBDISKFatFs, (TCHAR const*)USBHPath, 0) == FR_OK){
  //   flag.mounted = true;
  //   SERIAL_ECHO_MSG(STR_SD_CARD_OK);
  // }else{
  // }

  // FRESULT res;
  // DIR dir;

  // res = f_opendir(&dir, "/");                       /* Open the directory */
  // if (res == FR_OK) {
  //   SERIAL_ECHO_MSG(STR_SD_CARD_OK);
  // }else{
  // }


  // cdroot();

  ui.refresh();
}

void CardReader::release() {
  endFilePrint();
  flag.mounted = false;
}

void CardReader::openAndPrintFile(const char *name) {
  char cmd[4 + strlen(name) + 1]; // Room for "M23 ", filename, and null
  extern const char M23_STR[];
  sprintf_P(cmd, M23_STR, name);
  for (char *c = &cmd[4]; *c; c++) *c = tolower(*c);
  queue.enqueue_one_now(cmd);
  queue.enqueue_now_P(M24_STR);
}

void CardReader::startFileprint() {
  if (isMounted()) {
    flag.sdprinting = true;
    #if SD_RESORT
      flush_presort();
    #endif
  }
}

void CardReader::endFilePrint(
  #if SD_RESORT
    const bool re_sort/*=false*/
  #endif
) {
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    did_pause_print = 0;
  #endif
  flag.sdprinting = flag.abort_sd_printing = false;
  if (isFileOpen()) file.close();
  #if SD_RESORT
    if (re_sort) presort();
  #endif
}

void CardReader::openLogFile(char * const path) {
  flag.logging = true;
  openFileWrite(path);
}

//
// Get the root-relative DOS path of the selected file
//
void CardReader::getAbsFilename(TCHAR *dst) {
  // *dst++ = '/';
  // uint8_t cnt = 1;


  size_t s = shortlen(filedir);

  // *dst++ = 47;
  for(size_t i=0; i<s;i++){
    *dst++ = filedir[i];
  }
  *dst++ = 0;

  // auto appendAtom = [&](SdFile &file) {
  //   file.getDosName(dst);
  //   while (*dst && cnt < MAXPATHNAMELENGTH) { dst++; cnt++; }
  //   if (cnt < MAXPATHNAMELENGTH) { *dst = '/'; dst++; cnt++; }
  // };

  // LOOP_L_N(i, workDirDepth)                // Loop down to current work dir
  //   appendAtom(workDirParents[i]);

  // if (cnt < MAXPATHNAMELENGTH - (FILENAME_LENGTH) - 1) {    // Leave room for filename and nul
  //   appendAtom(file);
  //   --dst;
  // }
  // *dst = '\0';
}

void openFailed(const TCHAR * const fname) {
  SERIAL_ECHOLNPAIR(STR_SD_OPEN_FILE_FAIL, fname, ".");
}

void announceOpen(const uint8_t doing, const TCHAR * const path) {
  if (doing) {
    PORT_REDIRECT(SERIAL_BOTH);
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Now ");
    serialprintPGM(doing == 1 ? PSTR("doing") : PSTR("fresh"));
    SERIAL_ECHOLNPAIR(" file: ", path);
    PORT_RESTORE();
  }
}

void CardReader::openFileRender(char * const path, TCHAR * const altpath, const uint8_t subcall_type/*=0*/) {
if (!isMounted()) return;

  TCHAR unipath[255];

  if(path == NULL){
    for(size_t i=0; i<shortlen(altpath);i++){
      unipath[i] = altpath[i];
    }
    unipath[shortlen(altpath)] = 0;
  }else if(altpath == NULL){
    for(size_t i=0; i<strlen(path);i++){
      unipath[i] = path[i];
    }
    unipath[strlen(path)] = 0;
  }else{
    return;
  }

  filedir[0] = 47;
  for(size_t i=0; i<shortlen(unipath);i++){
    filedir[i+1] = unipath[i];
  }
  filedir[shortlen(unipath)+1] = 0;

  file_subcall_ctr = 0;

  size_t s = shortlen(unipath);
  TCHAR fname_s[s+1];
  fname_s[0] = 47;
  for(size_t i=0; i<s;i++){
    fname_s[i+1] = unipath[i];
  }
  fname_s[s+1] = 0;
  if (!fname_s) return;

  FRESULT res;  

  res = f_open(&FileToRender, fname_s, FA_READ);
  if(res != FR_OK)openFailed(unipath);

  res = f_stat(fname_s, &fno);
  if(res != FR_OK)openFailed(unipath);

  rendersize = FileToRender.obj.objsize;
  rsdpos = 0;
}


//
// Open a file by DOS path for read
// The 'subcall_type' flag indicates...
//   - 0 : Standard open from host or user interface.
//   - 1 : (file open) Opening a new sub-procedure.
//   - 1 : (no file open) Opening a macro (M98).
//   - 2 : Resuming from a sub-procedure
//
void CardReader::openFileRead(char * const path, TCHAR * const altpath, const uint8_t subcall_type/*=0*/) {
  if (!isMounted()) return;

  TCHAR unipath[255];

  if(path == NULL){
    for(size_t i=0; i<shortlen(altpath);i++){
      unipath[i] = altpath[i];
    }
    unipath[shortlen(altpath)] = 0;
  }else if(altpath == NULL){
    for(size_t i=0; i<strlen(path);i++){
      unipath[i] = path[i];
    }
    unipath[strlen(path)] = 0;
  }else{
    return;
  }



  filedir[0] = 47;
  for(size_t i=0; i<shortlen(unipath);i++){
    filedir[i+1] = unipath[i];
  }
  filedir[shortlen(unipath)+1] = 0;


  switch (subcall_type) {
    case 0:      // Starting a new print. "Now fresh file: ..."
      announceOpen(2, unipath);
      file_subcall_ctr = 0;
      break;

    case 1:      // Starting a sub-procedure

      // With no file is open it's a simple macro. "Now doing file: ..."
      if (!isFileOpen()) { announceOpen(1, unipath); break; }

      // Too deep? The firmware has to bail.
      if (file_subcall_ctr > SD_PROCEDURE_DEPTH - 1) {
        SERIAL_ERROR_MSG("trying to call sub-gcode files with too many levels. MAX level is:" STRINGIFY(SD_PROCEDURE_DEPTH));
        kill();
        return;
      }

      // Store current filename (based on workDirParents) and position
      getAbsFilename(proc_filenames[file_subcall_ctr]);
      filespos[file_subcall_ctr] = sdpos;

      // For sub-procedures say 'SUBROUTINE CALL target: "..." parent: "..." pos12345'
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPAIR("SUBROUTINE CALL target:\"", unipath, "\" parent:\"", proc_filenames[file_subcall_ctr], "\" pos", sdpos);
      file_subcall_ctr++;
      break;

    case 2:      // Resuming previous file after sub-procedure
      SERIAL_ECHO_MSG("END SUBROUTINE");
      break;
  }

  endFilePrint();

  // SdFile *curDir;
  // const char * const fname = diveToFile(true, curDir, path);
  size_t s = shortlen(unipath);
  TCHAR fname_s[s+1];
  fname_s[0] = 47;
  for(size_t i=0; i<s;i++){
    fname_s[i+1] = unipath[i];
  }
  fname_s[s+1] = 0;
  if (!fname_s) return;
/////////////////////////////////////////////////////
  FRESULT res;  

  res = f_open(&FileFromSD, fname_s, FA_READ);
  if(res != FR_OK)openFailed(unipath);

  res = f_stat(fname_s, &fno);
  if(res != FR_OK)openFailed(unipath);

  filesize = FileFromSD.obj.objsize;
  sdpos = 0;

  PORT_REDIRECT(SERIAL_BOTH);
  SERIAL_ECHOLNPAIR(STR_SD_FILE_OPENED, unipath, STR_SD_SIZE, filesize);
  SERIAL_ECHOLNPGM(STR_SD_FILE_SELECTED);
  PORT_RESTORE();

  selectFileByName(unipath);
  
/////////////////////////////////////////////////////
  // if (file.open(curDir, fname, O_READ)) {
  //   filesize = file.fileSize();
  //   sdpos = 0;

  //   PORT_REDIRECT(SERIAL_BOTH);
  //   SERIAL_ECHOLNPAIR(STR_SD_FILE_OPENED, fname, STR_SD_SIZE, filesize);
  //   SERIAL_ECHOLNPGM(STR_SD_FILE_SELECTED);
  //   PORT_RESTORE();

  //   selectFileByName(fname);
  //   ui.set_status(longFilename[0] ? longFilename : fname);
  // }
  // else
  //   openFailed(fname);
}

inline void echo_write_to_file(const char * const fname) {
  SERIAL_ECHOLNPAIR(STR_SD_WRITE_TO_FILE, fname);
}

//
// Open a file by DOS path for write
//
void CardReader::openFileWrite(char * const path) {
  if (!isMounted()) return;

  // announceOpen(2, " ");
  file_subcall_ctr = 0;

  endFilePrint();

  size_t s = strlen(path);
  TCHAR fname_s[s+1];
  fname_s[0] = 47;
  for(size_t i=0; i<s;i++){
    fname_s[i+1] = path[i];
  }
  if (!fname_s) return;
/////////////////////////////////////////////////////
  FRESULT res;
  // SdFile *curDir;

  // if (!fname_s) return;
  res = f_open(&FileFromSD, fname_s, FA_CREATE_ALWAYS | FA_CREATE_NEW | FA_READ | FA_WRITE);
  if(res == FR_OK){
    flag.saving = true;
    selectFileByName(fname_s);
    #if ENABLED(EMERGENCY_PARSER)
      emergency_parser.disable();
    #endif
    echo_write_to_file(path);
    ui.set_status_P(path,false);
  }else{
    openFailed(fname_s);
  }
  // if (file.open(curDir, fname, O_CREAT | O_APPEND | O_WRITE | O_TRUNC)) {
  //   flag.saving = true;
  //   selectFileByName(fname);
  //   #if ENABLED(EMERGENCY_PARSER)
  //     emergency_parser.disable();
  //   #endif
  //   echo_write_to_file(fname);
  //   ui.set_status(fname);
  // }
  // else
  //   openFailed(fname);
}

//
// Delete a file by name in the working directory
//
void CardReader::removeFile(const char * const name) {
  if (!isMounted()) return;
  size_t s = strlen(name);
  TCHAR fname_s[s+1];
  fname_s[0] = 47;
  for(size_t i=0; i<s;i++){
    fname_s[i+1] = name[i];
  }
  if (!fname_s) return;
/////////////////////////////////////////////////////
  FRESULT res;
  res = f_unlink (fname_s);
  if(res == FR_OK){
    SERIAL_ECHOLNPAIR("File deleted:", name);
    sdpos = 0;
    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif
  }else{
    SERIAL_ECHOLNPAIR("Deletion failed, File: ", name, ".");
  }
  //endFilePrint();

  // SdFile *curDir;
  // const char * const fname = diveToFile(false, curDir, name);
  // if (!fname) return;

  // if (file.remove(curDir, fname)) {
  //   SERIAL_ECHOLNPAIR("File deleted:", fname);
  //   sdpos = 0;
  //   #if ENABLED(SDCARD_SORT_ALPHA)
  //     presort();
  //   #endif
  // }
  // else
  //   SERIAL_ECHOLNPAIR("Deletion failed, File: ", fname, ".");
}

void CardReader::report_status() {
  if (isPrinting()) {
    SERIAL_ECHOPGM(STR_SD_PRINTING_BYTE);
    SERIAL_ECHO(sdpos);
    SERIAL_CHAR('/');
    SERIAL_ECHOLN(filesize);
  }
  else
    SERIAL_ECHOLNPGM(STR_SD_NOT_PRINTING);
}

void CardReader::write_command(char * const buf) {
  char* begin = buf;
  char* npos = nullptr;
  char* end = buf + strlen(buf) - 1;

  file.writeError = false;
  if ((npos = strchr(buf, 'N')) != nullptr) {
    begin = strchr(npos, ' ') + 1;
    end = strchr(npos, '*') - 1;
  }
  end[1] = '\r';
  end[2] = '\n';
  end[3] = '\0';
  file.write(begin);

  if (file.writeError) SERIAL_ERROR_MSG(STR_SD_ERR_WRITE_TO_FILE);
}

//
// Run the next autostart file. Called:
// - On boot after successful card init
// - After finishing the previous autostart file
// - From the LCD command to run the autostart file
//

void CardReader::checkautostart() {

  #ifdef ENDSTOPTEST
  int counterI = 0;
  if(Appli_state == APPLICATION_START){
    while(Appli_state != APPLICATION_READY){
      counterI++;
      MX_USB_HOST_Process();
      // HAL_Delay(1);
      if(counterI>1000){
        ui.set_status_P(GET_TEXT(MSG_FORMAT_UNSUPPORTED),0);
        break;
      }
      // TestFuncForReference(counterI);
      if(Appli_state == APPLICATION_IDLE)return;
      if(Appli_state == APPLICATION_DISCONNECT)return;
    }
  }


  if (autostart_index < 0 || flag.sdprinting) return;

  if (!isMounted()) mount();
  #if ENABLED(SDCARD_EEPROM_EMULATION)
    else settings.first_load();
  #endif

  if (isMounted()
    #if ENABLED(POWER_LOSS_RECOVERY)
      && !recovery.valid() // Don't run auto#.g when a resume file exists
    #endif
  ) {
    char autoname[8];
    sprintf_P(autoname, PSTR("auto%c.g"), autostart_index + '0');
    // dir_t p;
    root.rewind();
    FRESULT res;
    DIR dir;
    // UINT i;
    static FILINFO fno;
    res = f_opendir(&dir, (TCHAR*)"/");   
    // res = f_readdir(&dir, &fno);  
    if (res == FR_OK){


      for (;;) {
        res = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fattrib & AM_DIR) {                    /* It is a directory */
            // i = strlen(path);
            // sprintf(uintToHex,&path[i], "/%s \r\n", fno.fname);
            // sprintf(&path[i], "/%s", fno.fname);
            // USART_SendToCom(&uintToHex);
            // res = scan_files(path);                    /* Enter the directory */
            // if (res != FR_OK) break;
            // path[i] = 0;
        } else {                                       /* It is a file. */
          // sprintf(uintToHex,"%s/%s \r\n", path, fno.fname);
          for (int8_t i = (int8_t)strlen((char*)fno.fname); i--;) fno.fname[i] = tolower(fno.fname[i]);
          if (fno.fname[9] != '~' && \
          fno.fname[0] == autoname[0] &&\
          fno.fname[1] == autoname[1] &&\
          fno.fname[2] == autoname[2] &&\
          fno.fname[3] == autoname[3] &&\
          fno.fname[4] == autoname[4] &&\
          fno.fname[5] == autoname[5] &&\
          fno.fname[6] == autoname[6] &&\
          true) {
            openAndPrintFile(autoname);
            autostart_index++;
            return;
          }
          // USART_SendToCom(&uintToHex);
        }
      }
    }else{

    }
    




    // while (root.readDir(&p, nullptr) > 0) {
    //   for (int8_t i = (int8_t)strlen((char*)p.name); i--;) p.name[i] = tolower(p.name[i]);
    //   if (p.name[9] != '~' && strncmp((char*)p.name, autoname, 5) == 0) {
    //     openAndPrintFile(autoname);
    //     autostart_index++;
    //     return;
    //   }
    // }
  }
  #endif
  autostart_index = -1;
}

void CardReader::beginautostart() {
  autostart_index = 0;
  cdroot();
}

void CardReader::closefile(const bool store_location) {
  file.sync();
  file.close();
  flag.saving = flag.logging = false;
  sdpos = 0;
  #if ENABLED(EMERGENCY_PARSER)
    emergency_parser.enable();
  #endif

  if (store_location) {
    //future: store printer state, filename and position for continuing a stopped print
    // so one can unplug the printer and continue printing the next day.
  }
}

//
// Get info for a file in the working directory by index
//
void CardReader::selectFileByIndex(const uint16_t nr) {
  #if ENABLED(SDSORT_CACHE_NAMES)
    if (nr < sort_count) {
      strcpy(filename, sortshort[nr]);
      strcpy(longFilename, sortnames[nr]);
      flag.filenameIsDir = IS_DIR(nr);
      return;
    }
  #endif
  workDir.rewind();
  selectByIndex(workDir, nr);
}

//
// Get info for a file in the working directory by DOS name
//
void CardReader::selectFileByName(const TCHAR * const match) {
  #if ENABLED(SDSORT_CACHE_NAMES)
    for (uint16_t nr = 0; nr < sort_count; nr++)
      if (strcasecmp(match, sortshort[nr]) == 0) {
        strcpy(filename, sortshort[nr]);
        strcpy(longFilename, sortnames[nr]);
        flag.filenameIsDir = IS_DIR(nr);
        return;
      }
  #endif
  workDir.rewind();
  selectByName(workDir, match);
}

uint16_t CardReader::countFilesInWorkDir() {
  workDir.rewind();
  return countItems(workDir);
}

/**
 * Dive to the given DOS 8.3 file path, with optional echo of the dive paths.
 *
 * On exit, curDir contains an SdFile reference to the file's directory.
 *
 * Returns a pointer to the last segment (filename) of the given DOS 8.3 path.
 *
 * A nullptr result indicates an unrecoverable error.
 */
const char* CardReader::diveToFile(const bool update_cwd, SdFile*& curDir, const char * const path, const bool echo/*=false*/) {
  // Track both parent and subfolder
  static SdFile newDir1, newDir2;
  SdFile *sub = &newDir1, *startDir;

  // Parsing the path string
  const char *item_name_adr = path;

  if (path[0] == '/') {               // Starting at the root directory?
    curDir = &root;
    if (update_cwd) workDirDepth = 0; // The cwd can be updated for the benefit of sub-programs
    item_name_adr++;
  }
  else
    curDir = &workDir;                // Dive from workDir (as set by the UI)

  startDir = curDir;
  while (item_name_adr) {
    // Find next subdirectory delimiter
    char * const name_end = strchr(item_name_adr, '/');

    // Last atom in the path? Item found.
    if (name_end <= item_name_adr) break;

    // Set subDirName
    const uint8_t len = name_end - item_name_adr;
    char dosSubdirname[len + 1];
    strncpy(dosSubdirname, item_name_adr, len);
    dosSubdirname[len] = 0;

    if (echo) SERIAL_ECHOLN(dosSubdirname);

    // Open curDir
    if (!sub->open(curDir, dosSubdirname, O_READ)) {
      SERIAL_ECHOLNPAIR(STR_SD_OPEN_FILE_FAIL, dosSubdirname, ".");
      return nullptr;
    }

    // Close curDir if not at starting-point
    if (curDir != startDir) curDir->close();

    // curDir now subDir
    curDir = sub;

    // Update workDirParents, workDirDepth, and workDir
    if (update_cwd) {
      if (workDirDepth < MAX_DIR_DEPTH) workDirParents[workDirDepth++] = *curDir;
      workDir = *curDir;
    }

    // Point sub at the other scratch object
    sub = (curDir != &newDir1) ? &newDir1 : &newDir2;

    // Next path atom address
    item_name_adr = name_end + 1;
  }
  return item_name_adr;
}

void CardReader::cd(const char * relpath) {
  SdFile newDir;
  SdFile *parent = workDir.isOpen() ? &workDir : &root;

  if (newDir.open(parent, relpath, O_READ)) {
    workDir = newDir;
    flag.workDirIsRoot = false;
    if (workDirDepth < MAX_DIR_DEPTH)
      workDirParents[workDirDepth++] = workDir;
    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif
  }
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(STR_SD_CANT_ENTER_SUBDIR, relpath);
  }
}

int8_t CardReader::cdup() {
  if (workDirDepth > 0) {                                               // At least 1 dir has been saved
    workDir = --workDirDepth ? workDirParents[workDirDepth - 1] : root; // Use parent, or root if none
    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif
  }
  if (!workDirDepth) flag.workDirIsRoot = true;
  return workDirDepth;
}

void CardReader::cdroot() {
  workDir = root;
  flag.workDirIsRoot = true;
  #if ENABLED(SDCARD_SORT_ALPHA)
    presort();
  #endif
}

#if ENABLED(SDCARD_SORT_ALPHA)

  /**
   * Get the name of a file in the working directory by sort-index
   */
  void CardReader::getfilename_sorted(const uint16_t nr) {
    selectFileByIndex(
      #if ENABLED(SDSORT_GCODE)
        sort_alpha &&
      #endif
      (nr < sort_count) ? sort_order[nr] : nr
    );
  }

  #if ENABLED(SDSORT_USES_RAM)
    #if ENABLED(SDSORT_DYNAMIC_RAM)
      // Use dynamic method to copy long filename
      #define SET_SORTNAME(I) (sortnames[I] = strdup(longest_filename()))
      #if ENABLED(SDSORT_CACHE_NAMES)
        // When caching also store the short name, since
        // we're replacing the selectFileByIndex() behavior.
        #define SET_SORTSHORT(I) (sortshort[I] = strdup(filename))
      #else
        #define SET_SORTSHORT(I) NOOP
      #endif
    #else
      // Copy filenames into the static array
      #define _SET_SORTNAME(I) strncpy(sortnames[I], longest_filename(), SORTED_LONGNAME_MAXLEN)
      #if SORTED_LONGNAME_MAXLEN == LONG_FILENAME_LENGTH
        // Short name sorting always use LONG_FILENAME_LENGTH with no trailing nul
        #define SET_SORTNAME(I) _SET_SORTNAME(I)
      #else
        // Copy multiple name blocks. Add a nul for the longest case.
        #define SET_SORTNAME(I) do{ _SET_SORTNAME(I); sortnames[I][SORTED_LONGNAME_MAXLEN] = '\0'; }while(0)
      #endif
      #if ENABLED(SDSORT_CACHE_NAMES)
        #define SET_SORTSHORT(I) strcpy(sortshort[I], filename)
      #else
        #define SET_SORTSHORT(I) NOOP
      #endif
    #endif
  #endif

  /**
   * Read all the files and produce a sort key
   *
   * We can do this in 3 ways...
   *  - Minimal RAM: Read two filenames at a time sorting along...
   *  - Some RAM: Buffer the directory just for this sort
   *  - Most RAM: Buffer the directory and return filenames from RAM
   */
  void CardReader::presort() {

    // Throw away old sort index
    flush_presort();

    // Sorting may be turned off
    #if ENABLED(SDSORT_GCODE)
      if (!sort_alpha) return;
    #endif

    // If there are files, sort up to the limit
    uint16_t fileCnt = countFilesInWorkDir();
    if (fileCnt > 0) {

      // Never sort more than the max allowed
      // If you use folders to organize, 20 may be enough
      NOMORE(fileCnt, uint16_t(SDSORT_LIMIT));

      // Sort order is always needed. May be static or dynamic.
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        sort_order = new uint8_t[fileCnt];
      #endif

      // Use RAM to store the entire directory during pre-sort.
      // SDSORT_LIMIT should be set to prevent over-allocation.
      #if ENABLED(SDSORT_USES_RAM)

        // If using dynamic ram for names, allocate on the heap.
        #if ENABLED(SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            sortshort = new char*[fileCnt];
            sortnames = new char*[fileCnt];
          #endif
        #elif ENABLED(SDSORT_USES_STACK)
          char sortnames[fileCnt][SORTED_LONGNAME_STORAGE];
        #endif

        // Folder sorting needs 1 bit per entry for flags.
        #if HAS_FOLDER_SORTING
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            isDir = new uint8_t[(fileCnt + 7) >> 3];
          #elif ENABLED(SDSORT_USES_STACK)
            uint8_t isDir[(fileCnt + 7) >> 3];
          #endif
        #endif

      #else // !SDSORT_USES_RAM

        // By default re-read the names from SD for every compare
        // retaining only two filenames at a time. This is very
        // slow but is safest and uses minimal RAM.
        char name1[LONG_FILENAME_LENGTH];

      #endif

      if (fileCnt > 1) {

        // Init sort order.
        for (uint16_t i = 0; i < fileCnt; i++) {
          sort_order[i] = (
            #if ENABLED(SDCARD_RATHERRECENTFIRST)
              fileCnt - 1 -
            #endif
          i);
          // If using RAM then read all filenames now.
          #if ENABLED(SDSORT_USES_RAM)
            selectFileByIndex(i);
            SET_SORTNAME(i);
            SET_SORTSHORT(i);
            // char out[30];
            // sprintf_P(out, PSTR("---- %i %s %s"), i, flag.filenameIsDir ? "D" : " ", sortnames[i]);
            // SERIAL_ECHOLN(out);
            #if HAS_FOLDER_SORTING
              const uint16_t bit = i & 0x07, ind = i >> 3;
              if (bit == 0) isDir[ind] = 0x00;
              if (flag.filenameIsDir) isDir[ind] |= _BV(bit);
            #endif
          #endif
        }

        // Bubble Sort
        for (uint16_t i = fileCnt; --i;) {
          bool didSwap = false;
          uint8_t o1 = sort_order[0];
          #if DISABLED(SDSORT_USES_RAM)
            selectFileByIndex(o1);              // Pre-fetch the first entry and save it
            strcpy(name1, longest_filename());  // so the loop only needs one fetch
            #if HAS_FOLDER_SORTING
              bool dir1 = flag.filenameIsDir;
            #endif
          #endif

          for (uint16_t j = 0; j < i; ++j) {
            const uint16_t o2 = sort_order[j + 1];

            // Compare names from the array or just the two buffered names
            #if ENABLED(SDSORT_USES_RAM)
              #define _SORT_CMP_NODIR() (strcasecmp(sortnames[o1], sortnames[o2]) > 0)
            #else
              #define _SORT_CMP_NODIR() (strcasecmp(name1, name2) > 0)
            #endif

            #if HAS_FOLDER_SORTING
              #if ENABLED(SDSORT_USES_RAM)
                // Folder sorting needs an index and bit to test for folder-ness.
                #define _SORT_CMP_DIR(fs) IS_DIR(o1) == IS_DIR(o2) ? _SORT_CMP_NODIR() : IS_DIR(fs > 0 ? o1 : o2)
              #else
                #define _SORT_CMP_DIR(fs) ((dir1 == flag.filenameIsDir) ? _SORT_CMP_NODIR() : (fs > 0 ? dir1 : !dir1))
              #endif
            #endif

            // The most economical method reads names as-needed
            // throughout the loop. Slow if there are many.
            #if DISABLED(SDSORT_USES_RAM)
              selectFileByIndex(o2);
              const bool dir2 = flag.filenameIsDir;
              char * const name2 = longest_filename(); // use the string in-place
            #endif // !SDSORT_USES_RAM

            // Sort the current pair according to settings.
            if (
              #if HAS_FOLDER_SORTING
                #if ENABLED(SDSORT_GCODE)
                  sort_folders ? _SORT_CMP_DIR(sort_folders) : _SORT_CMP_NODIR()
                #else
                  _SORT_CMP_DIR(FOLDER_SORTING)
                #endif
              #else
                _SORT_CMP_NODIR()
              #endif
            ) {
              // Reorder the index, indicate that sorting happened
              // Note that the next o1 will be the current o1. No new fetch needed.
              sort_order[j] = o2;
              sort_order[j + 1] = o1;
              didSwap = true;
            }
            else {
              // The next o1 is the current o2. No new fetch needed.
              o1 = o2;
              #if DISABLED(SDSORT_USES_RAM)
                #if HAS_FOLDER_SORTING
                  dir1 = dir2;
                #endif
                strcpy(name1, name2);
              #endif
            }
          }
          if (!didSwap) break;
        }
        // Using RAM but not keeping names around
        #if ENABLED(SDSORT_USES_RAM) && DISABLED(SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            for (uint16_t i = 0; i < fileCnt; ++i) free(sortnames[i]);
            #if HAS_FOLDER_SORTING
              free(isDir);
            #endif
          #endif
        #endif
      }
      else {
        sort_order[0] = 0;
        #if BOTH(SDSORT_USES_RAM, SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            sortnames = new char*[1];
            sortshort = new char*[1];
            isDir = new uint8_t[1];
          #endif
          selectFileByIndex(0);
          SET_SORTNAME(0);
          SET_SORTSHORT(0);
          isDir[0] = flag.filenameIsDir;
        #endif
      }

      sort_count = fileCnt;
    }
  }

  void CardReader::flush_presort() {
    if (sort_count > 0) {
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        delete sort_order;
        #if ENABLED(SDSORT_CACHE_NAMES)
          LOOP_L_N(i, sort_count) {
            free(sortshort[i]); // strdup
            free(sortnames[i]); // strdup
          }
          delete sortshort;
          delete sortnames;
        #endif
      #endif
      sort_count = 0;
    }
  }

#endif // SDCARD_SORT_ALPHA

uint16_t CardReader::get_num_Files() {
  return
    #if ENABLED(SDCARD_SORT_ALPHA) && SDSORT_USES_RAM && SDSORT_CACHE_NAMES
      nrFiles // no need to access the SD card for filenames
    #else
      countFilesInWorkDir()
    #endif
  ;
}


void CardReader::stop_sd_print(){
  card.endFilePrint(
    #if SD_RESORT
    true
    #endif
  );
  file.seekSet(0);
  for(int i=0;i<BUFSIZE;i++)queue.command_buffer[i][0] = 0;
  queue.clear();
  quickstop_stepper();
  print_job_timer.stop();
  wait_for_heatup = false;
  #if ENABLED(POWER_LOSS_RECOVERY)
    recovery.purge();
  #endif
  #ifdef EVENT_GCODE_SD_STOP
  queue.inject_P(PSTR(EVENT_GCODE_SD_STOP));
  #endif  

  TFT_DrawRectangle(/*x,y*/50,466, /*width, height*/619,18, /*round corner*/9,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/52,468, /*width, height*/615,14, /*round corner*/7,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_HOME,/*thikness in px*/8,/*fill?*/1,/*punktir?*/0);
    
}
//
// Return from procedure or close out the Print Job
//
void CardReader::fileHasFinished() {
  planner.synchronize();
  f_close(&FileFromSD);
  file.close();
  if (file_subcall_ctr > 0) { // Resume calling file after closing procedure
    file_subcall_ctr--;
    openFileRead(NULL,proc_filenames[file_subcall_ctr], 2); // 2 = Returning from sub-procedure
    setIndex(filespos[file_subcall_ctr]);
    startFileprint();
  }
  else {
    endFilePrint();

    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif
    file.seekSet(0);
    sdprinting_done_state = 1;
  }
}

#if ENABLED(AUTO_REPORT_SD_STATUS)
  uint8_t CardReader::auto_report_sd_interval = 0;
  millis_t CardReader::next_sd_report_ms;
  #if NUM_SERIAL > 1
    int8_t CardReader::auto_report_port;
  #endif

  void CardReader::auto_report_sd_status() {
    millis_t current_ms = millis();
    if (auto_report_sd_interval && ELAPSED(current_ms, next_sd_report_ms)) {
      next_sd_report_ms = current_ms + 1000UL * auto_report_sd_interval;
      PORT_REDIRECT(auto_report_port);
      report_status();
    }
  }
#endif // AUTO_REPORT_SD_STATUS

#if ENABLED(POWER_LOSS_RECOVERY)

  bool CardReader::jobRecoverFileExists() {
    // const bool exists = recovery.file.open(&root, recovery.filename, O_READ);
    FL_sel = 2;
    uint8_t byte_length[1];
    W25qxx_ReadByte(byte_length,0);
    const bool exists = (byte_length[0]!=0xFF);
    // if (exists) recovery.file.close();
    return exists;
  }

  void CardReader::openJobRecoveryFile(const bool read) {
    if (!isMounted()) return;
    if (recovery.file.isOpen()) return;
    if (!recovery.file.open(&root, recovery.filename, read ? O_READ : O_CREAT | O_WRITE | O_TRUNC | O_SYNC))
      SERIAL_ECHOLNPAIR(STR_SD_OPEN_FILE_FAIL, recovery.filename, ".");
    else if (!read)
      echo_write_to_file(recovery.filename);
  }

  // Removing the job recovery file currently requires closing
  // the file being printed, so during SD printing the file should
  // be zeroed and written instead of deleted.
  void CardReader::removeJobRecoveryFile() {
    if (jobRecoverFileExists()) {
      recovery.init();

      FL_sel = 2;
      W25qxx_EraseSector(0);
      // removeFile(recovery.filename);
      #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
        SERIAL_ECHOPGM("Power-loss file delete");
        serialprintPGM(jobRecoverFileExists() ? PSTR(" failed.\n") : PSTR("d.\n"));
      #endif
    }
  }

  void CardReader::renderFile() {
    // int sd_count = 0;
    // bool card_eof = card.eof();
    // if(FileFromSD.obj.objsize){
    //   char buf[MAX_CMD_SIZE];
    //   while(!card_eof) {

        int total_height = render.get_gcode_height();
        // const int16_t n = card.get();
        // card_eof = card.eof();
        // if (n < 0 && !card_eof) { SERIAL_ERROR_MSG(STR_SD_ERR_READ); continue; }
        // // res_move = f_lseek(&FileFromSD, i);
        // // res_read = f_read(&FileFromSD, readData, 1, &ByteRead);

        // // if(res_read != FR_OK || res_move != FR_OK){
        // //   SERIAL_ERROR_MSG(STR_SD_ERR_READ);
        // // }else{
        //     // const int16_t n = readData[0];
        //     const char sd_char = (char)n;
        //     const bool is_eol = ((sd_char) == '\n' || (sd_char) == '\r');
        //     // sd_count++;
        //     if(is_eol || card_eof){
        //       if (!is_eol && sd_count) ++sd_count;          // End of file with no newline
        //       if (!process_line_done(sd_input_state, buf, sd_count)) {
        //         _commit_command(false);
        //       }
                
        //       if (card_eof) card.fileHasFinished();         // Handle end of file reached
        //     }else{
        //       process_stream_char(sd_char, sd_input_state, buf, sd_count);
        //     }
        // // }
    //   }
    //   TestFuncForReference(10);
    // }
  }
#endif // POWER_LOSS_RECOVERY

#endif // SDSUPPORT
