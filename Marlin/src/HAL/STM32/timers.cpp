/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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

#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "HAL.h"

#include "timers.h"

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 3

#define __TIMER_DEV(X)  TIM##X
#define _TIMER_DEV(X)   __TIMER_DEV(X)
#define STEP_TIMER_DEV  _TIMER_DEV(STEP_TIMER)
#define TEMP_TIMER_DEV  _TIMER_DEV(TEMP_TIMER)
#define HEAT_TIMER_DEV  _TIMER_DEV(HEAT_TIMER)
#define PWM_TIMER_DEV   _TIMER_DEV(PWM_TIMER)

// ------------------------
// Private Variables
// ------------------------

HardwareTimer *timer_instance[NUM_HARDWARE_TIMERS] = { NULL };
bool timer_enabled[NUM_HARDWARE_TIMERS] = { false };

// ------------------------
// Public functions
// ------------------------

// frequency is in Hertz
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  if (!HAL_timer_initialized(timer_num)) {
    switch (timer_num) {
      case STEP_TIMER_NUM: // STEPPER TIMER - use a 32bit timer if possible
        timer_instance[timer_num] = new HardwareTimer(STEP_TIMER_DEV);
        /* Set the prescaler to the final desired value.
         * This will change the effective ISR callback frequency but when
         * HAL_timer_start(timer_num=0) is called in the core for the first time
         * the real frequency isn't important as long as, after boot, the ISR
         * gets called with the correct prescaler and count register. So here
         * we set the prescaler to the correct, final value and ignore the frequency
         * asked. We will call back the ISR in 1 second to start at full speed.
         *
         * The proper fix, however, would be a correct initialization OR a
         * HAL_timer_change(const uint8_t timer_num, const uint32_t frequency)
         * which changes the prescaler when an IRQ frequency change is needed
         * (for example when steppers are turned on)
         */
        timer_instance[timer_num]->setPrescaleFactor(STEPPER_TIMER_PRESCALE); //the -1 is done internally
        timer_instance[timer_num]->setOverflow(_MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (HAL_TIMER_RATE) / (STEPPER_TIMER_PRESCALE) /* /frequency */), TICK_FORMAT);
        break;
      case TEMP_TIMER_NUM: // TEMP TIMER - any available 16bit timer
        timer_instance[timer_num] = new HardwareTimer(TEMP_TIMER_DEV);
        // The prescale factor is computed automatically for HERTZ_FORMAT
        timer_instance[timer_num]->setOverflow(frequency, HERTZ_FORMAT);
        break;
      case HEAT_TIMER_NUM: // TEMP TIMER - any available 16bit timer
        timer_instance[timer_num] = new HardwareTimer(HEAT_TIMER_DEV);
        // The prescale factor is computed automatically for HERTZ_FORMAT
        timer_instance[timer_num]->setOverflow(frequency, MICROSEC_FORMAT);
        break;
      case PWM_TIMER_NUM: // TEMP TIMER - any available 16bit timer
        timer_instance[timer_num] = new HardwareTimer(PWM_TIMER_DEV);
        // The prescale factor is computed automatically for HERTZ_FORMAT
        timer_instance[timer_num]->setOverflow(frequency, HERTZ_FORMAT);
        break;
    }

    HAL_timer_enable_interrupt(timer_num);

    /*
     * Initializes (and unfortunately starts) the timer.
     * This is needed to set correct IRQ priority at the moment but causes
     * no harm since every call to HAL_timer_start() is actually followed by
     * a call to HAL_timer_enable_interrupt() which means that there isn't
     * a case in which you want the timer to run without a callback.
     */
    timer_instance[timer_num]->resume(); // First call to resume() MUST follow the attachInterrupt()

    // This is fixed in Arduino_Core_STM32 1.8.
    // These calls can be removed and replaced with
    // timer_instance[timer_num]->setInterruptPriority
    switch (timer_num) {
      case STEP_TIMER_NUM:
        HAL_NVIC_SetPriority(STEP_TIMER_IRQ_NAME, STEP_TIMER_IRQ_PRIO, 0);
        break;
      case TEMP_TIMER_NUM:
        HAL_NVIC_SetPriority(TEMP_TIMER_IRQ_NAME, TEMP_TIMER_IRQ_PRIO, 0);
        break;
      case HEAT_TIMER_NUM:
        HAL_NVIC_SetPriority(HEAT_TIMER_IRQ_NAME, HEAT_TIMER_IRQ_PRIO, 0);
        break;
      case PWM_TIMER_NUM:
        HAL_NVIC_SetPriority(PWM_TIMER_IRQ_NAME, PWM_TIMER_IRQ_PRIO, 0);
        break;
    }
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  if (HAL_timer_initialized(timer_num) && !timer_enabled[timer_num]) {
    timer_enabled[timer_num] = true;
    switch (timer_num) {
      case STEP_TIMER_NUM:
      timer_instance[timer_num]->attachInterrupt(Step_Handler);
      break;
    case TEMP_TIMER_NUM:
      timer_instance[timer_num]->attachInterrupt(Temp_Handler);
      break;    
    case HEAT_TIMER_NUM:
      timer_instance[timer_num]->attachInterrupt(Heat_Handler);
      break;
    case PWM_TIMER_NUM:
      timer_instance[timer_num]->attachInterrupt(Pwm_Handler);
      break;
    }
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  if (HAL_timer_interrupt_enabled(timer_num)) {
    timer_instance[timer_num]->detachInterrupt();
    timer_enabled[timer_num] = false;
  }
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  return HAL_timer_initialized(timer_num) && timer_enabled[timer_num];
}

// Only for use within the HAL
TIM_TypeDef * HAL_timer_device(const uint8_t timer_num) {
  switch (timer_num) {
    case STEP_TIMER_NUM: return STEP_TIMER_DEV;
    case TEMP_TIMER_NUM: return TEMP_TIMER_DEV;
    case HEAT_TIMER_NUM: return HEAT_TIMER_DEV;
    case PWM_TIMER_NUM:  return PWM_TIMER_DEV;
  }
  return nullptr;
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
