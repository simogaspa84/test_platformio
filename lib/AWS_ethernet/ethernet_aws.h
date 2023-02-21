
/****************************************************************************
 * Title                 :   Ethernet connection on aws
 * Filename              :   ethernet_aws.h
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
/** @file  ethernet_aws.h
 *  @brief This is the header file ethernet_aws.h
 *   This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef ETHERNET_AWS_H
#define ETHERNET_AWS_H

#define TIME_OUT_WAITING_ETH_CABLE 10000
#define PORT_NUMBER_SECURE_MQTT 8883
#define NTP_TIME_OFFSET_SECONDS 0       // 0h in seconds
#define NTP_TIME_REFRESH_INTERVAL 60000 // 60s min milliseconds

uint8_t setup_ethernet(void);
void loop_ethernet(void);
bool publish_Telemetry_aws_ehernet(const char *topic, const char *payload);

#endif