/****************************************************************************
 * Title                 :   State machine
 * Filename              :   machine.cpp
 * Author                :   Eros Andolfo
 * Origin Date           :   07/03/2022
 * Version               :   1.0.0
 * Compiler              :   xtensa32 2.50200.97
 * Target                :   ESP32WROOMSE
 * Notes                 :   This is the state machine of the application
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
 *  07/03/22  1.0.0     Eros Andolfo     Interface Created.
 *
 *****************************************************************************/

#include <machine.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <task_module.h>
#include <Preferences.h>
#include "SPIFFS.h"
#include "captive_portal.h"

static int current_machine_state = INIT_STATE;
static int current_event = EVENT_NO_EVENT_ON_INIT;
static bool s_first_state_entry = true;
int machine_state = 0;
Preferences preferences_fota;
bool fota_update;
String current_fota_url;
String saved_fota_url;
String current_fota_command;
String saved_fota_command;

/**
 * @brief  Function to set the current state of the machine state
 * @param  None
 * @retval None
 */
int set_current_state(int current_event)
{
   switch (current_event)
   {
   case EVENT_OK_CONNECTION:
      machine_state = NORMAL_STATE;
      s_first_state_entry = true;
      break;
   case EVENT_TIMEOUT_WIFI:
      machine_state = INIT_STATE;
      s_first_state_entry = true;
      break;
   case EVENT_TOPIC_GO_IN_MAINTENANCE:
      machine_state = MAINTENANCE_STATE;
      s_first_state_entry = true;
      break;
   case EVENT_HTTP_REQUEST_GO_MAINTENANCE:
      machine_state = MAINTENANCE_STATE;
      s_first_state_entry = true;
      break;
   case EVENT_HTTP_REQUEST_GO_TO_NORMAL:
      machine_state = NORMAL_STATE;
      s_first_state_entry = true;
      break;
   case EVENT_HTTP_REQUEST_GO_TO_INIT:
      ESP.restart();
      break;
   case EVENT_GO_IN_FAULT:
      machine_state = FAULT_STATE;
      s_first_state_entry = true;
      break;
   default:
      // statements executed if expression does not equal
      // any case constant_expression
      break;
   }
   return machine_state;
}
/**
 * @brief  Function to set the current event
 * @param  None
 * @retval None
 */
void set_current_event(int event)
{
   current_event = event;
}
/**
 * @brief  Function to set the current fota url
 * @param  None
 * @retval None
 */
void set_current_fota_url(String fota_url)
{
   current_fota_url = fota_url;
}
/**
 * @brief  Function to set the current fota url
 * @param  None
 * @retval None
 */
void set_current_fota_command(String fota_command)
{
   current_fota_command = fota_command;
}
/**
 * @brief  Function to get the current fota url
 * @param  None
 * @retval None
 */
String get_current_fota_url(void)
{
   return saved_fota_url;
}
/**
 * @brief  Function to get the current fota command
 * @param  None
 * @retval None
 */
String get_current_fota_command(void)
{
   return saved_fota_command;
}

/**
 * @brief  Function to get the current event
 * @param  None
 * @retval None
 */
int get_current_event(void)
{
   return current_event;
}
/**
 * @brief  Function to get the current event
 * @param  None
 * @retval None
 */
int get_current_state(void)
{
   return current_machine_state;
}
/**
 * @brief  Function to execute the machine state
 * @param  None
 * @retval None
 */
void Handle_tasks_init_state(bool fota_update)
{
   vTaskResume(Gpioa_handle);

   if (fota_update == false)
   {
      vTaskSuspend(Fota_handle);
      vTaskResume(Mqtt_handle);
   }
   else
   {
      preferences_fota.putBool("fota", false);
      vTaskResume(Fota_handle);
      vTaskDelete(Mqtt_handle);
   }
}
/**
 * @brief  Function to execute the machine state
 * @param  None
 * @retval None
 */
void Handle_tasks_normal_state(void)
{
   vTaskResume(Mqtt_handle);
   vTaskResume(Webserver_handle);
   vTaskResume(Gpioa_handle);
   vTaskResume(Serial_handle);
   vTaskSuspend(Fota_handle);
}
/**
 * @brief  Function to execute the machine state
 * @param  None
 * @retval None
 */
void Handle_tasks_maintenance_state(void)
{
   vTaskSuspend(Mqtt_handle);
   vTaskSuspend(Serial_handle);
}
/**
 * @brief  Function to execute the machine state
 * @param  None
 * @retval None
 */
void Handle_tasks_fault_state(void)
{
   vTaskSuspend(Mqtt_handle);
   vTaskSuspend(Serial_handle);
   vTaskSuspend(Fota_handle);
   vTaskSuspend(Webserver_handle);
   vTaskResume(Gpioa_handle);
}

void machine_loop(void)
{
   current_event = get_current_event();

   current_machine_state = set_current_state(current_event);

   switch (current_machine_state)
   {
   case INIT_STATE:
      if (s_first_state_entry)
      {
         preferences_fota.begin("my-fota", false);
         fota_update = preferences_fota.getBool("fota");
         saved_fota_url = preferences_fota.getString("fota_url");
         saved_fota_command = preferences_fota.getString("fota_command");

         Serial.println("INIT STATE!");
         current_event = EVENT_NO_EVENT_ON_INIT;
         s_first_state_entry = false;
         Handle_tasks_init_state(fota_update);
      }
      // Do this stuff on every iteration

      break;
   case NORMAL_STATE:
      if (s_first_state_entry)
      {
         Serial.println("NORMAL_STATE!");
         Handle_tasks_normal_state();
         current_event = EVENT_NO_EVENT_ON_NORMAL;
         s_first_state_entry = false;
      }
      if (current_event == EVENT_RESTART_FOR_FOTA)
      {
         preferences_fota.putBool("fota", true);
         preferences_fota.putString("fota_url", current_fota_url);
         preferences_fota.putString("fota_command", current_fota_command);
         ESP.restart();
      }
      if (current_event == EVENT_HARD_RESET)
      {
         SPIFFS.remove("/embedded.txt");
         SPIFFS.remove("/rsa_key.pub");
         delete_preferences(1);
         ESP.restart();
      }

      // Do this stuff on every iteration
      break;

   case MAINTENANCE_STATE:
      if (s_first_state_entry)
      {
         Serial.println("MAINTENANCE_STATE!");
         Handle_tasks_maintenance_state();
         current_event = EVENT_NO_EVENT_ON_MAINTENANCE;
         s_first_state_entry = false;
      }
      // Do this stuff on every iteration

      break;
   case FAULT_STATE:
      if (s_first_state_entry)
      {
         // Only do this stuff once
         s_first_state_entry = false;
         Handle_tasks_fault_state();
      }
      // Do this stuff on every iteration
      break;
   default:
      break;
   }
}
