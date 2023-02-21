/****************************************************************************
 * Title                 :   Json payload format
 * Filename              :   Json_payload_format.h
 * Author                :   Eros Andolfo
 * Origin Date           :   07/03/2022
 * Version               :   1.0.0
 * Compiler              :   xtensa32 2.50200.97
 * Target                :   ESP32WROOMSE
 * Notes                 :   This is the implementation of the document located at:
 * https://systel.atlassian.net/wiki/spaces/IOTG/pages/130744321/IoT+Json+payload+Format
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

#ifndef JSON_PAYLOAD_FORMAT_H
#define JSON_PAYLOAD_FORMAT_H

#include <ArduinoJson.h>

void status_message(void);
void alarms_message(uint16_t array[]);
void state_message(uint8_t flag_first_connection, uint8_t eth_connection);
void raw_data_message(int16_t array[]);
void recipe_data_message(int16_t array[]);
void machine_state_message(uint16_t array[], uint16_t array_commands[]);
void log_message(uint16_t array[]);
void physical_values_message(uint16_t array[]);
void send_report_message(uint16_t machineid, uint16_t reportid, tm timestart, tm time_end, uint32_t duration);

#endif