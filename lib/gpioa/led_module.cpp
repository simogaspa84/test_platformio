/*******************************************************************************
 * Title                 :   Led
 * Filename              :   led_module.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   29/09/2022
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
 *******************************************************************************/
/****************************************************************************
 * Doxygen C Template
 * Copyright (c) 2021 - Systel Elettronica - All Rights Reserved
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Template.
 *
 *****************************************************************************/
/*************** SOURCE REVISION LOG *****************************************
 *
 *    Date    Version   Author         Description
 *  29/09/22   1.0.0   Simone Gasparella   Initial Release.
 *
 *******************************************************************************/
/** @file : led_module.cpp
 *  @brief This module is created for handling the rgb leds of the board
 */

#include <Arduino.h>
#include "led_module.h"
#include "machine.h"

/**
 * @brief  Function to configure the led ports as outputs
 * @param  None
 * @retval None
 */
void setup_leds_outputs(void)
{
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);
}
/**
 * @brief  Function to turn off all the leds
 * @param  None
 * @retval None
 */
void reset_leds_outputs(void)
{
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_BLUE, LOW);
}
/**
 * @brief  Function to turn on the red led
 * @param  None
 * @retval None
 */
void turn_on_red_led(void)
{
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_BLUE, LOW);
}
/**
 * @brief  Function to turn off the red led
 * @param  None
 * @retval None
 */
void turn_off_red_led(void)
{
    digitalWrite(PIN_RED, LOW);
}
/**
 * @brief  Function to BLINK the red led
 * @param  None
 * @retval None
 */
void blink_red_led(void)
{
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_BLUE, LOW);
    delay(500);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
    delay(500);
}
/**
 * @brief  Function to turn on the green led
 * @param  None
 * @retval None
 */
void turn_on_green_led(void)
{
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
}

/**
 * @brief  Function to BLINK the green led
 * @param  None
 * @retval None
 */
void blink_green_led(void)
{
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
    delay(500);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
    delay(500);
}
/**
 * @brief  Function to turn off the green led
 * @param  None
 * @retval None
 */
void turn_off_green_led(void)
{
    digitalWrite(PIN_GREEN, LOW);
}
/**
 * @brief  Function to turn on the blu led
 * @param  None
 * @retval None
 */
void turn_on_blue_led(void)
{
    digitalWrite(PIN_BLUE, HIGH);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
}
/**
 * @brief  Function to turn off the blu led
 * @param  None
 * @retval None
 */
void turn_off_blue_led(void)
{
    digitalWrite(PIN_BLUE, LOW);
}

/**
 * @brief  Function to turn on the yellow led
 * @param  None
 * @retval None
 */
void turn_on_yellow_led(void)
{
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_BLUE, LOW);
}
/**
 * @brief  Function to blink  the yellow led
 * @param  None
 * @retval None
 */
void blink_yellow_led(void)
{
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, HIGH);
    digitalWrite(PIN_BLUE, LOW);
    delay(500);
    digitalWrite(PIN_GREEN, LOW);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_BLUE, LOW);
    delay(500);
}
/**
 * @brief  Function to turn off the yellow led
 * @param  None
 * @retval None
 */
void turn_off_yellow_led(void)
{
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
}
/**
 * @brief  Function to turn on ciano led
 * @param  None
 * @retval None
 */
void turn_on_ciano_led(void)
{
    digitalWrite(PIN_BLUE, HIGH);
    digitalWrite(PIN_GREEN, HIGH);
}
/**
 * @brief  Function to turn off ciano led
 * @param  None
 * @retval None
 */
void turn_off_ciano_led(void)
{
    digitalWrite(PIN_BLUE, LOW);
    digitalWrite(PIN_GREEN, LOW);
}
/**
 * @brief  Function to turn on the purple led
 * @param  None
 * @retval None
 */
void turn_on_purple_led(void)
{
    digitalWrite(PIN_BLUE, HIGH);
    digitalWrite(PIN_RED, HIGH);
}
/**
 * @brief  Function to turn off the purple led
 * @param  None
 * @retval None
 */
void turn_off_purple_led(void)
{
    digitalWrite(PIN_BLUE, LOW);
    digitalWrite(PIN_RED, LOW);
}
/**
 * @brief  Function to turn on the white led
 * @param  None
 * @retval None
 */
void turn_on_white_led(void)
{
    digitalWrite(PIN_BLUE, HIGH);
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, HIGH);
}
/**
 * @brief  Function to turn off the white led
 * @param  None
 * @retval None
 */
void turn_off_white_led(void)
{
    digitalWrite(PIN_BLUE, LOW);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
}
/**
 * @brief  Function to blink the white led
 * @param  None
 * @retval None
 */
void blink_white_led(void)
{
    digitalWrite(PIN_BLUE, LOW);
    digitalWrite(PIN_RED, LOW);
    digitalWrite(PIN_GREEN, LOW);
    delay(500);
    digitalWrite(PIN_BLUE, HIGH);
    digitalWrite(PIN_RED, HIGH);
    digitalWrite(PIN_GREEN, HIGH);
}
/**
 * @brief  Function to evaluate the colour of the led
 * @param  uint16_t delay_between_leds
 * @retval None
 */
void evaluate_led_colors(uint16_t delay_between_leds)
{
    reset_leds_outputs();
    turn_on_red_led();
    vTaskDelay(delay_between_leds);
    turn_on_blue_led();
    reset_leds_outputs();
    vTaskDelay(delay_between_leds);
    reset_leds_outputs();
    turn_on_green_led();
    vTaskDelay(delay_between_leds);
    reset_leds_outputs();
    turn_on_yellow_led();
    vTaskDelay(delay_between_leds);
    reset_leds_outputs();
    turn_on_ciano_led();
    vTaskDelay(delay_between_leds);
    reset_leds_outputs();
    turn_on_purple_led();
    vTaskDelay(delay_between_leds);
    reset_leds_outputs();
    turn_on_white_led();
}

/**
 * @brief  Function to handle the rgb leds in all states iot board
 * @param  None
 * @retval None
 */
void monitor_events_and_handle_leds(void)
{
    int current_machine_state = get_current_state();

    switch (current_machine_state)
    {
    case INIT_STATE:
        handle_events_led_init_state();
        break;
    case NORMAL_STATE:
        handle_events_led_normal_state();
        break;
    case MAINTENANCE_STATE:
        handle_events_led_maintenance_state();
        break;
    case FAULT_STATE:
        blink_red_led();
        break;
    default:
        break;
    }
}

/**
 * @brief  Function to handle the leds in the init state
 * @param  None
 * @retval None
 */
void handle_events_led_init_state(void)
{
    int current_event = get_current_event();

    switch (current_event)
    {
    case EVENT_NO_EVENT_ON_INIT:
        turn_on_blue_led();
        break;
    case EVENT_OK_CREDENTIALS:
        blink_green_led();
        break;
    case EVENT_FOTA_IN_PROGRESS:
        blink_white_led();
        break;
    default:
        break;
    }
}

/**
 * @brief  Function to handle the leds in the normal state
 * @param  None
 * @retval None
 */
void handle_events_led_normal_state(void)
{
    int current_event = get_current_event();

    switch (current_event)
    {
    case EVENT_NO_EVENT_ON_NORMAL:
        turn_on_green_led();
        break;
    case EVENT_TIMEOUT_WIFI:
        turn_on_yellow_led();
        break;
    case EVENT_TIMEOUT_ETHERNET:
        turn_on_yellow_led();
        break;
    case EVENT_TIMEOUT_CLOUD_CONNECTION:
        blink_yellow_led();
        break;
    default:
        break;
    }
}

/**
 * @brief  Function to handle the leds in the maintenance state
 * @param  None
 * @retval None
 */
void handle_events_led_maintenance_state(void)
{
    int current_event = get_current_event();

    switch (current_event)
    {
    case EVENT_NO_EVENT_ON_MAINTENANCE:
        turn_on_white_led();
        break;
    case EVENT_OTA_IN_PROGRESS:
        blink_white_led();
        break;

    default:
        break;
    }
}