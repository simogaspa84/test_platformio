/*******************************************************************************
 * Title                 :   Mqtt
 * Filename              :   mqtt_mng.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   22/12/2021
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
 *  22/12/21   1.0.0   Simone Gasparella   Initial Release.
 *
 *******************************************************************************/
/** @file : mqtt_mng.cpp
 *  @brief This module is created for the connection and interaction with the broker
 */

#include "mqtt_mng.h"
#include <Arduino.h>
#include <Json_payload_format.h>
#include "task_module.h"
#include <modbus_mng.h>
#include <AWS_Cloud_Iot_Core/aws.h>
#include <cloud_connection.h>

#define ANALOG_PIN_FOR_ENTROPY 36
#define MAX_SSL_SESSIONS 1
#define SSL_DEBUG_LEVEL SSLClient::DebugLevel::SSL_DUMP

uint32_t counter_ticks_wifi_task = 0;
static uint32_t refresh_esp32_status = 350; /*550 delay of wifi task more or less every 1 minute*/
bool first_config_received_flag = false;
EspStatus_t Status_Iot;
static uint8_t flag_first_time_connected_cloud = 0;
uint8_t flag_receivef_command = 0; /*variable flag for synch with commnads applied*/
report_t report_struct_receive;
uint8_t result_eth_connection = 0;

/**
 * @brief  Function to do the setup to connect the iot board to aws cloud with mqtt
 * @param  None
 * @retval None
 */
void setup_IOT_mqtt(void)
{
  Serial.begin(115200);

  /*connection with aws with ethernet cable*/
  result_eth_connection = setupCloudIoTETH();

  /*connection with aws with wifi*/
  if (result_eth_connection == 1)
  {
    setupCloudIotWIFI();
  }

  /*connection with aws with ethernet*/
  if (result_eth_connection == 0)
  {
    Compute_topics_names(get_aws_thing_name());
  }
}

/**
 * @brief  Function to wait for new messages from the broker
 * @param  None
 * @retval None
 */
void loop_google_IOT_mqtt(void)
{

  static uint16_t array_logs_from_queue[NUM_REGISTERS_LOG] = {0};
  static uint16_t array_alarms_from_queue[NUM_REGISTERS_ALARMS] = {0};
  static uint16_t machine_status_value_from_queue[NUM_REGISTERS_MACHINE_STATE] = {0};
  static uint16_t physical_value_from_queue[NUM_REGISTERS_PHYSICAL_VALUES] = {0};
  static uint16_t commands_received_from_queue[NUM_REGISTERS_COMMANDS_RECEIVED] = {0};

  if (result_eth_connection == 1)
  {
    /*wifi*/
    loop_aws_wifi();
  }
  else
  {
    /*ethernet*/
    loop_aws_ethernet();
  }

  if (first_config_received_flag == false)
  {
    ++counter_ticks_wifi_task;

    /*physical values topic transmitted only if modbus message is arrived*/
    if (xQueueReceive(physics_queue_from_uart_to_wifi, &physical_value_from_queue, 0) == pdPASS)
    {
      physical_values_message(physical_value_from_queue);
      Serial.println("Transmitted physics");

      delay(10);
    }

    /*alarm topic transmitted only if modbus message is arrived*/
    if (xQueueReceive(alarms_queue_from_uart_to_wifi, &array_alarms_from_queue, 0) == pdPASS)
    {
      alarms_message(array_alarms_from_queue);
      Serial.println("Transmitted alarm");

      delay(10);
    }
    /*log topic transmitted only if modbus message is arrived*/
    if (xQueueReceive(logs_queue_from_uart_to_wifi, &array_logs_from_queue, 0) == pdPASS)
    {
      log_message(array_logs_from_queue);
      Serial.println("Transmitted log");

      delay(10);
    }
    /*machine status topic transmitted only if modbus message of machine status is arrived*/
    if (xQueueReceive(state_queue_from_uart_to_wifi, &machine_status_value_from_queue, 0) == pdPASS)
    {
      machine_state_message(machine_status_value_from_queue, commands_received_from_queue);
      Serial.println("Transmitted machine status for changing in status");

      delay(10);
    }

    /*machine status topic transmitted only if modbus message of command status is arrived*/
    if (xQueueReceive(commnads_status_queue_from_uart_to_wifi, &commands_received_from_queue, 0) == pdPASS)
    {
      machine_state_message(machine_status_value_from_queue, commands_received_from_queue);
      Serial.println("Transmitted machine status for changing in command status");

      delay(10);
    }
    /*report topic transmitted only if modbus message is arrived*/
    if (xQueueReceive(report_queue_from_uart_to_wifi, (void *)&report_struct_receive, 0) == pdPASS)
    {
      send_report_message(report_struct_receive.machine_id, report_struct_receive.report_id, report_struct_receive.init_time,
                          report_struct_receive.end_time, report_struct_receive.duration);

      Serial.println("Transmitted report topic");

      delay(10);
    }

    /*iot status transmitted every refresh_esp32_status  time*/
    if (counter_ticks_wifi_task > refresh_esp32_status)
    {
      status_message();
      Serial.println("Transmitted iot board status");

      delay(10);

      /*iot state transmitted only once*/
      if ((flag_first_time_connected_cloud == 0) || (counter_ticks_wifi_task > refresh_esp32_status))
      {
        Serial.println("Transmitted iot board state");
        state_message(flag_first_time_connected_cloud, result_eth_connection);
        /*update variable of refresh and first connection*/
        flag_first_time_connected_cloud = 1;
        counter_ticks_wifi_task = 0;

        delay(10);
      }
    }
  }
}
