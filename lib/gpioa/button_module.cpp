/*******************************************************************************
 * Title                 :   Button
 * Filename              :   button_module.cpp
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
/** @file : button_module.cpp
 *  @brief This module is created for handling the user button on the board
 */

#include <Arduino.h>
#include "button_module.h"
#include <captive_portal.h>

static int buttonState = 0;  // current state of the button
static int countPressed = 0; // the moment the button was pressed

/**
 * @brief  Function to configure the led ports as outputs
 * @param  None
 * @retval None
 */
void setup_user_button(void)
{
    pinMode(PIN_USER_BUTTON, INPUT);
}

/**
 * @brief  Function to return the state of the user button
 * @param  None
 * @retval pressed
 */
int red_user_button_state(void)
{
    int pressed = 0;
    pressed = digitalRead(PIN_USER_BUTTON);

    return pressed;
}

/**
 * @brief  Function to return the state of the user button
 * @param  None
 * @retval None
 */
void monitor_user_button(void)
{
    buttonState = red_user_button_state(); // read the button input

    updateState();
}
/**
 * @brief  Function to update the counter of the button
 * @param  None
 * @retval None
 */
void updateState(void)
{
    if (buttonState == HIGH)
    {
        ++countPressed;
    }
    else
    {
        if (countPressed > THRESHOLD_BUTTON_PRESSED)
        {
            /*delete information inseted in captive portal*/
            Serial.println("delete data");
            delete_preferences(1);
            restart_iot_board();
        }

        if ((countPressed < THRESHOLD_BUTTON_PRESSED) && (countPressed > 0))
        {
            restart_iot_board();
        }

        countPressed = 0;
    }
}
/**
 * @brief  Function to restart the iot board
 * @param  None
 * @retval None
 */
void restart_iot_board(void)
{
    ESP.restart();
}