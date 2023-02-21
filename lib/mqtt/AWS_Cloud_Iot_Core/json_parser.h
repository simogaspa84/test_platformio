/****************************************************************************
 * Title                 :   JSON Parser header file
 * Filename              :   json_parser.h
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
/** @file  json_parser.h
 *  @brief This is the header file json_parser.cpp*
 *   This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <Arduino.h>

#define SCALING_FACTOR_FROM_FLOAT_TO_INT 10

uint16_t Parse_paylod_for_getting_machine_id(String payload);
uint16_t Parse_paylod_for_getting_number_receipe(String payload);
uint16_t Parse_paylod_for_getting_valore_pressata(String payload);
uint16_t Parse_paylod_for_getting_valore_taglio(String payload);
uint16_t Parse_paylod_for_getting_valore_grammatura(String payload);
bool Parse_paylod_for_getting_update_command(String payload);
bool Parse_paylod_for_getting_hardreset_command(String payload);
String get_url_from_web(void);
String get_fota_command_from_web(void);

#endif