/*******************************************************************************
 * Title                 :   Version
 * Filename              :   version.cpp
 * Author                :   Simone Gasparella
 * Origin Date           :   20/12/2022
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
 *  20/12/22   1.0.0   Simone Gasparella   Initial Release.
 *
 *******************************************************************************/
/** @file : version.cpp
 *  @brief This module is created for saving the version of the fw
 */

#include "version.h"

int firmware_version_major = 1;
int firmware_version_minor = 0;
int firmware_version_patch = 0;