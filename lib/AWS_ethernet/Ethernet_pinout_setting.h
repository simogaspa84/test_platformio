/****************************************************************************
 * Title                 :   Ethernet pinout setting
 * Filename              :   Ethernet_pinout_setting.h
 * Author                :   Simone Gasparella
 * Origin Date           :   04/01/2023
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
 *  04/01/23  1.0.0   Simone Gasparella   Interface Created.
 *
 *****************************************************************************/
/** @file  Ethernet_pinout_setting.h
 *  @brief This is the header file ethernet_aws.h
 *   This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef ETHERNET_PINOUT_SETTING_H_
#define ETHERNET_PINOUT_SETTING_H_

#define ETH_SPI_BUS HSPI
#define ETH_MOSI 13
#define ETH_MISO 12
#define ETH_SCLK 14
#define ETH_CS 15

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

#endif