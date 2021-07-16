/**************************************************************************//**
 * @file     system_rtl8195blp.h
 * @brief    CMSIS Device System Initialization Header File for rtl8195b-lp..
 * @version  v1.00
 * @date     2017/11/23
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef _SYSTEM_RTL8195BLP_H_
#define _SYSTEM_RTL8195BLP_H_

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock) */


/**
 * \brief Setup the microcontroller system.
 *  Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
 * \brief  Update SystemCoreClock variable.
 *  Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_RTL8195BLP_H_ */

