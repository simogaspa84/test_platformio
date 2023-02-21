/****************************************************************************
 * Title                 :   Task
 * Filename              :   task_module.h
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
/** @file  task_module.h
 *  @brief This is the header file task_module.h
 *  This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef TASK_MODULE_H
#define TASK_MODULE_H

#include <Arduino.h>

typedef struct report_t
{
    uint16_t machine_id;
    uint16_t report_id;
    tm init_time;
    tm end_time;
    uint16_t duration;
} report_t;

extern QueueHandle_t alarms_queue_from_uart_to_wifi;
extern QueueHandle_t logs_queue_from_uart_to_wifi;
extern QueueHandle_t state_queue_from_uart_to_wifi;
extern QueueHandle_t physics_queue_from_uart_to_wifi;
extern QueueHandle_t report_queue_from_uart_to_wifi;
extern QueueHandle_t commnads_status_queue_from_uart_to_wifi;

extern QueueHandle_t commands_queue_from_uart_to_wifi;
extern QueueHandle_t config_queue_from_uart_to_wifi;
extern QueueHandle_t program_upload_queue_from_uart_to_wifi;

extern TaskHandle_t Mqtt_handle;
extern TaskHandle_t Serial_handle;
extern TaskHandle_t Ethernet_handle;
extern TaskHandle_t Gpioa_handle;
extern TaskHandle_t Canbus_handle;
extern TaskHandle_t Bluetooth_handle;
extern TaskHandle_t Webserver_handle;
extern TaskHandle_t Fota_handle;

extern report_t report_struct;

void Create_queue_for_exchange_alarms_between_uart_and_wifi(void);
void Create_queue_for_exchange_log_between_uart_and_wifi(void);
void Create_queue_for_exchange_state_between_uart_and_wifi(void);
void Create_queue_for_exchange_physics_between_uart_and_wifi(void);
void Create_queue_for_exchange_reports_between_uart_and_wifi(void);
void Create_queue_for_exchange_commands_status_between_uart_and_wifi(void);

void Create_queue_for_exchange_commands_between_wifi_and_uart(void);
void Create_queue_for_exchange_config_between_wifi_and_uart(void);
void Create_queue_for_exchange_program_upload_between_wifi_and_uart(void);

void Mqtt_task(void *parameters);
void Communication_with_serial_task(void *parameters);
void Communication_with_ethernet_task(void *parameters);
void Input_Output_analog_task(void *parameters);
void Webserver_task(void *parameters);
void Fota_task(void *parameters);

void First_task_creation(void);
void Second_task_creation(void);
void Fourth_task_creation(void);
void Seventh_task_creation(void);
void Eight_task_creation(void);

#endif