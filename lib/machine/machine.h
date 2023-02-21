/****************************************************************************
 * Title                 :   State machine
 * Filename              :   machine.h
 * Author                :   Eros Andolfo
 * Origin Date           :   07/03/2022
 * Version               :   1.0.0
 * Compiler              :   xtensa32 2.50200.97
 * Target                :   ESP32WROOMSE
 * Notes                 :   This is the implementation of the state machine
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

#ifndef MACHINE_H
#define MACHINE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>
#include <time.h>

struct ERROR_CODE
{
    uint8_t code;
    const char *description;
    uint8_t severity;
};

typedef enum
{
    INIT_STATE,
    NORMAL_STATE,
    MAINTENANCE_STATE,
    FAULT_STATE
} states_machine_state;

typedef enum
{
    EVENT_NO_EVENT_ON_INIT,
    EVENT_OK_CREDENTIALS,
    EVENT_OK_CONNECTION,
    EVENT_FOTA_IN_PROGRESS,
    EVENT_GO_IN_FAULT
} events_init_state;

typedef enum
{
    EVENT_NO_EVENT_ON_NORMAL = 5,
    EVENT_TIMEOUT_WIFI,
    EVENT_TIMEOUT_ETHERNET,
    EVENT_TIMEOUT_CLOUD_CONNECTION,
    EVENT_TOPIC_GO_IN_MAINTENANCE,
    EVENT_HTTP_REQUEST_GO_MAINTENANCE,
    EVENT_RESTART_FOR_FOTA,
    EVENT_HARD_RESET
} events_normal_state;

typedef enum
{
    EVENT_NO_EVENT_ON_MAINTENANCE = 13,
    EVENT_HTTP_REQUEST_GO_TO_NORMAL,
    EVENT_HTTP_REQUEST_GO_TO_INIT,
    EVENT_OTA_IN_PROGRESS
} events_maintenance_state;

typedef enum
{
    EVENT_NO_EVENT_ON_FAULT,
    EVENT_TBD1,
    EVENT_TBD2
} events_fault_state;

void machine_loop(void);
int set_current_state(int current_event);
void set_current_event(int event);
int get_current_event(void);
int get_current_state(void);
void set_current_fota_url(String fota_url);
String get_current_fota_url(void);
void set_current_fota_command(String fota_command);
String get_current_fota_command(void);

#endif