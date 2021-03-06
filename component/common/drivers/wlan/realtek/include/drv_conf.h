/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __DRV_CONF_H__
#define __DRV_CONF_H__

#include "autoconf.h"
#if ((RTL8195A_SUPPORT==1) || (RTL8711B_SUPPORT==1) || (RTL8721D_SUPPORT == 1))
#include "platform_autoconf.h"
#endif

#if defined (PLATFORM_LINUX) && defined (PLATFORM_WINDOWS)

#error "Shall be Linux or Windows, but not both!\n"

#endif

//Older Android kernel doesn't has CONFIG_ANDROID defined,
//add this to force CONFIG_ANDROID defined
#ifdef CONFIG_PLATFORM_ANDROID
#define CONFIG_ANDROID
#endif

#ifdef CONFIG_ANDROID
//Some Android build will restart the UI while non-printable ascii is passed
//between java and c/c++ layer (JNI). We force CONFIG_VALIDATE_SSID
//for Android here. If you are sure there is no risk on your system about this,
//mask this macro define to support non-printable ascii ssid.
//#define CONFIG_VALIDATE_SSID
#ifdef CONFIG_PLATFORM_ARM_SUNxI
	#ifdef CONFIG_VALIDATE_SSID
		#undef CONFIG_VALIDATE_SSID
	#endif
#endif

//Android expect dbm as the rx signal strength unit
#define CONFIG_SIGNAL_DISPLAY_DBM
#endif

#if defined(CONFIG_HAS_EARLYSUSPEND) && defined (CONFIG_RESUME_IN_WORKQUEUE)
	#warning "You have CONFIG_HAS_EARLYSUSPEND enabled in your system, we disable CONFIG_RESUME_IN_WORKQUEUE automatically"
	#undef CONFIG_RESUME_IN_WORKQUEUE
#endif

#if defined(CONFIG_ANDROID_POWER) && defined (CONFIG_RESUME_IN_WORKQUEUE)
	#warning "You have CONFIG_ANDROID_POWER enabled in your system, we disable CONFIG_RESUME_IN_WORKQUEUE automatically"
	#undef CONFIG_RESUME_IN_WORKQUEUE
#endif

#ifdef CONFIG_RESUME_IN_WORKQUEUE //this can be removed, because there is no case for this...
	#if !defined( CONFIG_WAKELOCK) && !defined(CONFIG_ANDROID_POWER)
	#error "enable CONFIG_RESUME_IN_WORKQUEUE without CONFIG_WAKELOCK or CONFIG_ANDROID_POWER will suffer from the danger of wifi's unfunctionality..."
	#error "If you still want to enable CONFIG_RESUME_IN_WORKQUEUE in this case, mask this preprossor checking and GOOD LUCK..."
	#endif
#endif

//About USB VENDOR REQ
#if defined(CONFIG_USB_VENDOR_REQ_BUFFER_PREALLOC) && !defined(CONFIG_USB_VENDOR_REQ_MUTEX) 
	#warning "define CONFIG_USB_VENDOR_REQ_MUTEX for CONFIG_USB_VENDOR_REQ_BUFFER_PREALLOC automatically"
	#define CONFIG_USB_VENDOR_REQ_MUTEX
#endif
#if defined(CONFIG_VENDOR_REQ_RETRY) &&  !defined(CONFIG_USB_VENDOR_REQ_MUTEX)
	#warning "define CONFIG_USB_VENDOR_REQ_MUTEX for CONFIG_VENDOR_REQ_RETRY automatically"
	#define CONFIG_USB_VENDOR_REQ_MUTEX
#endif

#ifndef CONFIG_RTW_ADAPTIVITY_EN
	#define CONFIG_RTW_ADAPTIVITY_EN 0
#endif

#ifndef CONFIG_RTW_ADAPTIVITY_MODE
	#define CONFIG_RTW_ADAPTIVITY_MODE 0
#endif

#ifndef CONFIG_RTW_ADAPTIVITY_DML
	#define CONFIG_RTW_ADAPTIVITY_DML 0
#endif

#ifndef CONFIG_RTW_ADAPTIVITY_DC_BACKOFF
	#define CONFIG_RTW_ADAPTIVITY_DC_BACKOFF 4
#endif

#ifndef CONFIG_RTW_NHM_EN
	#define CONFIG_RTW_NHM_EN 0
#endif

#ifndef CONFIG_IFACE_NUMBER
	#ifdef CONFIG_CONCURRENT_MODE
		#define CONFIG_IFACE_NUMBER	2
	#else
		#define CONFIG_IFACE_NUMBER	1
	#endif
#endif

//#include <rtl871x_byteorder.h>

#endif // __DRV_CONF_H__

