/*******************************************************************************
 * Title                 :   Modbus Managment
 * Filename              :   modbus_mng.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   31/01/2022
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
/** @file : modbus_mng.cpp
 *  @brief This module is created for handling the modbbus connection
 */

#include <ModbusRTU.h>
#include "modbus_mng.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "task_module.h"
#include "mqtt_mng.h"

#define U2TXD_OUT_IDX 198
#define UART_TXD_IDX(u) ((u == 0) ? U0TXD_OUT_IDX : ((u == 1) ? U1TXD_OUT_IDX : ((u == 2) ? U2TXD_OUT_IDX : 0)))

ModbusRTU mb;
tm time_test;
uint32_t counter_ticks_tx_enabled = 0;

const uint16_t DEFAULT_VALUE_HOLDING_REGISTER = 0xFFFF;
const uint32_t DEFAULT_BAUD_RATE_MODBUS = 38400;
static uint16_t array_alarms[NUM_REGISTERS_ALARMS] = {0};
static uint16_t array_state_machine[NUM_REGISTERS_MACHINE_STATE] = {0};
static uint16_t array_physic_value[NUM_REGISTERS_PHYSICAL_VALUES] = {0};
static uint16_t array_logs[NUM_REGISTERS_LOG] = {0};
static uint16_t array_commands_status[NUM_REGISTERS_COMMANDS_RECEIVED] = {0};

/**
 * @brief  Callback function for alarms to send data from modbus message to related queue
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_alarm(TRegister *reg, uint16_t val)
{

  if (reg->address.address == ALARMS)
  {
    array_alarms[0] = val;
  }

  if (reg->address.address == (ALARMS + ONE_REGISTER_OFFSET))
  {
    array_alarms[ONE_REGISTER_OFFSET] = val;
  }

  if (reg->address.address == (ALARMS + TWO_REGISTER_OFFSET))
  {
    array_alarms[TWO_REGISTER_OFFSET] = val;

    if (xQueueSend(alarms_queue_from_uart_to_wifi, &array_alarms, 0) == pdPASS)
    {
      memset(array_alarms, 0, NUM_REGISTERS_ALARMS);
    }
    else
    {
      Serial.println("Alarm queue is full!");
    }
  }

  return val;
}

/**
 * @brief  Callback function for events to send data from modbus message to related queue
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_events(TRegister *reg, uint16_t val)
{

  if (reg->address.address == EVENTS)
  {
    array_logs[0] = val;
  }

  if (reg->address.address == (EVENTS + ONE_REGISTER_OFFSET))
  {
    array_logs[ONE_REGISTER_OFFSET] = val;

    if (xQueueSend(logs_queue_from_uart_to_wifi, &array_logs, 0) == pdPASS)
    {
      memset(array_logs, 0, NUM_REGISTERS_LOG);
    }
    else
    {
      Serial.println("Logs queue is full!");
    }
  }

  return val;
}

/**
 * @brief  Callback function as knowledge that the commands has been applied in machine
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_command_received(TRegister *reg, uint16_t val)
{
  Serial.println("Commands written on machine");

  /*Cleaning the registers with the command once they are executed*/
  for (int j = 0; j < NUM_REGISTERS_COMMANDS; j++)
  {
    mb.Hreg(COMMANDS + j, 0xFF);
  }

  if (reg->address.address == COMMANDS_RECEIVED)
  {
    array_commands_status[0] = val;
  }

  if (reg->address.address == (COMMANDS_RECEIVED + ONE_REGISTER_OFFSET))
  {
    array_commands_status[ONE_REGISTER_OFFSET] = val;

    if (xQueueSend(commnads_status_queue_from_uart_to_wifi, &array_commands_status, 0) == pdPASS)
    {
      memset(array_commands_status, 0, NUM_REGISTERS_COMMANDS_RECEIVED);
    }
    else
    {
      Serial.println("Logs queue is full!");
    }
  }
}

/**
 * @brief  Callback function for the machine state to send data from modbus message to related queue
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_state_machine(TRegister *reg, uint16_t val)
{

  if (reg->address.address == MACHINE_STATE)
  {
    array_state_machine[0] = val;
  }
  if (reg->address.address == (MACHINE_STATE + ONE_REGISTER_OFFSET))
  {
    array_state_machine[ONE_REGISTER_OFFSET] = val;
  }
  if (reg->address.address == (MACHINE_STATE + TWO_REGISTER_OFFSET))
  {
    array_state_machine[TWO_REGISTER_OFFSET] = val;
  }
  if (reg->address.address == (MACHINE_STATE + THREE_REGISTER_OFFSET))
  {
    array_state_machine[THREE_REGISTER_OFFSET] = val;
  }
  if (reg->address.address == (MACHINE_STATE + FOUR_REGISTER_OFFSET))
  {
    array_state_machine[FOUR_REGISTER_OFFSET] = val;

    if (xQueueSend(state_queue_from_uart_to_wifi, &array_state_machine, 0) == pdPASS)
    {
      Serial.println("inserito!");
      memset(array_state_machine, 0, NUM_REGISTERS_MACHINE_STATE);
    }
    else
    {
      Serial.println("State machine queue is full!");
    }
    return val;
  }
}

/**
 * @brief  Callback function for the physical value to send data from modbus message to related queue
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_physic_value(TRegister *reg, uint16_t val)
{

  for (int j = 0; j < NUM_REGISTERS_PHYSICAL_VALUES; j++)
  {
    if (reg->address.address == (PHYSICAL_VALUE + j))
    {
      array_physic_value[j] = val;
    }

    if (reg->address.address == (PHYSICAL_VALUE + NUM_REGISTERS_PHYSICAL_VALUES - 1))
    {

      if (xQueueSend(physics_queue_from_uart_to_wifi, &array_physic_value, 0) == pdPASS)
      {
        memset(array_physic_value, 0, NUM_REGISTERS_PHYSICAL_VALUES);
      }
      else
      {
        Serial.println("Physics queue is full!");
      }
      return val;
    }
  }
}

/**
 * @brief  Conversion from EspStatus struct to uin16_t
 * @param  *status
 * @retval data
 */
uint16_t write_data_from_status(EspStatus_t *status)
{

  uint16_t data = 0x00;
  data = ((status->broker_connection & 0x01) | ((status->wifi_ethernet_selected & 0x01) << ONE_BIT_OFFSET) | ((status->status_wifi_ethernet & 0x01) << TWO_BIT_OFFSET) |
          ((status->command_to_apply & 0x01) << THREE_BIT_OFFSET) | ((status->receipe_to_apply & 0x01) << FOUR_BIT_OFFSET) | ((status->update_to_apply & 0x01) << FIVE_BIT_OFFSET) | (status->number_of_registers << SIX_BIT_OFFSET));

  return data;
}

/**
 * @brief  Callback function for the status of the iot board
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_status(TRegister *reg, uint16_t val)
{
  mb.Hreg(STATUS_ESP32, write_data_from_status(&Status_Iot));
  return val;
}

/**
 * @brief  Callback function for the status of the iot board
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_commands(TRegister *reg, uint16_t val)
{
  uint16_t commands_array[NUM_REGISTERS_COMMANDS];

  /*physical values topic transmitted only if modbus message is arrived*/
  if (xQueueReceive(commands_queue_from_uart_to_wifi, &commands_array, 0) == pdPASS)
  {
    for (int j = 0; j < NUM_REGISTERS_COMMANDS; j++)
    {
      Serial.println(commands_array[j]);
      mb.Hreg(COMMANDS + j, commands_array[j]);
    }

    return val;
  }
}

/**
 * @brief  Callback function for the status of the iot board
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_report_start(TRegister *reg, uint16_t val)
{

  if (reg->address.address == REPORT_START)
  {
    report_struct.machine_id = val;
  }

  if (reg->address.address == REPORT_START + ONE_REGISTER_OFFSET)
  {
    report_struct.report_id = val;

    /*computing current time*/
    getLocalTime(&time_test);

    report_struct.init_time = time_test; /*to change with the call to the function to compute the time*/

    Serial.println("inizio start");
  }

  return val;
}

/**
 * @brief  Function for computing the duration of the event
 * @param  tm init
 * @param  tm end
 * @retval val
 */
static uint16_t compute_duration_event(tm init, tm end)
{
  uint16_t time = 0;

  if (end.tm_min == init.tm_min)
  {
    time = end.tm_sec - init.tm_sec;
  }

  if (end.tm_min > init.tm_min)
  {
    time = (ONE_MINUTE_IN_SECONDS - init.tm_sec) + end.tm_sec;
  }

  /*check of the computation */
  if (time > TWO_MINUTES_IN_SECONDS)
  {
    time = 0;
  }

  Serial.println("durata evento");

  return time;
}

/**
 * @brief  Callback function for the status of the iot board
 * @param  reg the register
 * @param  val the value inside the register
 * @retval val
 */
static uint16_t call_report_stop(TRegister *reg, uint16_t val)
{

  if (reg->address.address == REPORT_STOP + ONE_REGISTER_OFFSET)
  {
    if (val == report_struct.report_id)
    {
      /* computing current time*/
      getLocalTime(&time_test);

      report_struct.end_time = time_test;

      report_struct.duration = compute_duration_event(report_struct.init_time, report_struct.end_time);

      Serial.println("inizio stop");

      if (xQueueSend(report_queue_from_uart_to_wifi, (void *)&report_struct, 0) == pdPASS)
      {
        /*cleaning the structure*/
        report_struct.duration = 0;
        report_struct.report_id = 0;
        report_struct.init_time.tm_min = 0;
        report_struct.init_time.tm_sec = 0;
        report_struct.init_time.tm_hour = 0;
        report_struct.end_time.tm_min = 0;
        report_struct.end_time.tm_sec = 0;
        report_struct.end_time.tm_hour = 0;
      }
    }
  }

  return val;
}

/**
 * @brief  Function to init the holding registers
 * @param  none
 * @retval none
 */
void Init_modbus_registers(void)
{

  /*Init of the serial port without using the tx pin*/
  Serial2.begin(DEFAULT_BAUD_RATE_MODBUS, SERIAL_8N1, RXD_PIN, TXD_PIN);

  mb.slave(SLAVE_ID);
  mb.begin(&Serial2);
  mb.setBaudrate(DEFAULT_BAUD_RATE_MODBUS);

  /*Creation of the holding registers*/
  mb.addHreg(ALARMS, 0, NUM_REGISTERS_ALARMS);
  mb.addHreg(EVENTS, 0, NUM_REGISTERS_LOG);
  mb.addHreg(MACHINE_STATE, 0, NUM_REGISTERS_MACHINE_STATE);
  mb.addHreg(PHYSICAL_VALUE, 0, NUM_REGISTERS_PHYSICAL_VALUES);
  mb.addHreg(REPORT_START, 0, NUM_REGISTERS_REPORT_START);
  mb.addHreg(REPORT_STOP, 0, NUM_REGISTERS_REPORT_STOP);
  mb.addHreg(COMMANDS, 0, NUM_REGISTERS_COMMANDS);
  mb.addHreg(COMMANDS_RECEIVED, 0, NUM_REGISTERS_COMMANDS_RECEIVED);
  mb.addHreg(STATUS_ESP32, 0, NUM_REGISTERS_IOT_STATUS);

  /*Init of the holding registers*/
  mb.Hreg(ALARMS, DEFAULT_VALUE_HOLDING_REGISTER);
  mb.Hreg(EVENTS, DEFAULT_VALUE_HOLDING_REGISTER);
  mb.Hreg(MACHINE_STATE, DEFAULT_VALUE_HOLDING_REGISTER);
  mb.Hreg(PHYSICAL_VALUE, DEFAULT_VALUE_HOLDING_REGISTER);
  mb.Hreg(REPORT_START, DEFAULT_VALUE_HOLDING_REGISTER);
  mb.Hreg(REPORT_STOP, DEFAULT_VALUE_HOLDING_REGISTER);

  for (uint8_t i = 0; i < NUM_REGISTERS_COMMANDS; i++)
  {
    mb.Hreg(COMMANDS + i, DEFAULT_VALUE_HOLDING_REGISTER);
  }

  mb.Hreg(STATUS_ESP32, DEFAULT_VALUE_HOLDING_REGISTER);
  mb.Hreg(COMMANDS_RECEIVED, DEFAULT_VALUE_HOLDING_REGISTER);

  /*callback on the write register action coming from the master*/
  mb.onSetHreg(ALARMS, call_alarm, NUM_REGISTERS_ALARMS);
  mb.onSetHreg(EVENTS, call_events, NUM_REGISTERS_LOG);
  mb.onSetHreg(MACHINE_STATE, call_state_machine, NUM_REGISTERS_MACHINE_STATE);
  mb.onSetHreg(PHYSICAL_VALUE, call_physic_value, NUM_REGISTERS_PHYSICAL_VALUES);
  mb.onSetHreg(REPORT_START, call_report_start, NUM_REGISTERS_REPORT_START);
  mb.onSetHreg(REPORT_STOP, call_report_stop, NUM_REGISTERS_REPORT_STOP);
  mb.onSetHreg(COMMANDS_RECEIVED, call_command_received, NUM_REGISTERS_COMMANDS_RECEIVED);
  mb.onGetHreg(STATUS_ESP32, call_status, NUM_REGISTERS_IOT_STATUS);
  mb.onGetHreg(COMMANDS, call_commands, NUM_REGISTERS_COMMANDS);
}

/**
 * @brief  Function to run the serial task with modbus messages handling
 * @param  none
 * @retval none
 */

void Serial_task(void)
{
  mb.task();
}
