/****************************************************************************
 * Title                 :   Captive Portal
 * Filename              :   captive_portal.h
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
/** @file captive_portal.h
 *  @brief This is the header file captive_portal.h
 *  This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef CAPTIVE_PORTAL_H
#define CAPTIVE_PORTAL_H

#define DEBUG false
#define DNS_PORT_NUMBER 53
#define ASYNC_WEBSERVER_PORT 80
#define MAX_DIM_CHAR_SSID_NAME 20
#define MAX_DIM_CHAR_PASSWORD_NAME 20
#define MAX_NUM_WIFI_NETWORKS 20
#define TIME_THRESHOLD_WIFI_CONNECTION 50000 // 50000 milliseconds elapsed connecting to WiFi
#define TIME_DELAY_IN_MS 2000                // 2000 ms
#define SERIAL_BAUDRATE 115200

void setupServer(void);
void WiFiSoftAPSetup(void);
void StartCaptivePortal(void);
int setup_captive_and_wifi(void);
void publish_data_from_uart(void);
int Wifi_list_search(void);
int delete_preferences(uint16_t val);
String processor(const String &var);

#endif