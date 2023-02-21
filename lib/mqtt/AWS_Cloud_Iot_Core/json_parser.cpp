
/*******************************************************************************
 * Title                 :   JSON Parser
 * Filename              :   json_parser.cpp
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
/** @file : json_parser.cpp
 *  @brief This is the main file of the project
 */
#include "json_parser.h"

static String url_from_web;
static String command_from_web;
/**
 * @brief  Function for parsing the payload into a numeric value for the machine id
 * @param  payload
 * @retval id_command
 */
uint16_t Parse_paylod_for_getting_machine_id(String payload)
{
    String first_filter_string = "machineId";
    String id_command = payload.substring(payload.indexOf(first_filter_string));
    id_command = id_command.substring(id_command.indexOf(':') + 1, id_command.indexOf('}'));
    return id_command.toInt();
}
/**
 * @brief  Function for parsing the payload into a numeric value for the number of recipe
 * @param  payload
 * @retval parameter_command
 */
uint16_t Parse_paylod_for_getting_number_receipe(String payload)
{
    String first_filter_string = "recipeId";
    String second_filter_string = "machineId";
    String id_command = payload.substring(payload.indexOf(first_filter_string), payload.indexOf(second_filter_string));
    id_command = id_command.substring(id_command.indexOf(':') + 1, id_command.indexOf('}'));
    return id_command.toInt();
}
/**
 * @brief  Function for parsing the payload into a numeric value for valore pressata
 * @param  payload
 * @retval parameter_command
 */
uint16_t Parse_paylod_for_getting_valore_pressata(String payload)
{
    String first_filter_string = "tPress";
    String second_filter_string = "tGr";
    String id_command = payload.substring(payload.indexOf(first_filter_string), payload.indexOf(second_filter_string));
    id_command = id_command.substring(id_command.indexOf(':') + 2, id_command.indexOf(',') - 1);
    id_command = id_command.toFloat() * SCALING_FACTOR_FROM_FLOAT_TO_INT;
    return id_command.toInt();
}
/**
 * @brief  Function for parsing the payload into a numeric value for valore taglio
 * @param  payload
 * @retval parameter_command
 */
uint16_t Parse_paylod_for_getting_valore_taglio(String payload)
{
    String first_filter_string = "tCut";
    String second_filter_string = "tPress";
    String id_command = payload.substring(payload.indexOf(first_filter_string), payload.indexOf(second_filter_string));
    id_command = id_command.substring(id_command.indexOf(':') + 2, id_command.indexOf(',') - 1);
    id_command = id_command.toFloat() * SCALING_FACTOR_FROM_FLOAT_TO_INT;
    return id_command.toInt();
}
/**
 * @brief  Function for parsing the payload into a numeric value for valore grammatura
 * @param  payload
 * @retval parameter_command
 */
uint16_t Parse_paylod_for_getting_valore_grammatura(String payload)
{
    String first_filter_string = "tGr";
    String second_filter_string = "machineId";
    String id_command = payload.substring(payload.indexOf(first_filter_string), payload.indexOf(second_filter_string));
    id_command = id_command.substring(id_command.indexOf(':') + 2, id_command.indexOf(',') - 2);
    id_command = id_command.toFloat() * SCALING_FACTOR_FROM_FLOAT_TO_INT;
    return id_command.toInt();
}

/**
 * @brief  Function for parsing the payload for the getting the url of fota
 * @param  payload
 * @retval true or false
 */
bool Parse_paylod_for_getting_update_command(String payload)
{
    String first_filter_string = "fw-update";
    int result_find = payload.indexOf(first_filter_string);

    if (result_find != -1)
    {
        String first_filter_string = "urlJSON";
        String second_filter_string = "linkExpiresIn";
        url_from_web = payload.substring(payload.indexOf(first_filter_string), payload.indexOf(second_filter_string));
        url_from_web = url_from_web.substring(url_from_web.indexOf(':') + 2, url_from_web.indexOf(',') - 1);

        command_from_web = "fw-update";

        return true;
    }

    first_filter_string = "fw-rollback";
    result_find = payload.indexOf(first_filter_string);

    if (result_find != -1)
    {
        String first_filter_string = "urlJSON";
        String second_filter_string = "linkExpiresIn";
        url_from_web = payload.substring(payload.indexOf(first_filter_string), payload.indexOf(second_filter_string));
        url_from_web = url_from_web.substring(url_from_web.indexOf(':') + 2, url_from_web.indexOf(',') - 1);

        Serial.println(url_from_web);

        command_from_web = "fw-rollback";

        return true;
    }

    return false;
}
/**
 * @brief  Function for parsing the payload for the getting the hardreset command
 * @param  payload
 * @retval true or false
 */
bool Parse_paylod_for_getting_hardreset_command(String payload)
{
    String first_filter_string = "kill-board";
    int result_find = payload.indexOf(first_filter_string);

    if (result_find != -1)
    {
        return true;
    }

    return false;
}
/**
 * @brief  Function to get the url
 * @param  none
 * @retval url_from_web
 */
String get_url_from_web(void)
{
    return url_from_web;
}
/**
 * @brief  Function to get the commnad from web
 * @param  none
 * @retval command_from_web
 */
String get_fota_command_from_web(void)
{
    return command_from_web;
}
