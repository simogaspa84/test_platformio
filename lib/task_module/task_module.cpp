/*******************************************************************************
 * Title                 :   Task
 * Filename              :   task_module.cpp
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
/** @file : task_module.cpp
 *  @brief This module is created for handling the tasks
 */

#include "task_module.h"
#include <captive_portal.h>
#include <gpioa_module.h>
#include <modbus_mng.h>
#include <machine.h>
#include <webserver_mng.h>
#include <mqtt_mng.h>
#include <automatic_ota.h>

static const uint32_t STACK_SIZE_MQTT_TASK = 15000;      /*optimal value to be defined*/
static const uint32_t STACK_SIZE_SERIAL_TASK = 2000;     /*optimal value to be defined*/
static const uint32_t STACK_SIZE_GPIOA_TASK = 5000;      /*optimal value to be defined*/
static const uint32_t STACK_SIZE_WEBSERVER_TASK = 20000; /*optimal value to be defined*/
static const uint32_t STACK_SIZE_FOTA_TASK = 30000;      /*optimal value to be defined*/

static const uint16_t SERIAL_TASK_DELAY = 10;  /*optimal value to be defined*/
static const uint16_t MQTT_TASK_DELAY = 100;   /*optimal value to be defined*/
static const uint16_t GPIOA_TASK_DELAY = 1000; /*optimal value to be defined*/
static const uint32_t WEBSERVER_DELAY = 500;   /*500 is fine for running single webpage*/
static const uint32_t FOTA_DELAY = 1;          /*500 is fine for running single webpage*/

UBaseType_t Mqtt_priority = 1;      /*optimal value to be defined*/
UBaseType_t Serial_priority = 1;    /*optimal value to be defined*/
UBaseType_t Gpioa_priority = 1;     /*optimal value to be defined*/
UBaseType_t Webserver_priority = 1; /*optimal value to be defined*/
UBaseType_t Fota_priority = 1;      /*optimal value to be defined*/

TaskHandle_t Mqtt_handle = NULL;      /*for handling the task1 from outside the task*/
TaskHandle_t Serial_handle = NULL;    /*for handling the task2 from outside the task*/
TaskHandle_t Gpioa_handle = NULL;     /*for handling the task4 from outside the task*/
TaskHandle_t Webserver_handle = NULL; /*for handling the task4 from outside the task*/
TaskHandle_t Fota_handle = NULL;      /*for handling the task4 from outside the task*/

QueueHandle_t alarms_queue_from_uart_to_wifi;
QueueHandle_t logs_queue_from_uart_to_wifi;
QueueHandle_t state_queue_from_uart_to_wifi;
QueueHandle_t physics_queue_from_uart_to_wifi;
QueueHandle_t report_queue_from_uart_to_wifi;
QueueHandle_t commnads_status_queue_from_uart_to_wifi;

QueueHandle_t commands_queue_from_uart_to_wifi;
QueueHandle_t config_queue_from_uart_to_wifi;
QueueHandle_t program_upload_queue_from_uart_to_wifi;

UBaseType_t queue_lenght = 10;         /*optimal value to be defined*/
UBaseType_t queue_lenght_commands = 1; /*optimal value to be defined*/

report_t report_struct;

/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_alarms_between_uart_and_wifi(void)
{
  alarms_queue_from_uart_to_wifi = xQueueCreate(queue_lenght, NUM_REGISTERS_ALARMS * sizeof(uint16_t));
}
/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_log_between_uart_and_wifi(void)
{
  logs_queue_from_uart_to_wifi = xQueueCreate(queue_lenght, NUM_REGISTERS_LOG * sizeof(uint16_t));
}
/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_state_between_uart_and_wifi(void)
{
  state_queue_from_uart_to_wifi = xQueueCreate(queue_lenght, NUM_REGISTERS_MACHINE_STATE * sizeof(uint16_t));
}
/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_physics_between_uart_and_wifi(void)
{
  physics_queue_from_uart_to_wifi = xQueueCreate(queue_lenght, NUM_REGISTERS_PHYSICAL_VALUES * sizeof(uint16_t));
}
/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_reports_between_uart_and_wifi(void)
{
  report_queue_from_uart_to_wifi = xQueueCreate(1, sizeof(report_t));
}
/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_commands_between_wifi_and_uart(void)
{
  commands_queue_from_uart_to_wifi = xQueueCreate(queue_lenght_commands, NUM_REGISTERS_COMMANDS * sizeof(uint16_t));
}
/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_config_between_wifi_and_uart(void)
{
  config_queue_from_uart_to_wifi = xQueueCreate(queue_lenght, 1 * sizeof(uint16_t));
}
/**
 * @brief  Function to create the queue for exchanging data between Wifi and uart task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_program_upload_between_wifi_and_uart(void)
{
  program_upload_queue_from_uart_to_wifi = xQueueCreate(queue_lenght, 1 * sizeof(uint16_t));
}
/**
 * @brief  Function to create the queue for exchanging data between uart and wifi task
 * @param  None
 * @retval None
 */
void Create_queue_for_exchange_commands_status_between_uart_and_wifi(void)
{
  commnads_status_queue_from_uart_to_wifi = xQueueCreate(queue_lenght_commands, NUM_REGISTERS_COMMANDS_RECEIVED * sizeof(uint16_t));
}
/**
 * @brief  Function to execute the creation of the first task
 * @param  None
 * @retval None
 */
void First_task_creation(void)
{
  xTaskCreate(Mqtt_task, "Wifi", STACK_SIZE_MQTT_TASK, NULL, Mqtt_priority, &Mqtt_handle);
  /*Block the task from running*/
  vTaskSuspend(Mqtt_handle);
}
/**
 * @brief  Function to execute the creation of the second task
 * @param  None
 * @retval None
 */
void Second_task_creation(void)
{
  xTaskCreate(Communication_with_serial_task, "Uart", STACK_SIZE_SERIAL_TASK, NULL, Serial_priority, &Serial_handle);
  /*Block the task from running*/
  vTaskSuspend(Serial_handle);
}
/**
 * @brief  Function to execute the creation of the fifth task
 * @param  None
 * @retval None
 */
void Fourth_task_creation(void)
{
  xTaskCreate(Input_Output_analog_task, "GPIOA", STACK_SIZE_GPIOA_TASK, NULL, Gpioa_priority, &Gpioa_handle);
  /*Block the task from running*/
  vTaskSuspend(Gpioa_handle);
}
/**
 * @brief  Function to execute the creation of the sixth task
 * @param  None
 * @retval None
 */
void Seventh_task_creation(void)
{
  xTaskCreate(Webserver_task, "WEBSERVER", STACK_SIZE_WEBSERVER_TASK, NULL, Webserver_priority, &Webserver_handle);
  /*Block the task from running*/
  vTaskSuspend(Webserver_handle);
}
/**
 * @brief  Function to execute the creation of the sixth task
 * @param  None
 * @retval None
 */
void Eight_task_creation(void)
{
  xTaskCreate(Fota_task, "FOTA", STACK_SIZE_FOTA_TASK, NULL, Fota_priority, &Fota_handle);
  /*Block the task from running*/
  vTaskSuspend(Fota_handle);
}
/**
 * @brief  Function to execute the wifi task. Responsible for the captive portal and transmitting/receiving the data with Mqtt over wifi
 * @param  None
 * @retval None
 */
void Mqtt_task(void *parameters)
{
  int result_captive_portal_config = 0;

  /*start of the captive portal or wifi network connection*/
  result_captive_portal_config = setup_captive_and_wifi();

  if (result_captive_portal_config == 1)
  {
    set_current_event(EVENT_OK_CREDENTIALS);
    setup_IOT_mqtt();
    set_current_event(EVENT_OK_CONNECTION);
  }

  for (;;)
  {
    if (result_captive_portal_config == 1)
    {
      loop_google_IOT_mqtt();
    }
    vTaskDelay(MQTT_TASK_DELAY / portTICK_PERIOD_MS); /*tells at the scheduler to suspende the task for X ms*/
  }
}
/**
 * @brief  Function to execute the uart task. Responsible to communicate with external mcu with a specific frame protocol over uart
 * @param  None
 * @retval None
 */
void Communication_with_serial_task(void *parameters)
{
  Init_modbus_registers();

  for (;;)
  {
    Serial_task();

    vTaskDelay(SERIAL_TASK_DELAY / portTICK_PERIOD_MS);
  }
}
/**
 * @brief  Function to handle the Input Output and Analog signals directly connected with the board
 * @param  None
 * @retval None
 */
void Input_Output_analog_task(void *parameters)
{
  gpioa_init();

  for (;;)
  {

    gpio_task();

    vTaskDelay(GPIOA_TASK_DELAY / portTICK_PERIOD_MS);
  }
}
/**
 * @brief  Function to execute the webserver task (for OTA update and log)
 * @param  None
 * @retval None
 */
void Webserver_task(void *parameters)
{
  /*init of the webserver task with properties*/
  web_portal_init(result_eth_connection);

  uint8_t http_id_request_from_webserver = 0;

  for (;;)
  {
    /*webserver task is complex with wifi and simple with ethernet*/
    if (result_eth_connection == 1)
    {
      http_id_request_from_webserver = get_event_on_web_portal();
      set_current_state_on_web_portal(get_current_state());

      switch (http_id_request_from_webserver)
      {
      case HTTP_REQUEST_GO_TO_MAINTENANCE:
        set_current_event(EVENT_HTTP_REQUEST_GO_MAINTENANCE);
        break;
      case HTTP_REQUEST_GO_TO_NORMAL:
        set_current_event(EVENT_HTTP_REQUEST_GO_TO_NORMAL);
        break;
      case HTTP_REQUEST_GO_TO_INIT:
        set_current_event(EVENT_HTTP_REQUEST_GO_TO_INIT);
        break;
      default:
        // default statements
        break;
      }

      reset_event_on_web_portal();
    }
    else
    {
      web_portal_start_ethernet();
    }

    vTaskDelay(WEBSERVER_DELAY / portTICK_PERIOD_MS);
  }
}
/**
 * @brief  Function to handle the Input Output and Analog signals directly connected with the board
 * @param  None
 * @retval None
 */
void Fota_task(void *parameters)
{
  Setup_automatic_ota(get_current_fota_url(), get_current_fota_command());

  for (;;)
  {
    Loop_automatic_ota();
  }
}