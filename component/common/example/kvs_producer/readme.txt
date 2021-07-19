This example is used to demo Amazon KVS(Kinesis Video Streams) Producer

The example has been contributed to https://github.com/aws-samples/amazon-kinesis-video-streams-producer-embedded-c
demo code will be downloaded in submodule, you can find it in <SDK>/lib_amazon/amazon-kinesis-video-streams-producer-embedded-c/samples/kvs-amebapro

Please follow the instructions for various options

	1. Set the parameter CONFIG_EXAMPLE_KVS_PRODUCER to 1 in "platform_opts.h" file 

	2. Check your camera sensor model in sensor.h: 

		#define SENSOR_USE      	SENSOR_XXXX

	3. Configure AWS KVS service setting in "sample_config.h"

See AmebaPro_Amazon_KVS_Producer_Getting_Started_Guide.pdf for more information.
        

[Supported List] 
	Supported :
	    Ameba-pro