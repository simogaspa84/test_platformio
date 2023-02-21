/*******************************************************************************
 * Title                 :   Automatic Ota
 * Filename              :   automatic_ota.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   11/10/2022
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
 *  11/10/22   1.0.0   Simone Gasparella   Initial Release.
 *
 *******************************************************************************/
/** @file : automatic_ota.cpp
 *  @brief This module is created for handling the automatic ota
 */

#include <esp32fota.h>
#include "automatic_ota.h"
#include <cloud_connection.h>
#include "machine.h"
#include <FS.h>
#include <SPIFFS.h>
#include "version.h"

const char *firmware_name = "esp32-fota-http";
const bool check_signature = true;
const bool disable_security = true;

esp32FOTA FOTA;
CryptoFileAsset *MyRSAKey = new CryptoFileAsset("/rsa_key.pub", &SPIFFS);
String command_to_apply;

/**
 * @brief  Function to do the setup for the ota
 * @param  String fota_url the url that contains the link to the update
 * @param  String fota_command the command can be update or force update
 * @retval None
 */
void Setup_automatic_ota(String fota_url, String fota_command)
{
    /*retrieve wifi credentials and start wifi*/
    Retrieve_saved_credentials_and_startwifi();

    SPIFFS.begin(true);

    auto cfg = FOTA.getConfig();
    cfg.name = firmware_name;
    cfg.sem = SemverClass(firmware_version_major, firmware_version_minor, firmware_version_patch);
    cfg.check_sig = check_signature;
    cfg.pub_key = MyRSAKey;
    cfg.unsafe = disable_security;

    FOTA.setConfig(cfg);
    FOTA.setManifestURL(fota_url.c_str());

    command_to_apply = fota_command;
}

/**
 * @brief  Function to loop the ota
 * @param  None
 * @retval None
 */
void Loop_automatic_ota(void)
{

    set_current_event(EVENT_FOTA_IN_PROGRESS);

    Serial.println(command_to_apply);

    if (command_to_apply == "fw-rollback")
    {
        Serial.println("entro");
        FOTA.forceUpdate(true);
    }

    bool updatedNeeded = FOTA.execHTTPcheck();
    if (updatedNeeded)
    {
        FOTA.execOTA();
    }

    delay(2000);
}
