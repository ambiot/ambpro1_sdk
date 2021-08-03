This example is used to demo Amazon KVS(Kinesis Video Streams) WebRTC

The example will be contributed to https://github.com/aws-samples/amazon-kinesis-video-streams-webrtc-sdk-c-for-freertos
demo code will be downloaded in submodule, you can find it in <SDK>/lib_amazon/amazon-kinesis-video-streams-webrtc-sdk-c-for-freertos/main_amebapro

Please follow the instructions for various options

	1. Set the parameter CONFIG_EXAMPLE_KVS_WEBRTC to 1 in "platform_opts.h" file 

	2. Check your camera sensor model in sensor.h: 

		#define SENSOR_USE      	SENSOR_XXXX

	3. Configure AWS KVS service setting in "<SDK>/lib_amazon/amazon-kinesis-video-streams-webrtc-sdk-c-for-freertos/main_amebapro/example_kvs_webrtc.h"

See AmebaPro_Amazon_KVS_WebRTC_Getting_Started_Guide.pdf for more information.


[Supported List] 
	Supported :
	    Ameba-pro