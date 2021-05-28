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

#if USE_BEEPER

#include "buzzer.h"
#include "../module/temperature.h"
#include "../core/mx_init.h"


#include "CircularBuffer.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

// Index for tone_data
#define FREQUENCY 0
#define DURATION  1

Buzzer::state_t Buzzer::state;
CircularQueue<tone_t, TONE_QUEUE_LENGTH> Buzzer::buffer;
Buzzer buzzer;

static bool soundEnabled = false;
static bool playing = false;

const static uint8_t TONE_QUEUE_SIZE = 20;

uint32_t tones_buf[TONE_QUEUE_SIZE];
CircularBuffer32 tones(TONE_QUEUE_SIZE, tones_buf);

struct tone {
	union {
		uint32_t tone_entry;	//Place holder for referencing
		uint16_t tone_data[2];	//index 0 is frequency, index 1 is duration
	};
};

//Tunes

const uint16_t tune_error[] PROGMEM = {
     NOTE_E5,    105,
     NOTE_0,     105,
     NOTE_B4,    105,
     NOTE_0,     105,
     NOTE_D4,    440,
     NOTE_0,       0, //Zero duration is a terminator
};

const uint16_t tune_startup[] PROGMEM = {
     NOTE_A7,    288,
     NOTE_0,     188,
     NOTE_CS8,   188,
     NOTE_0,	 188,
     NOTE_D8,    433,
     NOTE_0,       0, //Zero duration is a terminator
};

const uint16_t tune_print_done[] PROGMEM = {
     NOTE_A5,    188,
     NOTE_0,     188,
     NOTE_A6,    188,
     NOTE_0,     188,
     NOTE_B6,    188,
     NOTE_0,     188,
     NOTE_C7,    188,
     NOTE_CS7,   188,
     NOTE_0,     188,
     NOTE_A7,    333,
     NOTE_0,       0, //Zero duration is a terminator
};

const uint16_t tune_makerbot_tv[] PROGMEM = {
     NOTE_A7,    200,
     NOTE_0,      20,
     NOTE_A7,    200,
     NOTE_0,      20,
     NOTE_AS7,   400,
     NOTE_0,      20,
     NOTE_C8,    400,
     NOTE_0,      20,
     NOTE_A7,    800,
     NOTE_0,       0, //Zero duration is a terminator
};

const uint16_t tune_beethoven_5th[] PROGMEM = {
     NOTE_FS6,   200,
     NOTE_0,      20,
     NOTE_FS6,   200,
     NOTE_0,      20,
     NOTE_FS6,   200,
     NOTE_0,      20,
     NOTE_DS6,	1200,
     NOTE_0,      20,
     NOTE_F6,    200,
     NOTE_0,      20,
     NOTE_F6,    200,
     NOTE_0,      20,
     NOTE_F6,    200,
     NOTE_0,      20,
     NOTE_D6,   1200,
     NOTE_0,       0, //Zero duration is a terminator
};

const uint16_t tune_filament_start[] PROGMEM = {
     NOTE_A5,    400,
     NOTE_0,      20,
     NOTE_CS6,   200,
     NOTE_0,      20,
     NOTE_E6,    200,
     NOTE_0,      20,
     NOTE_A5,    400,
     NOTE_0,       0, //Zero duration is a terminator
};

const uint16_t tune_pause[] PROGMEM = {
     NOTE_A7,    200,
     NOTE_0,      20,
     NOTE_A7,    200,
     NOTE_0,      20,
     NOTE_C8,    200,
     NOTE_0,      20,
     NOTE_A7,    400,
     NOTE_0,       0, //Zero duration is a terminator
};

const uint16_t tune_sailfish_startup[] PROGMEM = {
     NOTE_C6,    200,
     NOTE_0,     220,
     NOTE_E6,    100,
     NOTE_0,     120,
     NOTE_F6,    200,
     NOTE_0,     220,
     NOTE_G6,    100,
     NOTE_0,     120,
     NOTE_A6,    200,
     NOTE_0,     220,
     NOTE_G6,    200,
     NOTE_0,     220,
     NOTE_G6,    200,
     NOTE_0,     220,
     NOTE_C7,    200,
     NOTE_0,       0, //Zero duration is a terminator
};

/**
 * @brief Add a tone to the queue
 * @details Adds a tone_t structure to the ring buffer, will block IO if the
 *          queue is full waiting for one slot to get available.
 *
 * @param duration Duration of the tone in milliseconds
 * @param frequency Frequency of the tone in hertz
 */
void Buzzer::tone(const uint16_t duration, const uint16_t frequency/*=0*/) {
  while (buffer.isFull()) {
    tick();
    thermalManager.manage_heater();
  }
  tone_t tone = { duration, frequency };
  buffer.enqueue(tone);
}

void Buzzer::tick() {
  const millis_t now = millis();

  if (!state.endtime) {
    if (buffer.isEmpty()) return;

    state.tone = buffer.dequeue();
    state.endtime = now + 10;//state.tone.duration;

     while(PENDING(millis(), state.endtime)){
          HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
          delay_us(200);
     }
     reset();
//     if (state.tone.frequency > 0) {
//       #if ENABLED(EXTENSIBLE_UI)
//         CRITICAL_SECTION_START();
//         ExtUI::onPlayTone(state.tone.frequency, state.tone.duration);
//         CRITICAL_SECTION_END();
//       #elif ENABLED(SPEAKER)
//         CRITICAL_SECTION_START();
//         ::tone(BEEPER_PIN, state.tone.frequency, state.tone.duration);
//         CRITICAL_SECTION_END();
//       #else
//         on();
//       #endif
//     }
  }
  else if (ELAPSED(now, state.endtime)) reset();
}

void reset(void) {
	// Reads the sound setting in from eeprom
	// soundEnabled = (bool)(eeprom::getEeprom8(eeprom_offsets::BUZZ_SETTINGS + buzz_eeprom_offsets::SOUND_ON,1) != 0);

	//Empty the queue
	tones.reset();

	// // Clear the timer, and we're not playing anymore
	// piezoTimeout.clear();
	// playing = false;

	// // We need to set the buzzer pin to output, so that the timer can drive it
	// BUZZER_PIN.setDirection(true);
	// BUZZER_PIN.setValue(false);

	// // And clear the timers
	// shutdown_timer();
}




void processNextTone(void) {
//      if ( tones.isEmpty() ) {
// 	  reset();
//      } 
//      else {
// 	  playing = true;

// 	  //Get the next tone from the buffer
// 	  struct tone tone;

// 	  tone.tone_entry = tones.pop();

// 	  //Schedule the end of tone.  Duration is in ms, timer is in uS, so
// 	  //we multiply by 1000
// 	  piezoTimeout.start((micros_t)tone.tone_data[DURATION] * (micros_t)1000);

// 	  if ( tone.tone_data[FREQUENCY] == NOTE_0 ) {
// 	       //Note 0 acts as a rest (i.e. doesn't play a note), so we shut the timer down
// 	       shutdown_timer();
// 	  } 
// 	  else {
// 	       uint32_t freq = (uint32_t)tone.tone_data[FREQUENCY];

// 	       // OCR0A for a given frequency can be calculated with:
// 	       // OCR0A = (F_CPU / (f * 2 * prescaler_factor)) - 1

// 	       // Pick the appropriate prescaler for the given frequency
// 	       uint8_t prescalerFactorBits;
// 	       uint8_t prescalerBits;

// #if BUZZER_TIMER == 4
// 	       //Setup the counter to count from 0 to OCR4A and toggle OC4A on counter reset
// 	       if ( freq >= 500) {
// 		    //Prescaler = 8 (1 << 3)
// 		    prescalerFactorBits	= 3;
// 		    prescalerBits       = _BV(CS41);
// 	       }
// 	       else {
// 		    //Prescaler = 64 (1 << 6)
// 		    prescalerFactorBits	= 6;
// 		    prescalerBits       = _BV(CS41) | _BV(CS40);
// 	       }

// 	       //Calculate the value for OCR4A, but save it in a variable
// 	       //to avoid contaminating the timer with a lengthy calculation
// 	       uint16_t outputCompareTop =
// 		    (uint16_t)(((uint32_t)F_CPU / ((freq << (uint32_t)1) << (uint32_t)prescalerFactorBits)) - 1);
//   #ifdef(BUZZER_SOFT_PWM)
//           TCCR4A = 0;				// Disconnect OC4A pin from Timer
//   #else
//           TCCR4A = _BV(COM4A0);			// Toggle OC4A pin on match; 1/2 freq of ORC4A
//   #endif
// 	       TCCR4B = prescalerBits |_BV(WGM42);	// Prescaler; CTC (top == OCR4A)
// 	       OCR4A  = (uint16_t)outputCompareTop;	// Frequency when compiled with prescaler
// 	       OCR4B  = 0x00;				// Not used
// 	       TCNT4  = 0x00;				// Clear the counter
//   #ifdef(BUZZER_SOFT_PWM)
//           TIMSK4 = _BV(OCIE4A);			// Enable interrupt
//   #else
//           TIMSK4 = 0x00;				// No interrupts
//   #endif
// #elif BUZZER_TIMER == 0
// 	       if ( freq >= 3906 ) {
// 		    //Prescaler = 8	(1 << 3)
// 		    prescalerFactorBits	= 3;
// 		    prescalerBits = _BV(CS01);
// 	       }
// 	       else if ( freq >= 488  ) {
// 		    //Prescaler = 64 (1 << 6)
// 		    prescalerFactorBits	= 6;
// 		    prescalerBits       = _BV(CS01) | _BV(CS00);
// 	       } else if ( freq >= 122  ) {
// 		    //Prescaler = 256 (1 << 8)
// 		    prescalerFactorBits	= 8;
// 		    prescalerBits       = _BV(CS02);
// 	       } else 			   {
// 		    //Prescaler = 1024 (1 << 10)
// 		    prescalerFactorBits	= 10;
// 		    prescalerBits       = _BV(CS02) | _BV(CS00);
// 	       }

// 	       //Calculate the value for OCR0A, but save it in a variable
// 	       //to avoid contaminating the timer with a lengthy calculation
// 	       uint8_t outputCompareTop =
// 		    (uint8_t)(((uint32_t)F_CPU / ((freq << (uint32_t)1) << (uint32_t)prescalerFactorBits)) - 1);

// 	       //Setup the counter to count from 0 to OCR0A and toggle OC0B on counter reset
// 	       TCCR0A =
//   #ifndef(BUZZER_SOFT_PWM)
//           _BV(COM0B0) |			// Toggle OC0B on match; 1/2 freq of ORC0A
//   #endif
// 							// because we're toggling)
// 		    _BV(WGM01);			       	// CTC (top == OCR0A)

// 	       TCCR0B = prescalerBits;			// Prescaler
// 	       OCR0A  = (uint8_t)outputCompareTop;	// Frequency when compiled with prescaler

// 	       OCR0B  = 0x00;				// Not used
// 	       TCNT0  = 0x00;				// Clear the counter
//   #ifdef(BUZZER_SOFT_PWM)
//           TIMSK0 = _BV(OCIE0A);			// Enable interrupt
//   #else
//           TIMSK0 = 0x00;				// No interrupts
//   #endif
// #else
//   #error missing piezo code
// #endif
// 	  }
//      }
}

void setTone(uint16_t frequency, uint16_t duration)
{
     //If sound is switched off, we do nothin
     if ( ! soundEnabled )
	  return;

     //Add the tone to the buffer
     if ( tones.getRemainingCapacity() ) {
	  struct tone tone;

	  tone.tone_data[FREQUENCY] = frequency;
	  tone.tone_data[DURATION]  = duration;

	  tones.push(tone.tone_entry);
     }

     //If we're not playing tones, then we schedule the tone we just put in the queue
     if ( ! playing )
	  processNextTone();
}
void playTune(uint8_t tuneid) {
     const prog_uint16_t *tunePtr = NULL;

     switch ( tuneid ) {
     case TUNE_ERROR:
	  tunePtr = tune_error;
	  break;
     case TUNE_PRINT_DONE:
	  tunePtr = tune_print_done;
	  break;
     case TUNE_STARTUP:
	  tunePtr = tune_startup;
	  break;
     case TUNE_MAKERBOT_TV:
	  tunePtr = tune_makerbot_tv;
	  break;
     case TUNE_BEETHOVEN_5TH:
	  tunePtr = tune_beethoven_5th;
	  break;
     case TUNE_FILAMENT_START:
	  tunePtr = tune_filament_start;
	  break;
     case TUNE_PAUSE:
	  tunePtr = tune_pause;
	  break;
     case TUNE_SAILFISH_STARTUP:
	  tunePtr = tune_sailfish_startup;
	  break;
     default:
	  break;
     }

     if ( tunePtr ) {
	  //Play the tune from progmem
	  uint8_t count = 0;
	  uint16_t note, duration;
	  do {
	       note     = pgm_read_word(tunePtr + count * 2);
	       duration	= pgm_read_word(tunePtr + count * 2 + 1);
	       if ( duration != 0 )
		    setTone(note, duration);
	       count++;
	  } while (duration != 0 ); 	//duration == 0 marks the end of the tune
     }
     else 
	  setTone(NOTE_B2, 500);	//Play this is the tuneid doesn't exist
}


#endif // USE_BEEPER
