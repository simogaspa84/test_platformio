/****************************************************************************
 * Title                 :   Ethernet connection on aws
 * Filename              :   ethernet_aws.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   14/10/2022
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
 *  14/10/22  1.0.0   Simone Gasparella   Interface Created.
 *
 *****************************************************************************/
/** @file ethernet_aws.cpp
 *  @brief This file contains function for the connection with aws cloud with ethernet cable
 *
 */

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "Ethernet_pinout_setting.h"
#include <PubSubClient.h>
#include <SSL_Utility/EthernetWebServer_SSL.h>
#include "AWS_security.h"
#include "ethernet_aws.h"
#include <NTPClient.h>
#include <EthernetUdp.h>
#include <WiFiUdp.h>
#include <Client.h>
#include <sys/time.h>
#include "machine.h"
#include <AWS_Cloud_Iot_Core/json_parser.h>
#include <AWS_Cloud_Iot_Core/aws.h>
#include "task_module.h"
#include <mqtt_mng.h>

void callback(char *topic, byte *payload, unsigned int length);

SPIClass ethernetSPI(ETH_SPI_BUS);
EthernetClient ethClient;
EthernetSSLClient ethClientSSL(ethClient, TAs, (size_t)TAs_NUM, 1);
PubSubClient mqtt_istance(mqttServer, PORT_NUMBER_SECURE_MQTT, callback, ethClientSSL);
EthernetUDP ethernetUdpClient;
NTPClient timeClient(ethernetUdpClient, "pool.ntp.org", NTP_TIME_OFFSET_SECONDS, NTP_TIME_REFRESH_INTERVAL);
std::unique_ptr<SSLClientParameters> mTLS;

static uint16_t commands_array[10] = {0};

/**
 * @brief  Function to execute the callback from mqtt message
 * @param  topic
 * @param  payload
 * @param  length
 * @retval None
 */
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String first_filter_string = "changeRecipeParams";
  String strPayload = String((char *)payload);
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
      memset(commands_array, 0, 10);
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
      memset(commands_array, 0, 10);
    }
    else
    {
      Serial.println("command queue is full!");
    }
  }

  /*checking for receiving the command for update fw with fota*/
  if (Parse_paylod_for_getting_update_command(strPayload))
  {
    set_current_event(EVENT_RESTART_FOR_FOTA);
  }

  /*checking for receiving the command for update fw with fota*/
  if (Parse_paylod_for_getting_hardreset_command(strPayload))
  {
    set_current_event(EVENT_HARD_RESET);
  }
}
/**
 * @brief  Function to execute the reconnection if it was not connected
 * @param  None
 * @retval None
 */
void reconnect(void)
{

  while (!mqtt_istance.connected())
  {
    Serial.println("Attempting MQTT connection...");

    if (mqtt_istance.connect(get_aws_thing_name().c_str()))
    {
      Serial.println("connected");

      mqtt_istance.subscribe(get_aws_commands_topic_name().c_str());
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt_istance.state());
      Serial.println(" try again in 5 seconds");
      /*set event for led and machine*/
      set_current_event(EVENT_TIMEOUT_CLOUD_CONNECTION);
      delay(5000);
    }
  }
}
/**
 * @brief  Function to do the setup of the timestamp
 * @param  None
 * @retval None
 */
void setup_time(void)
{
  Serial.println("Starting LAN");
  Serial.println("Waiting on time sync...");

  timeClient.begin();
  bool ok = timeClient.forceUpdate();
  if (ok)
  {
    Serial.println("NTP update okay!");
    Serial.println("It is: " + timeClient.getFormattedTime());
    unsigned long timestamp = timeClient.getEpochTime();
    Serial.println("Unix time: " + String(timestamp));
    // Set as system time
    struct timeval now;
    int rc;
    now.tv_sec = (time_t)timestamp;
    now.tv_usec = 0;
    rc = settimeofday(&now, NULL);
    if (rc == 0)
    {
      Serial.println("settimeofday() successful.");
    }
    else
    {
      Serial.println("settimeofday() failed");
    }
  }
  else
  {
    Serial.println("NTP failed!!");
  }
  timeClient.end();

  time_t rawtime;
  struct tm *info;
  char buffer[80];
  time(&rawtime);
  info = localtime(&rawtime);
  strftime(buffer, 80, "%x - %I:%M%p", info);
  Serial.println("Current date + time: " + String(buffer));

  // before Tue Nov 14 2017 07:36:07 GMT+0000? we did not sync correctly..
  if (time(nullptr) < 1510644967)
  {
    Serial.println("NTP sync failed, time is behind..");
  }
  else
  {
    Serial.println("NTP sync time sanity check passed.");
  }
}
/**
 * @brief  Function to execute the setup of the ethernet (functionality and timestamp)
 * @param  None
 * @retval result_connection the result of the connection
 */
uint8_t setup_ethernet(void)
{
  uint8_t result_connection = 0;

  /*retrieve security info for aws from memory*/
  Load_secrets_from_memory();

  PubSubClient mqtt_istance(get_aws_endpoint_name().c_str(), PORT_NUMBER_SECURE_MQTT, callback, ethClientSSL);

  /*init of some variables useful for setting the mTLS object*/
  const char my_cert_size[get_aws_certificate_size()] = "";
  const char my_key_size[get_aws_key_size()] = "";

  /*creation of the mTLS object*/
  mTLS = std::unique_ptr<SSLClientParameters>(new SSLClientParameters(SSLClientParameters::fromPEM(get_aws_certificate_name().c_str(),
                                                                                                   sizeof my_cert_size, get_aws_key_name().c_str(), sizeof my_key_size)));

  /*authentication on aws*/
  ethClientSSL.setMutualAuthParams(*mTLS);

  Ethernet.init(ETH_CS, &ethernetSPI, ETH_SCLK, ETH_MISO, ETH_MOSI);
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Ethernet.begin(mac, TIME_OUT_WAITING_ETH_CABLE);
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {

    result_connection = 1;
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");

    while (true)
    {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF)
  {
    result_connection = 1;
    Serial.println("Ethernet cable is not connected.");
  }

  Serial.print("Joined LAN with IP ");
  Serial.println(Ethernet.localIP());
  Serial.flush();

  setup_time();

  // Create a message handler
  mqtt_istance.setCallback(callback);

  return result_connection;
}

/**
 * @brief  Function to execute the loop of the mqtt connection
 * @param  None
 * @retval None
 */
void loop_ethernet(void)
{

  if (!mqtt_istance.connected())
  {
    reconnect();
  }

  bool result_connection_to_cloud = false;

  result_connection_to_cloud = mqtt_istance.loop();

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
 * @param  *topic
 * @param  *payload
 * @retval  bool
 */
bool publish_Telemetry_aws_ehernet(const char *topic, const char *payload)
{
  mqtt_istance.publish(topic, payload);
}
