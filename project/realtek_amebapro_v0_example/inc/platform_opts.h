/**
 ******************************************************************************
 *This file contains general configurations for ameba platform
 ******************************************************************************
*/

#ifndef __PLATFORM_OPTS_H__
#define __PLATFORM_OPTS_H__

/*For MP mode setting*/
#define SUPPORT_MP_MODE		1

/**
 * For AT cmd Log service configurations
 */
#define SUPPORT_LOG_SERVICE	1
#if SUPPORT_LOG_SERVICE
#undef LOG_SERVICE_BUFLEN
#define LOG_SERVICE_BUFLEN     100 //can't larger than UART_LOG_CMD_BUFLEN(127)
#define CONFIG_LOG_HISTORY	0
#if CONFIG_LOG_HISTORY
#define LOG_HISTORY_LEN    5
#endif
#define SUPPORT_INTERACTIVE_MODE	0//on/off wifi_interactive_mode
#define CONFIG_LOG_SERVICE_LOCK		0
#define CONFIG_ATCMD_MP				0 //support MP AT command
#define USE_MODE                    1 //for test
#endif

/**
 * For interactive mode configurations, depends on log service
 */
#if SUPPORT_INTERACTIVE_MODE
#define CONFIG_INTERACTIVE_MODE		1
#define CONFIG_INTERACTIVE_EXT		0
#else
#define CONFIG_INTERACTIVE_MODE		0
#define CONFIG_INTERACTIVE_EXT		0
#endif

/**
 * For FreeRTOS tickless configurations
 */
#define FREERTOS_PMU_TICKLESS_PLL_RESERVED   0   // In sleep mode, 0: close PLL clock, 1: reserve PLL clock
#define FREERTOS_PMU_TICKLESS_SUSPEND_SDRAM  1   // In sleep mode, 1: suspend SDRAM, 0: no act

/******************************************************************************/

/**
* For common flash usage  
*/
#define UART_SETTING_SECTOR		0x000FC000
#define FAST_RECONNECT_DATA 	0x5000
#define DCT_BEGIN_ADDR			0x200000	/*!< DCT begin address of flash, ex: 0x200000 = 2M */

/**
* Unlokc write protect for winbond flash
*/
#define WINBOND_FLASH_UNPROTECT	0
/**
 * For Wlan configurations
 */
#define CONFIG_WLAN		1
#if CONFIG_WLAN
#define CONFIG_LWIP_LAYER	1
#define CONFIG_INIT_NET		1 //init lwip layer when start up
#define CONFIG_WIFI_IND_USE_THREAD	0	// wifi indicate worker thread
#define CONFIG_ENABLE_AP_POLLING_CLIENT_ALIVE 0 // on or off AP POLLING CLIENT

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT	0
#define CONFIG_OTA_UPDATE	1
#define CONFIG_BSD_TCP		0//NOTE : Enable CONFIG_BSD_TCP will increase about 11KB code size
#define CONFIG_AIRKISS		0//on or off tencent airkiss
#define CONFIG_UART_SOCKET	0
#define CONFIG_JD_SMART		0//on or off for jdsmart
#define CONFIG_JOYLINK			0//on or off for jdsmart2.0
#define CONFIG_QQ_LINK		0//on or off for qqlink
#define CONFIG_AIRKISS_CLOUD	0//on or off for weixin hardware cloud
#define CONFIG_UART_YMODEM	0//support uart ymodem upgrade or not
#define CONFIG_GOOGLE_NEST	0//on or off the at command control for google nest
#define CONFIG_TRANSPORT	0//on or off the at command for transport socket
#define CONFIG_ALINK		0//on or off for alibaba alink

#define CONFIG_LIGHT_SENSOR 0 // NOTE: enable it only when with light sensor

#define CONFIG_VIDEO_APPLICATION 1	// make lwip use large buffer

/* For WPS and P2P */
#define CONFIG_ENABLE_WPS		0
#define CONFIG_ENABLE_P2P		0
#if CONFIG_ENABLE_WPS
#define CONFIG_ENABLE_WPS_DISCOVERY	1
#endif
#if CONFIG_ENABLE_P2P
#define CONFIG_ENABLE_WPS_AP		1
#undef CONFIG_WIFI_IND_USE_THREAD
#define CONFIG_WIFI_IND_USE_THREAD	1
#endif
#if (CONFIG_ENABLE_P2P && ((CONFIG_ENABLE_WPS_AP == 0) || (CONFIG_ENABLE_WPS == 0)))
#error "If CONFIG_ENABLE_P2P, need to define CONFIG_ENABLE_WPS_AP 1" 
#endif

/* For SSL/TLS */
#define CONFIG_USE_POLARSSL     0
#define CONFIG_USE_MBEDTLS      1

/* For Simple Link */
#define CONFIG_INCLUDE_SIMPLE_CONFIG		1

/*For fast reconnection*/
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	1
#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_FAST_DHCP 1
#else
#define CONFIG_FAST_DHCP 0
#endif

/*For wowlan service settings*/
#define CONFIG_WOWLAN_SERVICE           			0

#define CONFIG_GAGENT			0
/*Disable CONFIG_EXAMPLE_WLAN_FAST_CONNECT when CONFIG_GAGENT is enabled,because
	reconnect to previous AP is not suitable when re-configuration. 
*/
#if CONFIG_GAGENT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0
#endif

/*For promisc rx unsupported pkt info */
#define CONFIG_UNSUPPORT_PLCPHDR_RPT 0

#endif //end of #if CONFIG_WLAN
/*******************************************************************************/

/* For LWIP configuration */
#define CONFIG_LWIP_DHCP_COARSE_TIMER 60

/**
 * For Ethernet configurations
 */
#define CONFIG_ETHERNET 0
#if CONFIG_ETHERNET

#define CONFIG_LWIP_LAYER	1
#define CONFIG_INIT_NET         1 //init lwip layer when start up

//on/off relative commands in log service
#define CONFIG_SSL_CLIENT	0
#define CONFIG_BSD_TCP		0//NOTE : Enable CONFIG_BSD_TCP will increase about 11KB code size

#endif


/**
 * For iNIC configurations
 */
#ifdef CONFIG_INIC //this flag is defined in IAR project
#define CONFIG_INIC_EN 1//enable iNIC mode
#undef CONFIG_ENABLE_WPS
#define CONFIG_ENABLE_WPS 1
#undef CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG	1
#undef CONFIG_WOWLAN_SERVICE
#define CONFIG_WOWLAN_SERVICE 1
#undef LOG_SERVICE_BUFLEN
#define LOG_SERVICE_BUFLEN 256
#undef CONFIG_LWIP_LAYER
#define CONFIG_LWIP_LAYER	0
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE 0
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0
#define CONFIG_INIC_SDIO_HCI	1 //for SDIO or USB iNIC
#define CONFIG_INIC_USB_HCI	0
#define CONFIG_INIC_CMD_RSP     1 //need to return msg to host
#endif
/******************End of iNIC configurations*******************/

/* for CoAP example*/
#define CONFIG_EXAMPLE_COAP              0

/* For aj_basic_example */
#define CONFIG_EXAMPLE_AJ_BASIC          0

/*For aj_ameba_led example*/
#define CONFIG_EXAMPLE_AJ_AMEBA_LED      0

/* For WIFI GET BEACON FRAME example */
#define CONFIG_EXAMPLE_GET_BEACON_FRAME  0

/* For WIFI MAC MONITOR example */
#define CONFIG_EXAMPLE_WIFI_MAC_MONITOR  0
   
/* For HTTP CLIENT example */
#define CONFIG_EXAMPLE_HTTP_CLIENT  0

/* For MQTT example */
#define CONFIG_EXAMPLE_MQTT				0

/* For WiGadget example */
#define CONFIG_EXAMPLE_WIGADGET			0

/*For google nest example*/
#define CONFIG_EXAMPLE_GOOGLE_NEST		0

/* For mDNS example */
#define CONFIG_EXAMPLE_MDNS				0

/* For multicast example */
#define CONFIG_EXAMPLE_MCAST			0

/* For XML example */
#define CONFIG_EXAMPLE_XML				0

/* For JSON example */
#define CONFIG_EXAMPLE_CJSON			0

/* For socket select example */
#define CONFIG_EXAMPLE_SOCKET_SELECT	0

/* For socket nonblocking connect example */
#define CONFIG_EXAMPLE_NONBLOCK_CONNECT	0

/* For socket TCP bidirectional transmission example */
#define CONFIG_EXAMPLE_SOCKET_TCP_TRX	0

/* For smtp example */
#define CONFIG_EXAMPLE_SMTP	        0

/* For ssl download example */
#define CONFIG_EXAMPLE_SSL_DOWNLOAD      0

/* For http download example */
#define CONFIG_EXAMPLE_HTTP_DOWNLOAD	0

/* For httpc example */
#define CONFIG_EXAMPLE_HTTPC			0

/* For httpd example */
#define CONFIG_EXAMPLE_HTTPD			0

/* For http2 example */
#define CONFIG_EXAMPLE_HTTP2_CLIENT     0

/* For tcp keepalive example */
#define CONFIG_EXAMPLE_TCP_KEEPALIVE	0

/* For sntp show time example */
#define CONFIG_EXAMPLE_SNTP_SHOWTIME	0

/* For pppoe example */
#define CONFIG_EXAMPLE_PPPOE            0

/* For websocket client example */
#define CONFIG_EXAMPLE_WEBSOCKET_CLIENT		0

/* For websocket server example */
#define CONFIG_EXAMPLE_WEBSOCKET_SERVER		0

/*For Audio example */
#define CONFIG_EXAMPLE_AUDIO			0
#if CONFIG_EXAMPLE_AUDIO
#define FATFS_DISK_SD 	1
#define CONFIG_EXAMPLE_CODEC_SGTL5000         1
#endif

/* For audio mp3 pcm example */
#define CONFIG_EXAMPLE_AUDIO_MP3		0
#if CONFIG_EXAMPLE_AUDIO_MP3
#define FATFS_DISK_SD 	1
#define CONFIG_EXAMPLE_MP3_STREAM_SGTL5000      1
#endif

#define CONFIG_EXAMPLE_AUDIO_HELIX_MP3	0

#define CONFIG_EXAMPLE_AUDIO_HELIX_AAC	0

/*Foe alc audio codec firmware upgrade */
#define CONFIG_EXAMPLE_ALC_CODEC_FW_UPGRADE     0

/*Foe audio pcm upload */
#define CONFIG_EXAMPLE_AUDIO_PCM_UPLOAD     0

/* For UART Module AT command example */
#define CONFIG_EXAMPLE_UART_ATCMD	0
#if CONFIG_EXAMPLE_UART_ATCMD
#undef FREERTOS_PMU_TICKLESS_PLL_RESERVED
#define FREERTOS_PMU_TICKLESS_PLL_RESERVED  1
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE 1
#undef CONFIG_TRANSPORT
#define CONFIG_TRANSPORT 1
#undef LOG_SERVICE_BUFLEN
#define LOG_SERVICE_BUFLEN 1600
#undef CONFIG_LOG_SERVICE_LOCK
#define CONFIG_LOG_SERVICE_LOCK 1
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT  0
#endif

/* For SPI Module AT command example */
#define CONFIG_EXAMPLE_SPI_ATCMD 0

#if CONFIG_EXAMPLE_SPI_ATCMD
#undef FREERTOS_PMU_TICKLESS_PLL_RESERVED
#define FREERTOS_PMU_TICKLESS_PLL_RESERVED  1
#undef CONFIG_OTA_UPDATE
#define CONFIG_OTA_UPDATE 1
#undef CONFIG_TRANSPORT
#define CONFIG_TRANSPORT 1
#undef LOG_SERVICE_BUFLEN
#define LOG_SERVICE_BUFLEN 1600
#undef CONFIG_LOG_SERVICE_LOCK
#define CONFIG_LOG_SERVICE_LOCK 1
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT  0
#endif

#define CONFIG_EXAMPLE_MEDIA_FRAMEWORK					        0
#if CONFIG_EXAMPLE_MEDIA_FRAMEWORK
#define FATFS_DISK_SD								1
#define ISP_BOOT_MODE_ENABLE                                                    0
#if ISP_BOOT_MODE_ENABLE
#define ISP_BOOT_MP4                                                            0
#define ISP_BOOT_RTSP                                                           0
#define ISP_BOOT_MUX                                                            0
#if (ISP_BOOT_MP4 == 1 && ISP_BOOT_RTSP == 1 && ISP_BOOT_MUX == 1) || (ISP_BOOT_MP4 == 0 && ISP_BOOT_RTSP == 0 && ISP_BOOT_MUX == 0)
#error "It only can select the mp4 or rtsp"
#endif
#endif
#endif
//
#define CONFIG_EXAMPLE_MEDIA_FRAMEWORK_SD_DETECT					        0
#if CONFIG_EXAMPLE_MEDIA_FRAMEWORK_SD_DETECT
#define FATFS_DISK_SD								1
#define ISP_BOOT_MODE_ENABLE                                                    0
#if ISP_BOOT_MODE_ENABLE
#define ISP_BOOT_MP4                                                            0
#define ISP_BOOT_RTSP                                                           0
#define ISP_BOOT_MUX                                                            0
#if (ISP_BOOT_MP4 == 1 && ISP_BOOT_RTSP == 1 && ISP_BOOT_MUX == 1) || (ISP_BOOT_MP4 == 0 && ISP_BOOT_RTSP == 0 && ISP_BOOT_MUX == 0)
#error "It only can select the mp4 or rtsp"
#endif
#endif
#endif

#define CONFIG_EXAMPLE_MEDIA_MP4_DEMUXER					0
#ifdef  CONFIG_EXAMPLE_MEDIA_MP4_DEMUXER
#define FATFS_DISK_SD								1
#endif  

#define CONFIG_EXAMPLE_MEDIA_UVCH						0
//Note: When UVC-Host is enabled, please also enable lib_usbh.a and disable lib_usbd.a

#define CONFIG_EXAMPLE_MEDIA_UVCD						0
#define UVCD_DUAL                                       0

#define CONFIG_SDCARD_UPLOAD_HTTPD						0
#if CONFIG_SDCARD_UPLOAD_HTTPD
#define FATFS_DISK_SD								1
#endif

#define CONFIG_EXAMPLE_ISP_OSD_MULTI								0    
#define CONFIG_EXAMPLE_MASK								0   
#define CONFIG_EXAMPLE_MOTION_DETECT								0
     
#define CONFIG_MEDIA_H264_TO_SDCARD						0
#if CONFIG_MEDIA_H264_TO_SDCARD
#define FATFS_DISK_SD								1
#endif

#define CONFIG_MEDIA_AMEBACAM_APP_BROADCAST				0

//Defines for mp3 streaming over wifi, default output through alc5651
#define CONFIG_EXAMPLE_MP3_STREAM_RTP                           0

#if CONFIG_EXAMPLE_MP3_STREAM_RTP
#undef CONFIG_EXAMPLE_MEDIA_AUDIO_FROM_RTP
#define CONFIG_EXAMPLE_MEDIA_AUDIO_FROM_RTP             1
#endif

#define CONFIG_PRO_AUDIO_STREAM_RTP                             0
#if CONFIG_PRO_AUDIO_STREAM_RTP
#undef CONFIG_EXAMPLE_MEDIA_AUDIO_FROM_RTP
#define CONFIG_EXAMPLE_MEDIA_AUDIO_FROM_RTP             1
#undef	CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG	0
//Set this flag to 1 in case sgtl5000 to be used else alc5651 will be used
#define CONFIG_EXAMPLE_MP3_STREAM_SGTL5000                      0
#endif

// Use media source/sink example
#if defined(CONFIG_EXAMPLE_MEDIA_SS) || defined(CONFIG_EXAMPLE_MEDIA_MS)
#if (CONFIG_EXAMPLE_MEDIA_SS==1) || (CONFIG_EXAMPLE_MEDIA_MS==1)
#undef CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG		0
#define CONFIG_ENABLE_WPS	0
#endif
#endif
/* For ISP AT COMMAND config*/
#define CONFIG_ISP 1

/* For Mjpeg capture example*/
#define CONFIG_EXAMPLE_MJPEG_CAPTURE		0
#if CONFIG_EXAMPLE_MJPEG_CAPTURE
#define FATFS_DISK_SD 	1
#endif

/* For DCT example*/
#define CONFIG_EXAMPLE_DCT				0

/* For OTA over SD card example */
#define CONFIG_EXAMPLE_OTA_SDCARD		0
#if CONFIG_EXAMPLE_OTA_SDCARD
#define FATFS_DISK_SD 	1
#endif

/****************** For EAP method example *******************/
#define CONFIG_EXAMPLE_EAP	0

// on/off specified eap method
#define CONFIG_ENABLE_PEAP	0
#define CONFIG_ENABLE_TLS	0
#define CONFIG_ENABLE_TTLS	0

// optional feature: whether to verify the cert of radius server
#define ENABLE_EAP_SSL_VERIFY_SERVER	0

#if CONFIG_ENABLE_PEAP || CONFIG_ENABLE_TLS || CONFIG_ENABLE_TTLS
#define CONFIG_ENABLE_EAP
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0
#endif

#if CONFIG_ENABLE_TLS
#define ENABLE_EAP_SSL_VERIFY_CLIENT	1
#else
#define ENABLE_EAP_SSL_VERIFY_CLIENT	0
#endif
/******************End of EAP configurations*******************/

/* For usb mass storage example */
#define CONFIG_EXAMPLE_USB_MASS_STORAGE		0
/* For vendor specific example  */
#define CONFIG_EXAMPLE_USB_VENDOR_SPECIFIC      0

#define CONFIG_EXAMPLE_USB_ISOC_DEVICE          0   

/* For FATFS example*/
#define CONFIG_EXAMPLE_FATFS			0
#if CONFIG_EXAMPLE_FATFS
#define CONFIG_FATFS_EN	1
#if CONFIG_FATFS_EN
// fatfs version
#define FATFS_R_10C
// fatfs disk interface
#define FATFS_DISK_USB	0 // Not supported on AmebaPro
#define FATFS_DISK_SD 	1 // In AmebaPro C-cut QFN128, GPIOE_12 is low active to used to control SDIO power.
#define FATFS_DISK_FLASH 0
#endif
#endif

/* For STD FILE example*/
#define CONFIG_EXAMPLE_STD_FILE			0
#if CONFIG_EXAMPLE_STD_FILE
#define CONFIG_FATFS_EN	1
#if CONFIG_FATFS_EN
#define FATFS_R_10C
#define FATFS_DISK_SD 1
#endif
#endif

/* For iNIC host example*/
#ifdef CONFIG_INIC_GSPI_HOST //this flag is defined in IAR project
#define	CONFIG_EXAMPLE_INIC_GSPI_HOST	1
#if CONFIG_EXAMPLE_INIC_GSPI_HOST

#define CONFIG_INIC_HOST	1

#undef CONFIG_WLAN
#define CONFIG_WLAN		0
#undef	CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG	0
#undef	CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	0
#undef CONFIG_LWIP_LAYER
#define CONFIG_LWIP_LAYER	1
#undef	CONFIG_BSD_TCP
#define CONFIG_BSD_TCP		1

#endif
#endif

/*For uart update example*/
#define CONFIG_UART_UPDATE				0
#if CONFIG_UART_UPDATE
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT 0
#endif


/*For arduino wifi shield example */
#define CONFIG_EXAMPLE_ARDUINO_WIFI	0
#if CONFIG_EXAMPLE_ARDUINO_WIFI
#undef CONFIG_WIFI_NORMAL
#endif

/* For uart adapter example */
/* Please also configure LWIP_UART_ADAPTER to 1 
in lwip_opt.h for support uart adapter*/
#define CONFIG_EXAMPLE_UART_ADAPTER	0
#if CONFIG_EXAMPLE_UART_ADAPTER
#undef CONFIG_EXAMPLE_WLAN_FAST_CONNECT
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	1
#undef CONFIG_EXAMPLE_MDNS
#define CONFIG_EXAMPLE_MDNS	1
#undef FREERTOS_PMU_TICKLESS_PLL_RESERVED
#define FREERTOS_PMU_TICKLESS_PLL_RESERVED  1
#endif
#if CONFIG_JD_SMART
#if (CONFIG_ENABLE_WPS == 1)
#define CONFIG_ENABLE_WPS	0
#endif
#if (CONFIG_INCLUDE_SIMPLE_CONFIG == 1)
#define CONFIG_INCLUDE_SIMPLE_CONFIG	0
#endif
#if (CONFIG_EXAMPLE_WLAN_FAST_CONNECT == 1)
#define CONFIG_EXAMPLE_WLAN_FAST_CONNECT	0
#endif
#endif

/* For wifi scenarios example (Wi-Fi, WPS enrollee, P2P GO) */
// also need to enable WPS and P2P
#define CONFIG_EXAMPLE_WLAN_SCENARIO	0
	 
/* For broadcast example */
#define CONFIG_EXAMPLE_BCAST			0

/* For high-load memory use case memory usage */
#define CONFIG_EXAMPLE_HIGH_LOAD_MEMORY_USE		0

/* For rarp example */
#define CONFIG_EXAMPLE_RARP				0

/* For ssl server example */
#define CONFIG_EXAMPLE_SSL_SERVER		0

/*For secure boot example */
#define CONFIG_EXAMPLE_SECURE_BOOT 0

/*For timelapse example */
#define CONFIG_EXAMPLE_TIMELAPSE 0

#if CONFIG_EXAMPLE_TIMELAPSE
#define CONFIG_USE_HTTP_SERVER  0
#if CONFIG_USE_HTTP_SERVER
#undef CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG		0
#define CONFIG_ENABLE_WPS	0
#else
#undef CONFIG_INCLUDE_SIMPLE_CONFIG
#define CONFIG_INCLUDE_SIMPLE_CONFIG		0
#define CONFIG_ENABLE_WPS	0
#define CONFIG_FATFS_EN	1
#define FATFS_R_10C
#define FATFS_DISK_SD 	1
#endif  
#endif 

/* For ota update http example */
#define CONFIG_EXAMPLE_OTA_HTTP			0

/* For Amazon AWS IoT example */
#define CONFIG_EXAMPLE_AMAZON_AWS_IOT 0

#define CONFIG_EXAMPLE_AMAZON_ALEXA   0

/*For wifi roaming example*/
#define CONFIG_EXAMPLE_WIFI_ROAMING		0

/*For cm backtrace example*/
#define CONFIG_EXAMPLE_CM_BACKTRACE		0

#if CONFIG_QQ_LINK
#define FATFS_R_10C
#define FATFS_DISK_USB	0
#define FATFS_DISK_SD 	1
#endif

#if CONFIG_ENABLE_WPS
#define WPS_CONNECT_RETRY_COUNT		4
#define WPS_CONNECT_RETRY_INTERVAL	5000 // in ms
#endif 

#define AUTO_RECONNECT_COUNT	8
#define AUTO_RECONNECT_INTERVAL	5 // in sec

/* For QR code scanner example */
#define CONFIG_EXAMPLE_QR_CODE_SCANNER		0
   
/* For doorbell example */
#define CONFIG_EXAMPLE_DOORBELL		0
#if CONFIG_EXAMPLE_DOORBELL   
#define CONFIG_LIGHT_SENSOR 1 // NOTE: enable it only when with light sensor
#define FATFS_DISK_SD								1
#define ISP_BOOT_MODE_ENABLE                                                    1   
#define CONFIG_MEDIA_AMEBACAM_APP_BROADCAST					1
#endif   
   
#endif

/* For ActionCAM example */
#define CONFIG_EXAMPLE_ACTIONCAM    0
#if CONFIG_EXAMPLE_ACTIONCAM
#define SDCARD_OTA_UPDATE
#define FATFS_DISK_SD               1
#endif

#define CONFIG_USB_DFU_OTA          0

#define CONFIG_EXAMPLE_SD_HOT_PLUG  0
#if CONFIG_EXAMPLE_SD_HOT_PLUG
#define CONFIG_FATFS_EN	1
#if CONFIG_FATFS_EN
// fatfs version
#define FATFS_R_10C
// fatfs disk interface
#define FATFS_DISK_SD 	1 // In AmebaPro C-cut QFN128, GPIOE_12 is low active to used to control SDIO power.
#endif
#endif

/* For KVS WebRTC example*/
#define CONFIG_EXAMPLE_KVS_WEBRTC               0
#if CONFIG_EXAMPLE_KVS_WEBRTC
#define CONFIG_FATFS_EN	1
#if CONFIG_FATFS_EN
// fatfs disk interface
#define FATFS_DISK_SD 	1 // In AmebaPro C-cut QFN128, GPIOE_12 is low active to used to control SDIO power.
#endif
#endif

/* For KVS Producer example*/
#define CONFIG_EXAMPLE_KVS_PRODUCER             0

/* For opus encoder eample*/
#define CONFIG_EXAMPLE_AUDIO_OPUS_ENCODE        0
#if CONFIG_EXAMPLE_AUDIO_OPUS_ENCODE
#define FATFS_DISK_SD 	1
#endif

/* For audio loop eample*/
#define CONFIG_EXAMPLE_AUDIO_LOOP               0
   
/* For audio to G711 eample*/
#define CONFIG_EXAMPLE_AUDIO_TO_G711            0
   
/* For audio to opus eample*/
#define CONFIG_EXAMPLE_AUDIO_TO_OPUS            0

/* For audio to AAC eample*/
#define CONFIG_EXAMPLE_AUDIO_TO_AAC             0

/* For Amazon FreeRTOS SDK example */
#define CONFIG_EXAMPLE_AMAZON_FREERTOS          0

#define CONFIG_EXAMPLE_AMAZON_AFQP_TESTS        0