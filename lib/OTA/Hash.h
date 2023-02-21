/****************************************************************************
* Title                 :   Hash    
* Filename              :   Hash.h
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
/** @file  Hash.h
 *  @brief This is the header file Hash.h
 *  This is the header file for the definition TODO: MORE ABOUT ME!
 */

#ifndef HASH_H_
#define HASH_H_

void sha1(const uint8_t* data, uint32_t size, uint8_t hash[20]);
void sha1(const char* data, uint32_t size, uint8_t hash[20]);
void sha1(const String& data, uint8_t hash[20]);

String sha1(const uint8_t* data, uint32_t size);
String sha1(const char* data, uint32_t size);
String sha1(const String& data);

#endif /* HASH_H_ */