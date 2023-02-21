/****************************************************************************
 * Title                 :   Led
 * Filename              :   led_module.h
 * Author                :   Simone Gasparella
 * Origin Date           :   29/09/2022
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
 *  29/09/22  1.0.0   Simone Gasparella   Interface Created.
 *
 *****************************************************************************/
/** @file  led_module.h
 *  @brief This is the header file led_module.h
 *  This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef LED_MODULE_H
#define LED_MODULE_H

#define PIN_RED 5
#define PIN_GREEN 27
#define PIN_BLUE 4

void setup_leds_outputs(void);
void reset_leds_outputs(void);
void turn_on_red_led(void);
void turn_off_red_led(void);
void turn_on_green_led(void);
void turn_off_green_led(void);
void turn_on_blue_led(void);
void turn_off_blue_led(void);
void turn_on_yellow_led(void);
void turn_off_yellow_led(void);
void turn_on_ciano_led(void);
void turn_off_ciano_led(void);
void turn_on_purple_led(void);
void turn_off_purple_led(void);
void turn_on_white_led(void);
void turn_off_white_led(void);
void evaluate_led_colors(uint16_t delay_between_leds);
void monitor_events_and_handle_leds(void);
void handle_events_led_init_state(void);
void handle_events_led_normal_state();
void handle_events_led_maintenance_state();

#endif