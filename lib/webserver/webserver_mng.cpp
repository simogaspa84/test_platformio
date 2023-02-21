/*******************************************************************************
 * Title                 :   Webserver
 * Filename              :   webserver_mng.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   31/03/2022
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
 *  31/03/22   1.0.0   Simone Gasparella   Initial Release.
 *
 *******************************************************************************/
/** @file : webserver_mng.cpp
 *  @brief This module is created for handling the tasks
 */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char *ota_username = "Systel";
const char *ota_password = "lab2022";

AsyncWebServer local_server(80);

/**
 * @brief  Function to execute the init of the webportal task
 * @param  ethernet_cable
 * @retval None
 */
void web_portal_init(uint8_t ethernet_cable)
{
  /*run the webserver only if wifi active*/
  if (ethernet_cable == 1)
  {
    local_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->send(200, "text/plain", "Hi Guest! I am IOT board. Everything works fine"); });

    AsyncElegantOTA.begin(&local_server, ota_username, ota_password);
    local_server.begin();
    Serial.println("HTTP web server started (Wifi)");
  }
  else
  {
    Serial.println("HTTP web server started (Ethernet)");
  }
}
/**
 * @brief  Function to starting the local webserver in case of ethernet cable
 * @param  None
 * @retval None
 */
void web_portal_start_ethernet(void)
{
}
/**
 * @brief  Function for ending the local webserver
 *
 * @param  None
 * @retval None
 */
void web_portal_end(void)
{
}
/**
 * @brief  Function for getting the event occured in the webportal
 * @param  None
 * @retval AsyncElegantOTA.id_htt_request
 */
uint8_t get_event_on_web_portal(void)
{
  return AsyncElegantOTA.id_htt_request;
}
/**
 * @brief  Function for resetting the event occured in the webportal
 * @param  None
 * @retval None
 */
void reset_event_on_web_portal(void)
{
  AsyncElegantOTA.id_htt_request = 0;
}
/**
 * @brief  Function for updating the current machine state on the webportal
 * @param  curremt_state_for_web_portal
 * @retval AsyncElegantOTA.current_state_machine
 */
void set_current_state_on_web_portal(int curremt_state_for_web_portal)
{
  AsyncElegantOTA.current_state_machine = curremt_state_for_web_portal;
}