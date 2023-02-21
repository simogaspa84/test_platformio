/*******************************************************************************
 * Title                 :   Gpioa
 * Filename              :   gpioa_module.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   19/01/2022
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
 *  19/01/22   1.0.0   Simone Gasparella   Initial Release.
 *
 *******************************************************************************/
/** @file : gpioa_module.cpp
 *  @brief This module is created for handling the inputs outputs and analog signal
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gpioa_module.h"
#include "driver/gpio.h"
#include <Arduino.h>
#include "led_module.h"
#include "button_module.h"

/**
 * @brief  Function to do the init of the gpio task
 * @param  None
 * @retval None
 */
void gpioa_init(void)
{
    /*setup leds as outputs*/
    setup_leds_outputs();
    /*setup button as inputs*/
    setup_user_button();
    /*reset outputs*/
    reset_leds_outputs();
}
/**
 * @brief  Function to execute the gpio task
 * @param  None
 * @retval None
 */
void gpio_task(void)
{
    monitor_user_button();
    monitor_events_and_handle_leds();
}