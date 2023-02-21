/****************************************************************************
 * Title                 :   Gpioa
 * Filename              :   gpioa_module.h
 * Author                :   Simone Gasparella
 * Origin Date           :   19/01/2022
 * Version               :   1.0.0
 * Compiler              :   xtensa32 2.50200.97
 * Target                :   ESP32WROOMSE
 * Notes                 :   None
 *
 * THIS SOFTWARE IS PROVIDED BY SYSTEL ELETTRONICA "AS IS" AND ANY EXPRESSED
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL SYSTEL ELETTRONICA OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
/****************************************************************************
* Doxygen C Template
* Copyright (c) 2021 - Systel Elettronica - All Rights Reserved

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Template.
*
*****************************************************************************/
/*************** INTERFACE CHANGE LIST **************************************
 *
 *    Date    Version   Author         Description
 *  19/01/22  1.0.0   Simone Gasparella   Interface Created.
 *
 *****************************************************************************/
/** @file  gpioa_module.h
 *  @brief This is the header file gpioa_module.h
 *  This is the header file for the definition TODO: MORE ABOUT ME!
 */

#include <stdlib.h>
#include <stdint.h>
/**
 * @brief Defines of digital outputs according to ESP-WROOM-32E datasheet
 * Note: for FE2107_0 board it is necessary to:
 * manually connect pin 4 with pin 12
 * connect OUT-1 to pin 26
 * connect IN-0 to pin 6
 */
#define HIGH_LEVEL 1
#define LOW_LEVEL 0

#define OUT_0_PIN GPIO_NUM_27        // see ESP32-WROOM datasheet
#define OUT_1_PIN GPIO_NUM_4         // see ESP32-WROOM datasheet
#define LED_ON_BOARD_PIN GPIO_NUM_23 // see ESP32-WROOM datasheet
#define OUT_MASK ((1ULL << OUT_0_PIN) | (1ULL << OUT_1_PIN) | (1ULL << LED_ON_BOARD_PIN))
#define OUT_0_ON gpio_set_level((gpio_num_t)OUT_0_PIN, HIGH_LEVEL)
#define OUT_0_OFF gpio_set_level((gpio_num_t)OUT_0_PIN, LOW_LEVEL)
#define OUT_1_ON gpio_set_level((gpio_num_t)OUT_1_PIN, HIGH_LEVEL)
#define OUT_1_OFF gpio_set_level((gpio_num_t)OUT_1_PIN, LOW_LEVEL)
#define LED_ON_BOARD_ON gpio_set_level((gpio_num_t)LED_ON_BOARD_PIN, HIGH_LEVEL)
#define LED_ON_BOARD_OFF gpio_set_level((gpio_num_t)LED_ON_BOARD_PIN, LOW_LEVEL)

/**
 * @brief defines of digital inputs according to ESP-WROOM-32E datasheet
 *
 */
#define INPUT_NOT_TO_BE_USED GPIO_NUM_36 // for FE2107_0 board only
#define IN_0_PIN GPIO_NUM_34
#define IN_3_PIN GPIO_NUM_22
#define INPUT_MASK ((1ULL << IN_0_PIN) | (1ULL << IN_0_PIN) | (1ULL << IN_0_PIN) | (1ULL << IN_3_PIN))
/**
 * @brief Inputs are inverted (opto-isolated).
 *
 */
#define IN_0 !gpio_get_level((gpio_num_t)IN_0_PIN)
#define IN_1 !gpio_get_level((gpio_num_t)IN_0_PIN)
#define IN_2 !gpio_get_level((gpio_num_t)IN_0_PIN)
#define IN_3 !gpio_get_level((gpio_num_t)IN_3_PIN)

/**
 * @brief defines for analog inputs
 *
 */
#define DEFAULT_VREF 1100 // Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 5   // Multisampling
#define SIXTEEN_BITS_OF_SHIFT 16
/**
 * @brief Board temperature sensor is a  MCP9700AT chip.
 * The conversion formula for board temperature is:
 * board_temp = ((MCP9700AT output value [mv]) - 500) / 10
 *
 */
#define MCP9700AT_OFFSET 500
#define TEMP_DIVIDER 10.00
#define TEN_TICKS_DELAY 10

/**
 * @brief External temp input is made for a PT100 thermoresistor.
 * range 0-3.3V --> 0-100 °C
 *
 */
#define MAX_VOLTAGE_VALUE 3.3          // Volts
#define TEMPERATURE_AT_MAX_VOLTAGE 100 // °C
#define DIVIDER_BY_1000 1000

/**
 * @brief Resistors value for FE2107_0:
 * R38 = 100K
 * R40 = 47K
 *
 */
#define ANALOG_IN_MULTIPLIER 147
#define ANALOG_IN_DIVIDER 47

void gpioa_init(void);
void led_on_board_toggle(void);
void gpio_task(void);