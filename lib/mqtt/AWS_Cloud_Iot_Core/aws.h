/****************************************************************************
 * Title                 :   AWS
 * Filename              :   aws.h
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
/** @file  aws.h
 *  @brief This is the header file aws.h*
 *   This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef AWS_H
#define AWS_H

#include <Arduino.h>
#include "secrets.h"

#define NUM_REGISTERS_COMMANDS 10

void setupCloudIotWIFI(void);
void loop_aws_wifi(void);
bool publishTelemetry(const char *payload);
bool publish_alarms(const char *payload);
bool publish_state(const char *payload);
bool publish_raw_data(const char *payload);
bool publish_recipe_data(const char *payload);
bool publish_machine_state(const char *payload);
bool publish_log(const char *payload);
bool publish_machine_data(const char *payload);
bool publish_report_data(const char *payload);

void Load_secrets_from_memory(void);
void Compute_topics_names(String thing_name);

String get_aws_thing_name(void);
String get_aws_endpoint_name(void);
String get_aws_certificate_name(void);
String get_aws_key_name(void);
int get_aws_certificate_size(void);
int get_aws_key_size(void);
String get_aws_commands_topic_name(void);

#endif