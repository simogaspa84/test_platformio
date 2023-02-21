/****************************************************************************
 * Title                 :   Mqtt
 * Filename              :   mqtt_mng.h
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
/** @file  mqtt_mng.h
 *  @brief This is the header file mqtt_mng.h*
 *   This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef MQTT_MNG_H
#define MQTT_MNG_H

#include <stdint.h>

#define REFRESH_TIME 200
#define COMPARE_RESULT_OKAY 0
#define INCOMING_CAPACITY 400           // capacity of buffer for incoming message. TBD
#define PROGRAMS_INCOMING_CAPACITY 1000 // capacity of buffer for incoming programs. TBD
#define MAXIMUM_LINE_LENGHT 512
#define EOL_CHARACTER '\r'
#define COMMANDS_SUFFIX "commands"
#define COMMAND_SUFFIX
#define CONFIG_SUFFIX "config"
#define SET_OUTPUTS_CMD "set_output"
#define CHANGE_RECEIPE_PARAMS_COMMAND "changeRecipeParams"
#define CHANGE_RECEIPE_COMMAND "changeRecipe"
#define PROGRAMS_DOWNLOAD_CMD "program-upload" // It is an error, the right one is program-download

typedef struct Esp_Status
{
    uint16_t broker_connection : 1;      /* 0 not connected 1 connected */
    uint16_t wifi_ethernet_selected : 1; /* 0 wifi  1 ethernet*/
    uint16_t status_wifi_ethernet : 1;   /* 0 not working  1 working */
    uint16_t command_to_apply : 1;       /* 0 no command to apply 1 command to apply*/
    uint16_t receipe_to_apply : 1;       /* 0 no receipe to apply 1 receipw to apply*/
    uint16_t update_to_apply : 1;        /* 0 no update  to apply 1 update to apply*/
    uint16_t number_of_registers : 9;    /* numbers of registers for receipe/update*/
} EspStatus_t;

enum Commands_from_Cloud
{
    CHANGE_PARAM = 1,
    CHANGE_RECEIPE = 2
};

extern EspStatus_t Status_Iot;
extern uint8_t flag_receivef_command;
extern uint8_t result_eth_connection;

void setup_IOT_mqtt(void);
void loop_google_IOT_mqtt(void);

#endif