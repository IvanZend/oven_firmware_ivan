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

#include "../inc/MarlinConfig.h"

#if USE_BEEPER

  #include "circularqueue.h"


#define NOTE_0	 00 /* Memorial John Cage Tribute Note, 4:33 */
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978



  #define TONE_QUEUE_LENGTH 4

  /**
   * @brief Tone structure
   * @details Simple abstraction of a tone based on a duration and a frequency.
   */
  struct tone_t {
    uint16_t duration;
    uint16_t frequency;
  };

  /**
   * @brief Buzzer class
   */
  class Buzzer {
    public:

      typedef struct {
        tone_t   tone;
        uint32_t endtime;
      } state_t;

    private:
      static state_t state;

    protected:
      static CircularQueue<tone_t, TONE_QUEUE_LENGTH> buffer;

      /**
       * @brief Inverts the sate of a digital PIN
       * @details This will invert the current state of an digital IO pin.
       */
      FORCE_INLINE static void invert() { TOGGLE(BEEPER_PIN); }

      /**
       * @brief Turn off a digital PIN
       * @details Alias of digitalWrite(PIN, LOW) using FastIO
       */
      FORCE_INLINE static void off() { WRITE(BEEPER_PIN, LOW); }

      /**
       * @brief Turn on a digital PIN
       * @details Alias of digitalWrite(PIN, HIGH) using FastIO
       */
      FORCE_INLINE static void on() { WRITE(BEEPER_PIN, HIGH); }

      /**
       * @brief Resets the state of the class
       * @details Brings the class state to a known one.
       */
      static inline void reset() {
        off();
        state.endtime = 0;
      }

    public:
      /**
       * @brief Class constructor
       */
      Buzzer() {
        SET_OUTPUT(BEEPER_PIN);
        reset();
      }

      /**
       * @brief Add a tone to the queue
       * @details Adds a tone_t structure to the ring buffer, will block IO if the
       *          queue is full waiting for one slot to get available.
       *
       * @param duration Duration of the tone in milliseconds
       * @param frequency Frequency of the tone in hertz
       */
      static void tone(const uint16_t duration, const uint16_t frequency=0);

      /**
       * @brief Tick function
       * @details This function should be called at loop, it will take care of
       *          playing the tones in the queue.
       */
      static void tick();
  };


enum TuneId {
        TUNE_ERROR              = 0,
        TUNE_PRINT_DONE         = 1,
        TUNE_STARTUP            = 3,
        TUNE_MAKERBOT_TV        = 4,
        TUNE_BEETHOVEN_5TH      = 5,
        TUNE_FILAMENT_START     = 6,
	TUNE_PAUSE		= 7,
	TUNE_SAILFISH_STARTUP	= 8,
};

  // Provide a buzzer instance
  extern Buzzer buzzer;

  // Buzz directly via the BEEPER pin tone queue
  #define BUZZ(d,f) buzzer.tone(d, f)

#elif HAS_BUZZER

  // Buzz indirectly via the MarlinUI instance
  #define BUZZ(d,f) ui.buzz(d,f)

#else

  // No buzz capability
  #define BUZZ(d,f) NOOP

#endif
