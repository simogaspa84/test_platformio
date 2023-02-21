/****************************************************************************
 * Title                 :   OTA
 * Filename              :   AsyncElegantOTA.h
 * Author                :   Simone Gasparella
 * Origin Date           :   01/04/2022
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
* Copyright (c) 2022 - Systel Elettronica - All Rights Reserved

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Template.
*
*****************************************************************************/
/*************** INTERFACE CHANGE LIST **************************************
 *
 *    Date    Version   Author         Description
 *  01/04/22  1.0.0   Simone Gasparella   Interface Created.
 *
 *****************************************************************************/
/** @file  AsyncElegantOTA.h
 *  @brief This is the header file AsyncElegantOTA.h
 *  This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef AsyncElegantOTA_h
#define AsyncElegantOTA_h

#include "Arduino.h"
#include "stdlib_noniso.h"
#include "WiFi.h"
#include "AsyncTCP.h"
#include "Update.h"
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"
#include "Hash.h"
#include "ESPAsyncWebServer.h"
#include "FS.h"
#include "elegantWebpage.h"
#include <machine.h>

#define HTTP_REQUEST_GO_TO_MAINTENANCE 1
#define HTTP_REQUEST_GO_TO_NORMAL 2
#define HTTP_REQUEST_GO_TO_INIT 3

/*! AsyncElegantOta class used for providing a nice gui for update and log info */
class AsyncElegantOtaClass
{

public:
    /*function for setting id*/
    void setID(const char *id)
    {
        _id = id;
    }

    uint8_t id_htt_request;
    int current_state_machine;

    /*function for begin the webserver*/
    void begin(AsyncWebServer *server, const char *username = "", const char *password = "")
    {
        _server = server;

        if (strlen(username) > 0)
        {
            _authRequired = true;
            _username = username;
            _password = password;
        }
        else
        {
            _authRequired = false;
            _username = "";
            _password = "";
        }
        /*HTTP request for MAINTENANCE STATE*/
        _server->on("/maintenance", HTTP_GET, [&](AsyncWebServerRequest *request)
                    {   
                        id_htt_request = HTTP_REQUEST_GO_TO_MAINTENANCE;
                        request->send(200, "text/plain", "MAINTENANCE MODE"); });
        /*HTTP request for NORMAL STATE*/
        _server->on("/normal", HTTP_GET, [&](AsyncWebServerRequest *request)
                    {  
                         id_htt_request = HTTP_REQUEST_GO_TO_NORMAL;
                        request->send(200, "text/plain", "NORMAL MODE"); });
        /*HTTP request for INIT STATE*/
        _server->on("/init", HTTP_GET, [&](AsyncWebServerRequest *request)
                    {   
                        id_htt_request = HTTP_REQUEST_GO_TO_INIT;
                        request->send(200, "text/plain", "INIT MODE"); });

        /*HTTP rqquest for update with authentication*/
        _server->on("/update/identity", HTTP_GET, [&](AsyncWebServerRequest *request)
                    {
                if(_authRequired)
                {
                    if(!request->authenticate(_username.c_str(), _password.c_str()))
                    {
                        return request->requestAuthentication();
                    }
                }
       
                request->send(200, "application/json", "{\"id\": \""+_id+"\", \"hardware\": \"ESP32\"}"); });

        /*HTTP request for update*/
        _server->on("/update", HTTP_GET, [&](AsyncWebServerRequest *request)
                    {
            if ((_authRequired) && (current_state_machine == 2))
            {
                if (!request->authenticate(_username.c_str(), _password.c_str()))
                {
                    return request->requestAuthentication();
                }
            }
            if ((current_state_machine == 2))
            {
                AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", ELEGANT_HTML, ELEGANT_HTML_SIZE);
                response->addHeader("Content-Encoding", "gzip");
                request->send(response);
                
                /*set the event*/
                set_current_event(EVENT_OTA_IN_PROGRESS);
            } });

        /*HTTP post for update*/
        _server->on(
            "/update", HTTP_POST, [&](AsyncWebServerRequest *request)
            {
            if ((_authRequired) && (current_state_machine == 2))
            {
                if (!request->authenticate(_username.c_str(), _password.c_str()))
                {
                    return request->requestAuthentication();
                }
            }
            // the request handler is triggered after the upload has finished...
            // create the response, add header, and send response
            AsyncWebServerResponse *response = request->beginResponse((Update.hasError()) ? 500 : 200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
            response->addHeader("Connection", "close");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
            restart(); },
            [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
            {
                // Upload handler chunks in data
                if (_authRequired)
                {
                    if (!request->authenticate(_username.c_str(), _password.c_str()))
                    {
                        return request->requestAuthentication();
                    }
                }

                if (!index)
                {
                    if (!request->hasParam("MD5", true))
                    {
                        return request->send(400, "text/plain", "MD5 parameter missing");
                    }

                    if (!Update.setMD5(request->getParam("MD5", true)->value().c_str()))
                    {
                        return request->send(400, "text/plain", "MD5 parameter invalid");
                    }

                    int cmd = (filename == "filesystem") ? U_SPIFFS : U_FLASH;

                    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
                    { // Start with max available size
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }

                // Write chunked data to the free sketch space
                if (len)
                {
                    if (Update.write(data, len) != len)
                    {
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }

                if (final)
                { // if the final flag is set then this is the last frame of data
                    if (!Update.end(true))
                    { // true to set the size to the current progress
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "Could not end OTA");
                    }
                }
                else
                {
                    return;
                }
            });
    }

    /*function for restarting the ESP after the update*/
    void restart()
    {
        yield();
        delay(1000);
        yield();
        ESP.restart();
    }

private:
    AsyncWebServer *_server;

    String getID()
    {
        String id = "";
        id = String((uint32_t)ESP.getEfuseMac(), HEX);
        id.toUpperCase();
        return id;
    }

    String _id = getID();
    String _username = "";
    String _password = "";
    bool _authRequired = false;
};

AsyncElegantOtaClass AsyncElegantOTA;
#endif
