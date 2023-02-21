/****************************************************************************
 * Title                 :   Json payload format
 * Filename              :   Json_payload_format.cpp
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

#include <Json_payload_format.h>
#include <Arduino.h>
#include <gpioa_module.h>
#include <cloud_connection.h>
#include <machine.h>
#include <mqtt_mng.h>
#include <AWS_Cloud_Iot_Core/aws.h>
#include <Ethernet.h>
#include "version.h"

#define TIME_BUFFER 100              // capacity of buffer for time message
#define STATUS_CAPACITY 400          // capacity of buffer for status message
#define ALARMS_CAPACITY 200          // capacity of buffer for alarms message
#define PHYSICAL_VALUE_CAPACITY 2000 // capacity of buffer for physical value message
#define STATE_CAPACITY 250           // capacity of buffer for state message
#define MACHINE_STATE_CAPACITY 200   // capacity of buffer for state message
#define LOG_CAPACITY 100             // capacity of buffer for log message
#define RAW_CAPACITY 250             // capacity of buffer for state message
#define REPORT_CAPACITY 250          // capacity of buffer for state message
#define MAX_NUM_RECEIPES 10
#define SCALING_FACTOR_FROM_FLOAT_TO_INT 10

String firmware_version = String(firmware_version_major) + "." + String(firmware_version_minor) + "." + String(firmware_version_patch);
String hardware_version = "0.0.1";

struct tm timeinfo;
char actual_time[TIME_BUFFER];
struct tm time_info_first_connction;
char time_first_connection[TIME_BUFFER];

/**
 * @brief function to publish the status message.
 * @param  none
 * @retval none
 */
void status_message(void)
{
    StaticJsonDocument<STATUS_CAPACITY> doc;
    static char buffer_json[STATUS_CAPACITY];

    doc["DI"][0] = IN_0;
    doc["DI"][1] = IN_1;
    doc["DI"][2] = IN_2;
    doc["DI"][3] = IN_3;
    doc["DO"][0] = 1;
    doc["DO"][1] = 1;
    doc["DO"][2] = 1;
    doc["DO"][3] = false;
    doc["AI"][0] = 5.0;
    doc["AI"][1] = 0;
    doc["AI"][2] = 0;
    doc["AO"][0] = 0;
    doc["AO"][1] = 0;
    doc["AO"][2] = 0;
    doc["temperatures"]["board"] = 3.0;
    doc["temperatures"]["external"] = 4.0;
    doc["machine"]["status"] = "normal";
    serializeJson(doc, buffer_json);
    publishTelemetry(buffer_json);
}
/**
 * @brief function to publish an alarm.
 * @param  array is the array with the information to send
 * @retval none
 */
void alarms_message(uint16_t array[])
{
    get_actual_time(&timeinfo);
    strftime(actual_time, sizeof(actual_time), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    StaticJsonDocument<ALARMS_CAPACITY> doc;
    static char buffer_json[ALARMS_CAPACITY];

    doc["machineId"] = (uint8_t)array[0]; /*to retrieve from modbus */
    doc["code"] = (uint8_t)array[1];

    if (array[1] == 0)
    {
        doc["description"] = "Communication not present";
    }

    if (array[1] == 1)
    {
        doc["description"] = "Emergency button pressed";
    }

    if (array[1] == 2)
    {
        doc["description"] = "Thermal protection pump engine";
    }

    if (array[1] == 3)
    {
        doc["description"] = "Lateral covers opened";
    }

    if (array[1] == 4)
    {
        doc["description"] = "Cover opened during working";
    }

    doc["severity"] = (uint8_t)array[2];
    doc["timestamp"] = actual_time;
    serializeJson(doc, buffer_json);
    publish_alarms(buffer_json);
}

/**
 * @brief function to publish the state message
 * @param  flag_first_connection
 * @param  eth_connection
 * @retval none
 */

void state_message(uint8_t flag_first_connection, uint8_t eth_connection)
{
    get_actual_time(&timeinfo);
    strftime(actual_time, sizeof(actual_time), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);

    if (flag_first_connection == 0)
    {
        get_actual_time(&time_info_first_connction);
        strftime(time_first_connection, sizeof(time_first_connection), "%Y-%m-%dT%H:%M:%SZ", &time_info_first_connction);
    }

    StaticJsonDocument<STATE_CAPACITY> doc;
    static char buffer_json[STATE_CAPACITY];
    doc["serial"] = get_aws_thing_name();
    doc["time"] = actual_time;
    doc["bootTimestamp"] = time_first_connection;
    doc["fw"] = firmware_version;
    doc["hw"] = hardware_version;

    /*change info according connection*/
    if (eth_connection == 1)
    {
        doc["wifiSsid"] = WiFi.SSID();
        doc["ethernet"] = 0;
        doc["ipAddress"] = WiFi.localIP().toString().c_str();
    }
    else
    {
        doc["wifiSsid"] = "Not active";
        doc["ethernet"] = 1;
        doc["ipAddress"] = Ethernet.localIP().toString().c_str();
    }
    doc["status"] = "NORMAL"; /*TO DO change with the current status of iot board*/
    doc["runningCmd"] = 0;

    serializeJson(doc, buffer_json);
    publish_state(buffer_json);
}

/**
 * @brief function to publish the raw data message from modbus
 * @param  array is the array with the information to send
 * @retval none
 */

void raw_data_message(int16_t array[])
{

    StaticJsonDocument<RAW_CAPACITY> doc;
    static char buffer_json[RAW_CAPACITY];

    doc["machineId"] = 0; /*to retrieve from modbus */
    doc["name"] = "modbus-raw";
    doc["dataId"] = 0; /*address of the modbus register*/
    doc["data"] = 0;   /*value of the modbus register*/

    serializeJson(doc, buffer_json);
    publish_raw_data(buffer_json);
}

/**
 * @brief function to publish the raw data message from modbus
 * @param  array
 * @retval none
 */

void recipe_data_message(int16_t array[])
{

    StaticJsonDocument<RAW_CAPACITY> doc;
    static char buffer_json[RAW_CAPACITY];

    doc["machineId"] = 0; /*to retrieve from modbus */
    doc["recipeName"] = "Recipe_number_one";
    doc["data"] = 0; /*value of the modbus registers? */

    serializeJson(doc, buffer_json);
    publish_recipe_data(buffer_json);
}

/**
 * @brief function to publish the machine state message
 * @param  array
 * @param  array_commands
 * @retval none
 */

void machine_state_message(uint16_t array[], uint16_t array_commands[])
{
    get_actual_time(&timeinfo);
    strftime(actual_time, sizeof(actual_time), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    StaticJsonDocument<MACHINE_STATE_CAPACITY> doc;
    static char buffer_json[MACHINE_STATE_CAPACITY];

    doc["serial"] = array[0];
    doc["machineId"] = (uint8_t)array[1];
    doc["fwVersion"] = (uint8_t)array[2];
    doc["hwVersion"] = (uint8_t)array[3];
    doc["idStatusCode"] = (uint8_t)array[4];

    if ((uint8_t)array[4] == 0)
    {
        doc["status"] = "Normal";
    }
    else
    {
        doc["status"] = "Alarm";
    }

    doc["bootTimestamp"] = time_first_connection;
    doc["timestamp"] = actual_time;

    doc["cmdStatus"][""] = "";

    serializeJson(doc, buffer_json);
    publish_machine_state(buffer_json);
}

/**
 * @brief function to publish a log.
 * @param array is the array with the information to send
 * @retval none
 */

void log_message(uint16_t array[])
{
    get_actual_time(&timeinfo);
    strftime(actual_time, sizeof(actual_time), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    StaticJsonDocument<LOG_CAPACITY> doc;
    static char buffer_json[LOG_CAPACITY];

    doc["machineId"] = (uint8_t)array[0];
    doc["code"] = array[1];
    doc["description"] = "Ricetta Numero";
    doc["timestamp"] = actual_time;

    serializeJson(doc, buffer_json);
    publish_log(buffer_json);
}
/**
 * @brief function to store the programs received from the server.
 * A lenght check for the first line is done.
 *
 * @param doc
 */

void programs_download(StaticJsonDocument<PROGRAMS_INCOMING_CAPACITY> doc)
{
    uint8_t lenght = 0;
    const char *programs_to_download = doc["params"]["program"];
    Serial.println("First line lenght is:");
    /*check for the EOL characther (\r)*/
    for (lenght = 0; programs_to_download[lenght] != EOL_CHARACTER; ++lenght)
        ;
    Serial.println(lenght);
}

/**
 * @brief function to send the data of the machine
 * @param array the array containg the info to send
 * @retval none
 */

void physical_values_message(uint16_t array[])
{
    get_actual_time(&timeinfo);
    strftime(actual_time, sizeof(actual_time), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    StaticJsonDocument<PHYSICAL_VALUE_CAPACITY> doc;
    static char buffer_json[PHYSICAL_VALUE_CAPACITY];

    char taglio[SCALING_FACTOR_FROM_FLOAT_TO_INT];
    char grammatura[SCALING_FACTOR_FROM_FLOAT_TO_INT];
    char pressatura[SCALING_FACTOR_FROM_FLOAT_TO_INT];

    sprintf(taglio, "%.1f", (float)array[1] / SCALING_FACTOR_FROM_FLOAT_TO_INT);
    sprintf(grammatura, "%.1f", (float)array[2] / SCALING_FACTOR_FROM_FLOAT_TO_INT);
    sprintf(pressatura, "%.1f", (float)array[3] / SCALING_FACTOR_FROM_FLOAT_TO_INT);

    doc["machineId"] = 0;
    doc["workingStatus"]["recipeId"] = (uint8_t)array[5];
    doc["workingStatus"]["tCut"] = taglio;
    doc["workingStatus"]["tGr"] = grammatura;
    doc["workingStatus"]["tPress"] = pressatura;
    doc["params"][""] = "";
    doc["timestamp"] = actual_time;
    serializeJson(doc, buffer_json);
    publish_machine_data(buffer_json);
}

/**
 * @brief function to send the report of an event
 * @param machineid id of the machine
 * @param timestart start time of the report
 * @param time_end  end time of the report
 * @param duration duration of the report
 * @retval none
 */

void send_report_message(uint16_t machineid, uint16_t reportid, tm timestart, tm time_end, uint32_t duration)
{
    StaticJsonDocument<REPORT_CAPACITY> doc;
    static char buffer_json[REPORT_CAPACITY];

    char init_report_time[TIME_BUFFER];
    char end_report_time[TIME_BUFFER];

    strftime(init_report_time, sizeof(init_report_time), "%Y-%m-%dT%H:%M:%SZ", &timestart);
    strftime(end_report_time, sizeof(end_report_time), "%Y-%m-%dT%H:%M:%SZ", &time_end);

    // doc["machineId"] = (uint8_t)machineid;
    doc["machineId"] = 0;

    if ((uint8_t)reportid == 0)
    {
        doc["type"] = "jobReport";
    }

    doc["data"]["recipeId"] = 1;
    doc["data"]["startTimestamp"] = init_report_time;
    doc["data"]["endTimestamp"] = end_report_time;
    doc["data"]["durationSec"] = duration;
    doc["timestamp"] = end_report_time;

    serializeJson(doc, buffer_json);
    publish_report_data(buffer_json);
}
