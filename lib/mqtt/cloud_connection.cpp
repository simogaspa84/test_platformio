/****************************************************************************
 * Title                 :   Cloud connection
 * Filename              :   cloud_connection.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   24/02/2021
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
/** @file cloud_connection.cpp
 *  @brief This file contains
 *
 */

#include "cloud_connection.h"
#include <Client.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#include <ethernet_aws.h>

// Initialize WiFi and MQTT for this board
WiFiClient *netClient;

unsigned long iat = 0;
const int keepAlive = 180;
const int timeout = 1000;
const int buffer_size = 512;
const int valid_utc_date = 1645698852;
unsigned long getTime;

/**
 * @brief  Function for retrieve wifi credentials and start wifi
 * @param  none
 * @retval none
 */
void Retrieve_saved_credentials_and_startwifi(void)
{
  /*local variables*/
  Preferences preferences;
  char ssid_arr[MAX_SIZE_CREDENTIALS_IN_BYTE];
  char password_arr[MAX_SIZE_CREDENTIALS_IN_BYTE];
  String rec_ssid;
  String rec_password;

  /*Get info about the wifi credendials from flash*/
  preferences.begin("my-pref", false);
  rec_ssid = preferences.getString("rec_ssid", "Sample_SSID");
  rec_password = preferences.getString("rec_password", "abcdefgh");
  rec_ssid.toCharArray(ssid_arr, rec_ssid.length() + 1);
  rec_password.toCharArray(password_arr, rec_password.length() + 1);

  /*starting wifi service*/
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_arr, password_arr);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}

/**
 * @brief  Function for connecting with the wifi
 * @param  timeinfo
 * @retval none
 */
void get_actual_time(struct tm *timeinfo)
{
  if (!getLocalTime(&timeinfo[0]))
  {
    Serial.println("Failed to obtain time");
    return;
  }
}

/**
 * @brief  Function for connecting with the wifi
 * @param  none
 * @retval none
 */
void connectWifi(void)
{
  Serial.print("checking wifi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("connected to wifi");
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.localIP());
  }
}

/**
 * @brief  Function for setting the connecting to the cloud with ethernet
 * @param  none
 * @retval result_connection
 */
uint8_t setupCloudIoTETH(void)
{
  /*Init of the ethernet*/
  uint8_t result_connection = setup_ethernet();

  return result_connection;
}
/**
 * @brief  Function to execute in loop generated by the arduino framework
 * @param  None
 * @retval None
 */
void loop_aws_ethernet(void)
{
  loop_ethernet();
}
