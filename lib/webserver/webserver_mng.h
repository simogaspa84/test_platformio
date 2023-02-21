/****************************************************************************
 * Title                 :   Webserver header file
 * Filename              :   webserver_mng.h
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
* Copyright (c) 2021 - Systel Elettronica - All Rights Reserved

* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Template.
*
*****************************************************************************/
/*************** INTERFACE CHANGE LIST **************************************
 *
 *    Date    Version   Author         Description
 *  28/12/21  1.0.0   Simone Gasparella   Interface Created.
 *
 *****************************************************************************/
/** @file  webserver_mng.h
 *  @brief This is the header file web_server.h
 *  This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef WEBSERVER_MNG_H
#define WEBSERVER_MNG_H

#define HTTP_REQUEST_GO_TO_MAINTENANCE 1
#define HTTP_REQUEST_GO_TO_NORMAL 2
#define HTTP_REQUEST_GO_TO_INIT 3

void web_portal_init(uint8_t ethernet_cable);
void web_portal_start_ethernet(void);
void web_portal_end(void);
uint8_t get_event_on_web_portal(void);
void reset_event_on_web_portal(void);
void set_current_state_on_web_portal(int curremt_state_for_web_portal);

#endif