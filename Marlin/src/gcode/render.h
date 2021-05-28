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
#pragma once

/**
 * queue.h - The G-code command queue, which holds commands before they
 *           go to the parser and dispatcher.
 */

#include "../inc/MarlinConfig.h"

class Render {
public:
  /**
   * GCode line number handling. Hosts may include line numbers when sending
   * commands to Marlin, and lines will be checked for sequentiality.
   * M110 N<int> sets the current line number.
   */
  static long last_N;

  /**
   * GCode Command Queue
   * A simple ring buffer of BUFSIZE command strings.
   *
   * Commands are copied into this buffer by the command injectors
   * (immediate, serial, sd card) and they are processed sequentially by
   * the main loop. The gcode.process_next_command method parses the next
   * command and hands off execution to individual handler functions.
   */
  static uint8_t length,  // Count of commands in the queue
                 index_r; // Ring buffer read position

  static char command_buffer[BUFSIZE][MAX_CMD_SIZE];

  /*
   * The port that the command was received on
   */
  #if NUM_SERIAL > 1
    static int16_t port[BUFSIZE];
  #endif

  /**
   * Clear the Marlin command queue
   */
  static void clear();

  /**
   * Enqueue one or many commands to run from program memory.
   * Aborts the current queue, if any.
   * Note: process_injected_command() will process them.
   */
  static void inject_P(PGM_P const pgcode);

  /**
   * Enqueue and return only when commands are actually enqueued
   */
  static void enqueue_one_now(const char* cmd);

  /**
   * Attempt to enqueue a single G-code command
   * and return 'true' if successful.
   */
  static bool process_string(PGM_P const pgcode);

  /**
   * Enqueue from program memory and return only when commands are actually enqueued
   */
  static void enqueue_now_P(PGM_P const cmd);

  static bool process_line_done(uint8_t &sis, char (&buff)[MAX_CMD_SIZE], int &ind);
  static void render_char(const char c, uint8_t &sis, char (&buff)[MAX_CMD_SIZE], int &ind, int &nZ, int &nY, int &nX, int &num);
  static void getval_char(char * buf, int &Xval, int &Yval, int &Zval);
  static void render_line(int x1, int x2, int z, int layer);
  static void render_clone();
  static int char_check_num(char c);
  static int check_height_temp();
  static int char_get_numbers(uint32_t pos);
  static int get_val_after(PGM_P const pgcode, uint32_t pos);
  static int get_max_height();
  static int get_min_x(int &Xmin, int &Xmax);
  static int find_gcode_char(PGM_P const pgcode, int len, char g);
  static int get_gcode_height();
  static char get_char(uint32_t pos);

  static bool _enqueue(const char* cmd);

private:

  static uint8_t index_w;  // Ring buffer write position

  /**
   * Enqueue with Serial Echo
   * Return true on success
   */
  static bool enqueue_one(const char* cmd);


};

extern Render render;
