/****************************************************************************
 * Title                 :   Modbus
 * Filename              :   modbus_mng.h
 * Author                :   Simone Gasparella
 * Origin Date           :   22/12/2021
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
 *  22/12/21  1.0.0   Simone Gasparella   Interface Created.
 *
 *****************************************************************************/
/** @file  modbus_mng.h
 *  @brief This is the header file modbus_mng.h*
 *   This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef MODBUS_MNG_H
#define MODBUS_MNG_H

#define TXD_PIN (GPIO_NUM_19)
#define RXD_PIN (GPIO_NUM_21)
#define RTS_PIN (GPIO_NUM_18)

#define SLAVE_ID 0x02

#define ALARMS 0x20
#define EVENTS 0x23
#define MACHINE_STATE 0x25
#define PHYSICAL_VALUE 0x30
#define REPORT_START 0x10
#define REPORT_STOP 0x12

#define STATUS_ESP32 0x10
#define COMMANDS 0x40
#define COMMANDS_RECEIVED 0x55
#define SETTINGS 0x60
#define UPDATE 0xB0

#define NUM_REGISTERS_ALARMS 3
#define NUM_REGISTERS_LOG 2
#define NUM_REGISTERS_MACHINE_STATE 5
#define NUM_REGISTERS_PHYSICAL_VALUES 11
#define NUM_REGISTERS_REPORT_START 2
#define NUM_REGISTERS_REPORT_STOP 2

#define NUM_REGISTERS_COMMANDS 12
#define NUM_REGISTERS_COMMANDS_RECEIVED 2
#define NUM_REGISTERS_IOT_STATUS 1
#define NUM_REGISTERS_SETTINGS 70
#define NUM_REGISTERS_UPDATE 100

#define FOUR_REGISTER_OFFSET 4
#define THREE_REGISTER_OFFSET 3
#define TWO_REGISTER_OFFSET 2
#define ONE_REGISTER_OFFSET 1

#define ONE_BIT_OFFSET 1
#define TWO_BIT_OFFSET 2
#define THREE_BIT_OFFSET 3
#define FOUR_BIT_OFFSET 4
#define FIVE_BIT_OFFSET 5
#define SIX_BIT_OFFSET 6

#define READ_HOLD_REGISTERS 0x03
#define NUM_TICKS_TX_ENABLED 30 /*more or less 30ms to be verified*/

#define ONE_MINUTE_IN_SECONDS 60
#define TWO_MINUTES_IN_SECONDS 120

void Init_modbus_registers(void);
void Serial_task(void);

#endif