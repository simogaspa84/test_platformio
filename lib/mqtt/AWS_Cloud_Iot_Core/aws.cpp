/*******************************************************************************
 * Title                 :   AWS
 * Filename              :   aws.cpp
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
/** @file : aws.cpp
 *  @brief This is the main file of the project
 */

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPIFFS.h>
#include "mqtt_mng.h"
#include "task_module.h"
#include "aws.h"
#include "json_parser.h"
#include "ethernet_aws.h"
#include "cloud_connection.h"
#include "machine.h"

using namespace std;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client_aws(net);

static uint16_t commands_array[NUM_REGISTERS_COMMANDS] = {0};

/* Configuration for NTP */
const char *ntp_primary_aws = "pool.ntp.org";
const char *ntp_secondary_aws = "time.nist.gov";
const int valid_utc_date_aws = 1645698852;

/* Strings with info about the cloud connection*/
String AWS_THING_NAME_FROM_FILE;
String AWS_ENDPOINT_FROM_FILE;
String AWS_CERT_CRT_FROM_FILE;
String AWS_CERT_PRIVATE_FROM_FILE;
int AWS_CERT_SIZE_FROM_FILE;
int AWS_KEY_SIZE_FROM_FILE;

/* Strings with the lisy of topics */
String AWS_IOT_PUBLISH_TOPIC_BUCKET;
String AWS_IOT_PUBLISH_TOPIC_TELEMETRY;
String AWS_IOT_PUBLISH_TOPIC_STATUS;
String AWS_IOT_PUBLISH_TOPIC_ALARMS;
String AWS_IOT_PUBLISH_TOPIC_STATE;
String AWS_IOT_PUBLISH_TOPIC_RAW;
String AWS_IOT_PUBLISH_TOPIC_RECIPE;
String AWS_IOT_PUBLISH_TOPIC_REPORTS;
String AWS_IOT_PUBLISH_TOPIC_MACHINE_STATE;
String AWS_IOT_PUBLISH_TOPIC_LOG;
String AWS_IOT_PUBLISH_TOPIC_MACHINE_DATA;
String AWS_IOT_SUBSCRIBE_TOPIC_COMMANDS;
String AWS_IOT_SUBSCRIBE_TOPIC_CONFIG;

/**
 * @brief  Function for parsing the messanges coming from aws
 * @param  topic
 * @param  payload
 * @param  length
 * @retval none
 */
void messageHandler(char *topic, byte *payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
  String strPayload = String((char *)payload);
  Serial.println(strPayload);

  String first_filter_string = "changeRecipeParams";
  int result_find = strPayload.indexOf(first_filter_string);

  /*Check if i receive the change recipe param command and parsing*/
  if (result_find != -1)
  {
    /*parsing command values*/
    commands_array[0] = Parse_paylod_for_getting_machine_id(strPayload);
    commands_array[1] = CHANGE_PARAM; /* id of the command*/
    commands_array[2] = 0xFFFF;       /*receipe not valid it has to modify the current receipe*/
    commands_array[3] = Parse_paylod_for_getting_valore_pressata(strPayload);
    commands_array[4] = Parse_paylod_for_getting_valore_taglio(strPayload);
    commands_array[5] = Parse_paylod_for_getting_valore_grammatura(strPayload);

    /*Insert commands commands in the queue*/
    if (xQueueSend(commands_queue_from_uart_to_wifi, &commands_array, 0) == pdPASS)
    {
      Serial.println("command element inserted in the queue!");
      flag_receivef_command = 1;
      memset(commands_array, 0, NUM_REGISTERS_COMMANDS);
    }
    else
    {
      Serial.println("command queue is full!");
    }
  }

  first_filter_string = "changeRecipe";
  result_find = strPayload.indexOf(first_filter_string);

  /*Check if i receive the change recipe command and parsing*/
  if (result_find != -1)
  {
    /*parsing command values*/
    commands_array[0] = Parse_paylod_for_getting_machine_id(strPayload);
    commands_array[1] = CHANGE_RECEIPE; /* id of the command*/
    commands_array[2] = Parse_paylod_for_getting_number_receipe(strPayload);

    /*Insert commands commands in the queue*/
    if (xQueueSend(commands_queue_from_uart_to_wifi, &commands_array, 0) == pdPASS)
    {
      Serial.println("command element inserted in the queue!");
      flag_receivef_command = 1;
      memset(commands_array, 0, NUM_REGISTERS_COMMANDS);
    }
    else
    {
      Serial.println("command queue is full!");
    }
  }

  /*checking for receiving the command for update fw with fota*/
  if (Parse_paylod_for_getting_update_command(strPayload))
  {
    set_current_fota_url(get_url_from_web());
    set_current_fota_command(get_fota_command_from_web());
    set_current_event(EVENT_RESTART_FOR_FOTA);
  }
  /*checking for receiving the command for update fw with fota*/
  if (Parse_paylod_for_getting_hardreset_command(strPayload))
  {
    set_current_event(EVENT_HARD_RESET);
  }
}
/**
 * @brief  Function to load the info for aws connection from txt file
 * @param  None
 * @retval None
 */
void Load_secrets_from_memory(void)
{
  /*filters for the text file*/
  const String filter_thingname = "THINGNAME";
  const String filter_endpoint = "AWS_ENDPOINT";
  const String filter_certificate = "AWS_CERT_CRT";
  const String filter_start_certificate_iot = "-----BEGIN CERTIFICATE-----";
  const String filter_end_certificate_iot = "AWS_CERT_PRIVATE";
  const String filter_keys_start = "-----BEGIN RSA PRIVATE KEY-----";
  const String filter_keys_end = "-----END RSA PRIVATE KEY-----\n";
  const String filter_cert_size = "CERT_SIZE";
  const String filter_key_size = "KEY_SIZE";

  String parsed_txt_file_with_endline_termintor;
  String parsed_txt_file_without_endline_termintor;

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open("/embedded.txt");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
  }

  /*if file embedded size is 0 this means that has been deleted*/
  if (file.size() == 0)
  {
    set_current_event(EVENT_GO_IN_FAULT);
  }

  while (file.available())
  {
    parsed_txt_file_with_endline_termintor.concat(file.readString());
  }

  file.close();

  file = SPIFFS.open("/embedded.txt");

  while (file.available())
  {
    parsed_txt_file_without_endline_termintor.concat(file.readStringUntil('\n'));
  }

  file.close();

  AWS_THING_NAME_FROM_FILE = parsed_txt_file_without_endline_termintor.substring(parsed_txt_file_without_endline_termintor.indexOf(filter_thingname), parsed_txt_file_without_endline_termintor.indexOf(filter_endpoint));
  AWS_THING_NAME_FROM_FILE = AWS_THING_NAME_FROM_FILE.substring(AWS_THING_NAME_FROM_FILE.indexOf(':') + 2);

  AWS_ENDPOINT_FROM_FILE = parsed_txt_file_without_endline_termintor.substring(parsed_txt_file_without_endline_termintor.indexOf(filter_endpoint), parsed_txt_file_without_endline_termintor.indexOf(filter_certificate));
  AWS_ENDPOINT_FROM_FILE = AWS_ENDPOINT_FROM_FILE.substring(AWS_ENDPOINT_FROM_FILE.indexOf(':') + 2);

  AWS_CERT_CRT_FROM_FILE = parsed_txt_file_with_endline_termintor.substring(parsed_txt_file_with_endline_termintor.indexOf(filter_start_certificate_iot), parsed_txt_file_with_endline_termintor.indexOf(filter_end_certificate_iot) - 1);
  AWS_CERT_PRIVATE_FROM_FILE = parsed_txt_file_with_endline_termintor.substring(parsed_txt_file_with_endline_termintor.indexOf(filter_keys_start));

  AWS_CERT_SIZE_FROM_FILE = AWS_CERT_CRT_FROM_FILE.length() + 1;
  AWS_KEY_SIZE_FROM_FILE = AWS_CERT_PRIVATE_FROM_FILE.length() + 2;
}
/**
 * @brief  Function to execute the connection with aws cloud
 * @param  None
 * @retval None
 */
void connectAWS(void)
{
  /*retrieve wifi credentials and start wifi*/
  Retrieve_saved_credentials_and_startwifi();

  /*retrieve security info for aws from memory*/
  Load_secrets_from_memory();

  /*compute topics*/
  Compute_topics_names(AWS_THING_NAME_FROM_FILE);

  /*Getting the current time from the server*/
  configTime(0, 0, ntp_primary_aws, ntp_secondary_aws);
  Serial.println("Waiting on time sync...");

  /*waiting until i don't get a valid date*/
  while (time(nullptr) < valid_utc_date_aws)
  {
    delay(10);
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT_FROM_FILE.c_str());
  net.setPrivateKey(AWS_CERT_PRIVATE_FROM_FILE.c_str());

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client_aws.setServer(AWS_ENDPOINT_FROM_FILE.c_str(), PORT_NUMBER);

  // Create a message handler
  client_aws.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while (!client_aws.connect(AWS_THING_NAME_FROM_FILE.c_str()))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client_aws.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client_aws.subscribe(AWS_IOT_SUBSCRIBE_TOPIC_COMMANDS.c_str());
  client_aws.subscribe(AWS_IOT_SUBSCRIBE_TOPIC_CONFIG.c_str());

  Serial.println("AWS IoT Connected!");
}

/**
 * @brief  Function to execute the setup generated by the arduino framework
 * @param  None
 * @retval None
 */
void setupCloudIotWIFI(void)
{
  Serial.begin(115200);
  connectAWS();
}
/**
 * @brief  Function to execute in loop generated by the arduino framework
 * @param  None
 * @retval None
 */
void loop_aws_wifi(void)
{

  bool result_connection_to_cloud = false;

  result_connection_to_cloud = client_aws.loop();

  if (result_connection_to_cloud == false)
  {
    set_current_event(EVENT_TIMEOUT_CLOUD_CONNECTION);
  }
  if (result_connection_to_cloud == true)
  {
    set_current_event(EVENT_NO_EVENT_ON_NORMAL);
  }
}

/**
 * @brief  Function for publish the telemetry
 * @param  payload
 * @retval none
 */
bool publishTelemetry(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_TELEMETRY.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_TELEMETRY.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the telemetry on alarms topic
 * @param  payload
 * @retval none
 */
bool publish_alarms(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_ALARMS.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_ALARMS.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the telemetry on state topic
 * @param  payload
 * @retval none
 */
bool publish_state(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_STATE.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_STATE.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the raw data of a modbus register
 * @param  payload
 * @retval none
 */
bool publish_raw_data(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_RAW.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_RAW.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the raw data of a modbus register
 * @param  payload
 * @retval none
 */
bool publish_recipe_data(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_RECIPE.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_RECIPE.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the telemetry on state topic
 * @param  payload
 * @retval none
 */
bool publish_machine_state(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_MACHINE_STATE.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_MACHINE_STATE.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the telemetry on log topic
 * @param  payload
 * @retval none
 */
bool publish_log(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_LOG.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_LOG.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the telemetry on machine_data topic
 * @param  payload
 * @retval none
 */
bool publish_machine_data(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_MACHINE_DATA.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_MACHINE_DATA.c_str(), payload);
  }
}
/**
 * @brief  Function for publish the report
 * @param  payload
 * @retval none
 */
bool publish_report_data(const char *payload)
{
  if (result_eth_connection == 1)
  {
    client_aws.publish(AWS_IOT_PUBLISH_TOPIC_REPORTS.c_str(), payload);
  }
  else
  {
    publish_Telemetry_aws_ehernet(AWS_IOT_PUBLISH_TOPIC_REPORTS.c_str(), payload);
  }
}
/**
 * @brief  Function for get the string of the thing name
 * @param  none
 * @retval String AWS_THING_NAME_FROM_FILE
 */
String get_aws_thing_name(void)
{
  return AWS_THING_NAME_FROM_FILE;
}
/**
 * @brief  Function for get the string of the thing name
 * @param  none
 * @retval String AWS_ENDPOINT_FROM_FILE
 */
String get_aws_endpoint_name(void)
{
  return AWS_ENDPOINT_FROM_FILE;
}
/**
 * @brief  Function for get the string of the thing name
 * @param  none
 * @retval String AWS_CERT_CRT_FROM_FILE
 */
String get_aws_certificate_name(void)
{
  return AWS_CERT_CRT_FROM_FILE;
}
/**
 * @brief  Function for get the string of the thing name
 * @param  none
 * @retval String AWS_CERT_PRIVATE_FROM_FILE
 */
String get_aws_key_name(void)
{
  return AWS_CERT_PRIVATE_FROM_FILE;
}
/**
 * @brief  Function for get the string of the cert size
 * @param  none
 * @retval String AWS_CERT_SIZE_FROM_FILE
 */
int get_aws_certificate_size(void)
{
  return AWS_CERT_SIZE_FROM_FILE;
}
/**
 * @brief  Function for get the string of the key size
 * @param  none
 * @retval String AWS_KEY_SIZE_FROM_FILE
 */
int get_aws_key_size(void)
{
  return AWS_KEY_SIZE_FROM_FILE;
}
/**
 * @brief  Function for get the string of the commands topic
 * @param  none
 * @retval String AWS_IOT_SUBSCRIBE_TOPIC_COMMANDS
 */
String get_aws_commands_topic_name(void)
{
  return AWS_IOT_SUBSCRIBE_TOPIC_COMMANDS;
}
/**
 * @brief  Function to compute the right names for the topic
 * @param  thing_name
 * @retval none
 */
void Compute_topics_names(String thing_name)
{
  AWS_IOT_PUBLISH_TOPIC_BUCKET = "devices/" + thing_name + "/bucket";
  AWS_IOT_PUBLISH_TOPIC_TELEMETRY = "devices/" + thing_name + "/events/status";
  AWS_IOT_PUBLISH_TOPIC_STATUS = "devices/" + thing_name + "/events/status";
  AWS_IOT_PUBLISH_TOPIC_ALARMS = "devices/" + thing_name + "/events/alarms";
  AWS_IOT_PUBLISH_TOPIC_STATE = "devices/" + thing_name + "/state";
  AWS_IOT_PUBLISH_TOPIC_RAW = "devices/" + thing_name + "/events/data";
  AWS_IOT_PUBLISH_TOPIC_RECIPE = "devices/" + thing_name + "/events/recipe";
  AWS_IOT_PUBLISH_TOPIC_REPORTS = "devices/" + thing_name + "/events/reports";
  AWS_IOT_PUBLISH_TOPIC_MACHINE_STATE = "devices/" + thing_name + "/events/machine-state";
  AWS_IOT_PUBLISH_TOPIC_LOG = "devices/" + thing_name + "/events/logs";
  AWS_IOT_PUBLISH_TOPIC_MACHINE_DATA = "devices/" + thing_name + "/events/machine-data";
  AWS_IOT_SUBSCRIBE_TOPIC_COMMANDS = "devices/" + thing_name + "/commands";
  AWS_IOT_SUBSCRIBE_TOPIC_CONFIG = "devices/" + thing_name + "/config";
}
