﻿# AmebaPro Introduction 

AmebaPro is one of the Ameba IoT Family System On Chip which integrates the following features 

<ul class="simple">
<li><p>Wi-Fi 802.11ac/n</p></li>
<li><p>H.264</p></li>
<li><p>Image Signal Processing/ISP</p></li>
<li><p>Audio Codec</p></li>
<li><p>SD Storage</p></li>
</ul>

AmebaPro is based on the ARM cortex v8m up to 300MHz with 32/64MB LPDDR. The high performance capability enables the comparatively complicated video and vision application, while still be a best choice for battery-based product. The highly integrated solution makes it possible to have a smaller form factor design, which is more cost effective and has rich potentiality.

It has FreeRTOS running on it, and provides standard BSD socket and Posix interface for easier open sources integration. The simplicity of RTOS architecture achieves fast boot capability, which is a big benefit from both key product feature, and power consumption point of view.

By integration of image sensing and communication technology, AmebaPro transmits the video or picture to the cloud, which then bridges the back-end AI service for computer vision application. It realizes a lightweight IoT device to have intelligent vision capability.


# AmebaPro with KVS WebRTC and Producer
This demonstrates how to use KVS Producer and WebRTC on AmebaPro.
<a href="https://www.amebaiot.com/zh/amebapro/">
  <img src="https://img.shields.io/badge/Realtek%20IoT-AmebaPro-blue" valign="middle" alt="test image size" height="15%" width="15%"/>
</a>  

![AmebaPro_RTOS_Amazon_Eco_System](https://user-images.githubusercontent.com/63438091/134361259-9d845184-8b54-4c79-a087-0b3447aa9e42.jpg)

Amazon FreeRTOS-LTS libraries are supported on AmebaPro now, it can be used to communicate with AWS IoT core easily!
[Version 202012.00](https://github.com/aws/amazon-freertos/tree/202012.00)

## Brief Introduction to Amazon KVS and FreeRTOS-LTS  
:blue_book: **Amazon Kinesis Video Streams**  
&nbsp; include two main services, which are Producer and WebRTC
<a href="https://aws.amazon.com/kinesis/video-streams/?nc1=h_ls&amazon-kinesis-video-streams-resources-blog.sort-by=item.additionalFields.createdDate&amazon-kinesis-video-streams-resources-blog.sort-order=desc">
  <img src="https://img.shields.io/badge/-Link-lightgrey" valign="middle" alt="test image size" height="4%" width="4%"/>
</a>  

:blue_book: **KVS Producer**  
&nbsp; stream live video from devices to the AWS Cloud, or build applications for batch-oriented video analytics.
<a href="https://docs.aws.amazon.com/kinesisvideostreams/latest/dg/what-is-kinesis-video.html">
  <img src="https://img.shields.io/badge/-Link-lightgrey" valign="middle" alt="test image size" height="4%" width="4%"/>
</a>  

:blue_book: **KVS WebRTC**  
&nbsp; enable real-time communication (RTC) across browsers and mobile applications via simple APIs.
<a href="https://docs.aws.amazon.com/kinesisvideostreams-webrtc-dg/latest/devguide/what-is-kvswebrtc.html">
  <img src="https://img.shields.io/badge/-Link-lightgrey" valign="middle" alt="test image size" height="4%" width="4%"/>
</a>  

:blue_book: **FreeRTOS-LTS Libraries**  
&nbsp; provides feature stability, and security patches and critical bug fixes for two years from release date.
<a href="https://www.freertos.org/lts-libraries.html">
  <img src="https://img.shields.io/badge/-Link-lightgrey" valign="middle" alt="test image size" height="4%" width="4%"/>
</a>

## Applications on AmebaPro  
### :bulb: AI Face Detection
&nbsp; AmebaPro(Producer) + Rekognition(AWS AI) = video cloud storage and content analytics :cloud:  
<table width="100%" table-layout:fixed>
  <tr>
    <td width=50%><img src="doc/photo/face_detection.jpg" valign="middle" alt="test image size" height=233px width=400px/></td>
    <td align=Left width=50%>
      1. Stream live video from AmebaPro to KVS<BR><BR>2. AI face detection by using AWS Rekognition API<BR><BR>&nbsp; :point_right: &nbsp;
      <a href="https://github.com/ambiot/ambpro1_sdk/blob/main/doc/AmebaPro_Amazon_KVS_Producer_Getting_Started_Guide_v1.1_r.pdf">
        <img src="https://img.shields.io/badge/-Getting%20Started-green" valign="middle" height=25px width=120px/>
      </a>
    </td>
  </tr>
</table>  

### :bulb: Real-time communucation  
&nbsp; AmebaPro(WebRTC master) + Viewer(WebRTC Client) = real-time communication, 1-way video & 2-way audio  
<table width="100%" table-layout:fixed>
  <tr>
    <td width=50%><img src="doc/photo/p2p.jpg" valign="middle" alt="test image size" height=206px width=400px/></td>
    <td align=Left width=50%>
      1. AmebaPro start a signaling channel, as a master<BR><BR>2. Browser run as a viewer (client)<BR><BR>3. Start a P2P connection<BR><BR>&nbsp; :point_right: &nbsp;
      <a href="https://github.com/ambiot/ambpro1_sdk/blob/main/doc/AmebaPro_Amazon_KVS_WebRTC_Getting_Started_Guide_v1.1_r.pdf">
        <img src="https://img.shields.io/badge/-Getting%20Started-green" valign="middle" height=25px width=120px/>
      </a>
    </td>
  </tr>
</table>

### :bulb: Object Detection
&nbsp; AmebaPro(image uploader) + Rekognition(AWS AI) = object detection and classification :cloud:  
<table style="width:100%; table-layout:fixed">
  <tr>
    <td width=50%><img src="doc/photo/object_detection.jpg" valign="middle" alt="test image size" height=225px width=400px/></td>
    <td align=Left width=50%>
      1. Take a snapshot by camera sensor<BR><BR>2. Upload image from AmebaPro to S3<BR><BR>3. Trigger Lambda function to do object detection<BR><BR>&nbsp; :point_right: &nbsp;
      <a href="https://github.com/ambiot/ambpro1_sdk/blob/main/doc/AmebaPro_AWS_S3+Rekognition_Getting_Started_Guide_v1.2_r.pdf">
        <img src="https://img.shields.io/badge/-Getting%20Started-green" valign="middle" height=25px width=120px/>
      </a>
      <BR><BR>Notes: Based on FreeRTOS-v202012-LTS framework
    </td>
  </tr>
</table>  
      
### :bulb: FreeRTOS-LTS-v202012.00 Libraries
&nbsp; AmebaPro can connect to AWS IoT with the long term support libraries maintained by Amazon.
<a href="https://github.com/ambiot/ambpro1_sdk/blob/main/doc/AmebaPro_Amazon_FreeRTOS-LTS_Getting_Started_Guide_v1.2_r.pdf">
  <img src="https://img.shields.io/badge/-Getting%20Started-green" valign="middle" height=25px width=120px/>
</a>  
&nbsp; [FreeRTOS demos](https://docs.aws.amazon.com/freertos/latest/userguide/freertos-next-steps.html): coreHTTP, coreMQTT, Over-the-air updates(OTA), AWS IoT Device Shadow...  

## Demo code  
:pencil2: **KVS WebRTC** &nbsp; &nbsp;`lib_amazon/amazon-kinesis-video-streams-webrtc-sdk-c-for-freertos/main_amebapro`  

:pencil2: **KVS Producer** &nbsp; &nbsp;`lib_amazon/amazon-kinesis-video-streams-producer-embedded-c/samples/kvs-amebapro`  

:pencil2: **S3 file upload** &nbsp; &nbsp;`project/component/common/application/amazon/JPEG_snapshot_s3_upload_example`  

:pencil2: **FreeRTOS-LTS** &nbsp; &nbsp;`lib_amazon/amazon-freertos-202012.00/demos`  


## IDE/toolchain Requirement

:hammer: **IAR Embedded Workbench - IAR Systems**  
&nbsp; Please use IAR version 8.3  

:hammer: **GCC toolchain**  
&nbsp; Linux: asdk-6.4.1-linux-newlib-build-3026-x86_64  
&nbsp; Cygwin: asdk-6.4.1-cygwin-newlib-build-2778-i686  


## Clone Project  
To check out this repository:  

```
git clone --recurse-submodules https://github.com/ambiot/ambpro1_sdk.git
```

If you already have a checkout, run the following command to sync submodules recursively:

```
git submodule update --init --recursive
```

If there is GCC makefile error like: "No rule to make target …", it may mean that some codes have not been downloaded correctly.  
Please run the above command again to download the missing codes.  

If there is compile error with shell script in "component/soc/realtek/8195b/misc/gcc_utility/", you may need to run following command  
```
dos2unix component/soc/realtek/8195b/misc/gcc_utility/*
```
