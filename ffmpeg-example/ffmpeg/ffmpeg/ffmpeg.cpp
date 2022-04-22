﻿// ffmpeg.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/dict.h"
#include "libavdevice/avdevice.h"
#include "libavutil/log.h"
#include "libavutil/error.h"
#include "libavformat/avio.h"
#include "libavutil/mathematics.h"
#include "libavutil/time.h"
#include "libavutil/timestamp.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
};

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")

#define MAX_AUDIO_FRAME_SIZE 192000

void rec_cam()
{
	int nRet = 0;
	char errors[1024] = {};
	avdevice_register_all(); //注册输入设备
	//windows下指定摄像头信息
	AVInputFormat* pIptFmt = av_find_input_format("dshow");
	if (!pIptFmt) {
		av_strerror(nRet, errors, 1024);
		fprintf(stderr, "Failed to open video device, [%d]%s\n", nRet, errors);
		printf("av_find_input_format fail");
	}
	AVFormatContext* pFmtCtx = nullptr;
	const char* devName = "video=ov9734_techfront_camera";
	AVDictionary* options = nullptr;
	av_dict_set(&options, "video_size", "640x480", 0);//根据红框中摄像头参数设置
	av_dict_set(&options, "framerate", "30", 0);//根据红框中摄像头参数设置
	av_dict_set(&options, "pixel_format", "yuyv422", 0);//根据红框中摄像头参数设置
	nRet = avformat_open_input(&pFmtCtx, devName, pIptFmt, &options);
	if (nRet < 0)
	{
		av_strerror(nRet, errors, 1024);
		fprintf(stderr, "Failed to open video device, [%d]%s\n", nRet, errors);
		printf("avformat_open_input fail");
	}
	FILE* pOutFile = fopen("./cam.yuv", "wb+");
	AVPacket pkt;
	av_init_packet(&pkt);
	while (0 == av_read_frame(pFmtCtx, &pkt))
	{
		fwrite(pkt.data, 1, pkt.size, pOutFile);
		fflush(pOutFile);
		av_packet_unref(&pkt);
	}
}

int FFmpegTest() {
	AVFormatContext* pFormatCtx = NULL;
	AVCodecContext* pCodecCtx = NULL;
	AVCodec* pCodec;
	AVDictionaryEntry* dict = NULL;

	int iHour, iMinute, iSecond, iTotalSeconds;//HH:MM:SS
	int videoIndex, audioIndex;

	const char* fileName = "qwe.mp4";
	//char *fileName = "Titanic.ts";

	av_register_all();//注册所有组件

	if (avformat_open_input(&pFormatCtx, fileName, NULL, NULL) != 0)//打开输入视频文件
	{
		printf("Couldn't open input stream.\n");
		return -1;
	}

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("Couldn't find stream information.\n");
		return -1;
	}

	videoIndex = -1;
	for (int i = 0; i < pFormatCtx->nb_streams/*视音频流的个数*/; i++)
	{
		if (pFormatCtx->streams[i]/*视音频流*/->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)//查找音频
		{
			videoIndex = i;
			break;
		}
	}
	if (videoIndex == -1)
	{
		printf("Couldn't find a video stream.\n");
		return -1;
	}

	/**
	*  不赞成这样使用
	*  pCodecCtx = pFormatCtx->streams[videoIndex]->codec;	//指向AVCodecContext的指针
	*/

	pCodecCtx = avcodec_alloc_context3(NULL);
	if (pCodecCtx == NULL)
	{
		printf("Could not allocate AVCodecContext\n");
		return -1;
	}
	avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoIndex]->codecpar);

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);	//指向AVCodec的指针.查找解码器
	if (pCodec == NULL)
	{
		printf("Codec not found.\n");
		return -1;
	}
	//打开解码器
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("Could not open codec.\n");
		return -1;
	}

	audioIndex = -1;
	for (int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioIndex = i;
			break;
		}
	}
	if (audioIndex == -1)
	{
		printf("Couldn't find a audio stream.\n");
		return -1;
	}



	//打印结构体信息

	puts("AVFormatContext信息：");
	puts("---------------------------------------------");
	printf("文件名：%s\n", pFormatCtx->filename);
	iTotalSeconds = (int)pFormatCtx->duration/*微秒*/ / 1000000;
	iHour = iTotalSeconds / 3600;//小时
	iMinute = iTotalSeconds % 3600 / 60;//分钟
	iSecond = iTotalSeconds % 60;//秒
	printf("持续时间：%02d:%02d:%02d\n", iHour, iMinute, iSecond);
	printf("平均混合码率：%d kb/s\n", pFormatCtx->bit_rate / 1000);
	printf("视音频个数：%d\n", pFormatCtx->nb_streams);
	puts("---------------------------------------------");

	puts("AVInputFormat信息:");
	puts("---------------------------------------------");
	printf("封装格式名称：%s\n", pFormatCtx->iformat->name);
	printf("封装格式长名称：%s\n", pFormatCtx->iformat->long_name);
	printf("封装格式扩展名：%s\n", pFormatCtx->iformat->extensions);
	printf("封装格式ID：%d\n", pFormatCtx->iformat->raw_codec_id);
	puts("---------------------------------------------");

	puts("AVStream信息:");
	puts("---------------------------------------------");
	printf("视频流标识符：%d\n", pFormatCtx->streams[videoIndex]->index);
	printf("音频流标识符：%d\n", pFormatCtx->streams[audioIndex]->index);
	printf("视频流长度：%d微秒\n", pFormatCtx->streams[videoIndex]->duration);
	printf("音频流长度：%d微秒\n", pFormatCtx->streams[audioIndex]->duration);
	puts("---------------------------------------------");

	puts("AVCodecContext信息:");
	puts("---------------------------------------------");
	printf("视频码率：%d kb/s\n", pCodecCtx->bit_rate / 1000);
	printf("视频大小：%d * %d\n", pCodecCtx->width, pCodecCtx->height);
	puts("---------------------------------------------");

	puts("AVCodec信息:");
	puts("---------------------------------------------");
	printf("视频编码格式：%s\n", pCodec->name);
	printf("视频编码详细格式：%s\n", pCodec->long_name);
	puts("---------------------------------------------");

	printf("视频时长：%d微秒\n", pFormatCtx->streams[videoIndex]->duration);
	printf("音频时长：%d微秒\n", pFormatCtx->streams[audioIndex]->duration);
	printf("音频采样率：%d\n", pFormatCtx->streams[audioIndex]->codecpar->sample_rate);
	printf("音频信道数目：%d\n", pFormatCtx->streams[audioIndex]->codecpar->channels);

	puts("AVFormatContext元数据：");
	puts("---------------------------------------------");
	while (dict = av_dict_get(pFormatCtx->metadata, "", dict, AV_DICT_IGNORE_SUFFIX))
	{
		printf("[%s] = %s\n", dict->key, dict->value);
	}
	puts("---------------------------------------------");

	puts("AVStream视频元数据：");
	puts("---------------------------------------------");
	dict = NULL;
	while (dict = av_dict_get(pFormatCtx->streams[videoIndex]->metadata, "", dict, AV_DICT_IGNORE_SUFFIX))
	{
		printf("[%s] = %s\n", dict->key, dict->value);
	}
	puts("---------------------------------------------");

	puts("AVStream音频元数据：");
	puts("---------------------------------------------");
	dict = NULL;
	while (dict = av_dict_get(pFormatCtx->streams[audioIndex]->metadata, "", dict, AV_DICT_IGNORE_SUFFIX))
	{
		printf("[%s] = %s\n", dict->key, dict->value);
	}
	puts("---------------------------------------------");


	av_dump_format(pFormatCtx, -1, fileName, 0);
	printf("\n\n编译信息：\n%s\n\n", avcodec_configuration());

	avcodec_free_context(&pCodecCtx);
	//avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
}

#define ADTS_HEAD_LEN 7

void adts_header(char* szAdtsHeader, int dataLen) {

	int audio_object_type = 2; //通过av_dump_format显示音频信息或者ffplay获取多媒体文件的音频流编码acc（LC），对应表格中Object Type ID -- 2
	int sampling_frequency_index = 4; //音频信息中采样率为44100 Hz 对应采样率索引0x4
	int channel_config = 2; //音频信息中音频通道为双通道2

	int adtsLen = dataLen + 7;             //采用头长度为7字节，所以protection_absent=1   =0时为9字节，表示含有CRC校验码

	szAdtsHeader[0] = 0xff;         //syncword ：总是0xFFF, 代表一个ADTS帧的开始, 用于同步. 高8bits

	szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
	szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 : MPEG-4(mp4a),1 : MPEG-2  1bit
	szAdtsHeader[1] |= (0 << 1);    //Layer:0                                   2bits 
	szAdtsHeader[1] |= 1;           //protection absent:1  没有CRC校验            1bit

	szAdtsHeader[2] = (audio_object_type - 1) << 6;            //profile=(audio_object_type - 1) 表示使用哪个级别的AAC  2bits
	szAdtsHeader[2] |= (sampling_frequency_index & 0x0f) << 2; //sampling frequency index:sampling_frequency_index  4bits 
	szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
	szAdtsHeader[2] |= (channel_config & 0x04) >> 2;           //channel configuration:channel_config               高1bit

	szAdtsHeader[3] = (channel_config & 0x03) << 6;     //channel configuration:channel_config      低2bits
	szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
	szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit ----------------固定头完结，开始可变头
	szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit  
	szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
	szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);    //frame length：value                       高2bits  000|1 1000|0000 0000

	szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits             0000  0111 1111 1000

	szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低 3bits              0000  0000 0000 0111
	//number_of_raw_data_blocks_in_frame：表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧。
	//所以说number_of_raw_data_blocks_in_frame == 0 表示说ADTS帧中有一个AAC数据块。(一个AAC原始帧包含一段时间内1024个采样及相关数据)
	szAdtsHeader[5] |= 0x1f;  //buffer fullness:0x7ff 高5bits   0x7FF 说明是码率可变的码流 ---> 111 1111 1111 00----> 1 1111 1111 1100--->0x1f与0xfc                               

	szAdtsHeader[6] = 0xfc;
}

int GetAudioFromVideo(const char* src, const char *dst) {
	AVFormatContext* ifmt_ctx = nullptr;
	AVPacket pkt;
	FILE* pOutput = nullptr;
	int audio_index = 0;
	char adtsHeader[ADTS_HEAD_LEN] = "";
	int len = 0;
	int ret = 0;

	av_log_set_level(AV_LOG_INFO);
	av_register_all();

	ret = avformat_open_input(&ifmt_ctx, src, nullptr, nullptr);
	if (ret < 0) {
		printf("Failed to open input file!\n");
		av_log(nullptr, AV_LOG_ERROR, "Failed to open input file!\n");
		if (ifmt_ctx) {
			avformat_close_input(&ifmt_ctx);
			return -1;
		}
	}

	ret = avformat_find_stream_info(ifmt_ctx, nullptr);
	if (ret < 0) {
		printf("Failed to find stream info!\n");
		av_log(nullptr, AV_LOG_ERROR, "Failed to find stream info!\n");
		if (ifmt_ctx) {
			avformat_close_input(&ifmt_ctx);
			return -1;
		}
	}

	ret = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
	if (ret < 0) {
		printf("Failed to find stream!\n");
		av_log(nullptr, AV_LOG_ERROR, "Failed to find stream!\n");
		if (ifmt_ctx) {
			avformat_close_input(&ifmt_ctx);
			return -1;
		}
	}
	audio_index = ret;

	pOutput = fopen(dst, "wb+");
	if (!pOutput) {
		printf("Failed to create file!\n");
		return -1;
	}

	av_init_packet(&pkt);

	while (av_read_frame(ifmt_ctx, &pkt) >= 0) {
		if (pkt.stream_index == audio_index) {
			adts_header(adtsHeader, ADTS_HEAD_LEN);
			len = fwrite(adtsHeader, 1, ADTS_HEAD_LEN, pOutput);
			if (len != ADTS_HEAD_LEN) {
				printf("Failed to write adts header to file!\n");
				return -1;
			}

			len = fwrite(pkt.data, 1, pkt.size, pOutput);
			if (len != pkt.size) {
				printf("Failed to write data to file!\n");
				return -1;
			}
		}

		av_packet_unref(&pkt);
	}

	avformat_close_input(&ifmt_ctx);
	if (pOutput) {
		fclose(pOutput);
	}

	return 0;
}

// 这里是将前面的(startcode+SPS+PPS,size)+(NALU数据,size)传入函数中，使得函数在NALU 前面加入startcode;
// 这里的startcode不是4字节，而是3字节
static int alloc_and_copy(AVPacket * out, const uint8_t * sps_pps, uint32_t sps_pps_size,
                          const uint8_t * in, uint32_t in_size) {
	uint32_t offset = out->size;            // 偏移量，就是out已有数据的大小，后面再写入数据就要从偏移量处开始操作
	uint8_t start_code_size = sps_pps == NULL ? 3 : 4;   // 特征码的大小，SPS/PPS占4字节，其余占3字节
    int err;

    err = av_grow_packet(out, sps_pps_size + in_size + start_code_size); // 包扩容，在原来out已有数据基础上进行扩容，使得可以加入所有数据
    if (err < 0)
        return err;

    // 1.含有SPS、PPS数据，直接写入。注意：在SPS、PPS前面已经写入了start code
    if (sps_pps)
        memcpy(out->data + offset, sps_pps, sps_pps_size);  // 先写入SPS、PPS数据

    // 2.在NALU前面加入start code
    for (int i = 0; i < start_code_size; i++) {
        (out->data + offset + sps_pps_size)[i] = i == start_code_size - 1 ? 1 : 0;
	}
	
	// 3.最后将NALU数据拷贝过去
	memcpy(out->data + offset + sps_pps_size + start_code_size, in, in_size);
	
	return 0;
}

// 从AVCC中的extradata中获取SPS、PPS数据;此外由于SPS、PPS前面的start code为4字节，所以我们这里直接写入进去吧
// 注意：第4个参数padding，是表示AVBufferRef中填充的字节数。取决于ffmpeg版本，这里是64字节；
// 所以AVPacket中data大小最大字节数为INT_MAX - 64;而这里64被宏定义为AV_INPUT_BUFFER_PADDING_SIZE
int h264_extradata_to_annexb(const uint8_t * codec_extradata, const int codec_extradata_size, AVPacket * out_extradata, int padding)
{
	uint16_t unit_size;     // 读取两个字节，用来获取SPS、PPS的大小
	uint64_t total_size = 0;    // 用来记录从extradata中读取的全部SPS、PPS大小，
                                // 最后来验证大小不要超过AVPacket中data的大小限制（因为我们最后是将数据存放在AVPacket中返回的）
	
	uint8_t * out = NULL;// out:是一个指向一段内存的指针，这段内存用于存放所有拷贝的sps/pps数据和其特征码数据
	uint8_t unit_nb;    // unit_nb:sps/pps个数
	uint8_t sps_done = 0;   // 表示sps数据是否已经处理完毕，当sps_done为0,表示没有处理，处理完成后不为0
	uint8_t sps_seen = 0, sps_offset = 0;   // sps_seen：是否有sps数据 sps_offset：sps数据的偏移，为0
	uint8_t pps_seen = 0, pps_offset = 0;   // pps_seen：是否有pps数据 pps_offset：pps数据的偏移，因为pps数据在sps后面，
                                            // 所以其偏移就是所有sps数据长度+sps的特征码所占字节数
	
	static const uint8_t start_code[4] = { 0,0,0,1 }; // 记录start code
	
	const uint8_t * extradata = codec_extradata + 4; // 扩展数据的前4字节无用，跳过
	int length_size = ((*extradata++) & 0x3) + 1;     // 第一个字节的后两位存放NALULengthSizeMinusOne字段的值（0，1,2,3）+1=（1,2,3,4）
                                                      // 其中3不被使用;这个字段要被返回的
	
	sps_offset = pps_offset = -1;
	
	// 先获取SPS的个数 大小1字节，在后5位中
	unit_nb = (*extradata++) & 0x1f;
	if (!unit_nb) {   // 没有SPS
	    goto pps;   // 就直接去获取PPS数据
	}
	else {          // 有SPS数据
		sps_offset = 0; // SPS是最开始的，不需要偏移
		sps_seen = 1;   // 表示有SPS数据
	}
	
	while (unit_nb--) {   // 开始处理SPS、PPS类型的每一个数据，一般SPS、PPS都是1个
	    int err;
	    // 先读取2个字节的数据，用来表示SPS/PPS的数据长度
	    unit_size = (extradata[0] << 8) | extradata[1];
	    total_size += unit_size + 4;    // +4是加开始码,注意：total_size是累加了每一次获取SPS、PPS的数据量
	    if (total_size > INT_MAX - padding) { // 防止数据溢出AVPacket的data大小
	            av_log(NULL, AV_LOG_ERROR, "Too big extradata size, corrupted stream or invalid MP4/AVCC bitstream\n");
	            av_free(out);
	            return AVERROR(EINVAL);
	
	    }
	    // 判断数据是否越界
	    if (extradata + 2 + unit_size > codec_extradata + codec_extradata_size) {
	        av_log(NULL, AV_LOG_ERROR, "Packet header is not contained in global extradata, corrupted stream or invalid MP4/AVCC bitstream\n");
	        av_free(out);   // 释放前面的空间
	        return AVERROR(EINVAL);
	
	    }
	    // 开始为out指针分配空间
	    if ((err = av_reallocp(&out, total_size + padding)) < 0)  // reallocp是在原来空间上扩充，已经存在的数据不会被丢弃
	        return err;
	    memcpy(out + total_size - unit_size - 4, start_code, 4);    // 先拷贝start code到out中
	    memcpy(out + total_size - unit_size, extradata + 2, unit_size); // 拷贝对应的SPS、PPS数据
	    extradata += unit_size + 2;   // 注意多加2,前面没有跳过长度信息
	
	pps:    // 获取完成SPS后，会开始从这里更新PPS的信息到上面的unit_nb中
	    if (!unit_nb && !sps_done++) {    // 当SPS获取完成以后，unit_nb=0;!sps_done=1;  注意，sps_done++,导致不为0,获取一次PPS之后，后面就不会在进入这里
	            unit_nb = *extradata++;     // 当读取了所有SPS数据以后，再读取一个字节，用来表示PPS的个数，然后再循环去获取PPS的数据
	            if (unit_nb) {    //PPS存在
				    pps_offset = total_size;// 表示前面的SPS已经获取完成，后面偏移写入PPS数据即可    
				    pps_seen = 1;           // 表示获取了PPS数据
			}
		}
	}
	
	if (out) // 开始进行数据0填充
	    memset(out + total_size, 0, padding);
	if (!sps_seen)   // 没有获取到SPS数据
	    av_log(NULL, AV_LOG_WARNING, "Warning: SPS NALU missing or invalid. The resulting stream may not play.\n");
	
	if (!pps_seen)   // 没有获取到PPS数据
	    av_log(NULL, AV_LOG_WARNING, "Warning: PPS NALU missing or invalid. The resulting stream may not play.\n");
	// 将数据赋值给AVPacket中返回
	    out_extradata->data = out;
	out_extradata->size = total_size;
	
	return length_size; // 返回前缀长度
}

// 负责将H264格式的本地mp4文件从AVCC格式转为实时流AnnexB格式
int h264_mp4toannexb(AVFormatContext* fmt_ctx, AVPacket* in, FILE* dst_fd)
{
	AVPacket* out = NULL;   // 设置输入包信息
	AVPacket spspps_pkt;    // 用来存放SPS、PPS信息，对于AnnexB，我们需要在所有I帧前面加上SPS、PPS数据
							   
	int len;                // 保存fwrite返回写入的数据长度
	uint8_t unit_type;      // 存放NALU的header，长度为8bits
							   
	uint8_t nal_size_len;   // AVCC格式数据采用NALU长度（固定字节，一般为4字节，取决与extradata中的NALULengthSizeMinusOne字段）分隔NALU
	int32_t nal_size;      // 由nal_size_len可以知道，保存NALU长度一般可以取（1、2、4字节），我们这里取4字节，兼容所有

	uint32_t cumul_size = 0;// 存放当前包中已经处理多少字节数据，当==buf_size表示都处理完了，退出循环
	uint32_t buf_size;      // 存放in中数据data的大小
	const uint8_t* buf;     // 采访in中数据的起始地址（注意：使用uint_8，按单字节增长）
	const uint8_t* buf_end; // 采访in中数据的结束地址

	int ret = 0, i;          // 存放返回值,以及循环变量i

	buf = in->data;         // 指向AVPacket数据data开头
	buf_end = in->data + in->size;  // 指向AVPacket数据data的末尾
	buf_size = in->size;    // 记录AVPacket数据data的大小

	// 我们是将AVCC格式数据转为AnnexB格式，所以首先去读取SPS、PPS数据，因为AVCC格式数据保存在extradata中。
	// 而且AVCC格式用于存储，比如MP4,并非实时流，所以SPS、PPS不会在中间被修改，所以我们获取一次即可！！！！
	nal_size_len = h264_extradata_to_annexb(fmt_ctx->streams[in->stream_index]->codec->extradata,
											fmt_ctx->streams[in->stream_index]->codec->extradata_size,
											&spspps_pkt, AV_INPUT_BUFFER_PADDING_SIZE);  //获取SPS、PPS数据，并且返回前缀值
	if (nal_size_len < 0)
		return -1;

	out = av_packet_alloc();    // 为out AVPacket分配数据空间

	do {
		ret = AVERROR(EINVAL);  // 初始一个返回错误码，无效参数值（不用管）
		if (buf + nal_size_len > buf_end)
			goto fail;          // 我们输入数据是AVCC格式，该格式数据前4字节用于存放NALU长度信息，如果连这个4字节都不存在，则返回错误
		// 先假设nal_size_len=4
		for (nal_size = 0, i = 0; i < nal_size_len; i++) {  // 开始获取NALU长度信息
			// 注意：视频数据存放时，是大端格式，我们要**读取**长度信息，需要进行相应处理(如果只是单纯写入，就不需要处理，但是我们需要去读取长度）！！！
			nal_size = (nal_size << 8) | buf[i];
		}

		// buf指针后移，指向NALU数据的header部分
		buf += nal_size_len;    // 跳过NALU长度部分数据，进入NALU主要数据区域
		unit_type = (*buf) & 0x1f;  // header长度为1字节，前3bits影响不大，我们获取后面5bits，去获取NALU类型下信息

		if (nal_size > buf_end - buf || nal_size < 0)   // 检查长度，是否有效
			goto fail;

		// 开始判断NALU单元的类型，是否为关键帧，如果是关键帧，我们需要在其前面加入SPS、PPS信息
		if (unit_type == 5) {
			FILE* sp = fopen("spspps.h264", "ab");

			len = fwrite(spspps_pkt.data, 1, spspps_pkt.size, sp);

			fflush(sp);
			fclose(sp);
			// 先写入start code和SPS和PPS数据，都被保存在前面spspps_pkt的data中,我们转放入out中
			if ((ret = alloc_and_copy(out, spspps_pkt.data, spspps_pkt.size, buf, nal_size)) < 0)    // 这里是将前面的startcode+SPS+PPS+NALU数据传入函数中，使得函数在NALU 前面加入startcode;这里的startcode不是4字节，而是3字节
				goto fail;
		}
		else {  // 对于非关键帧，不需要SPS、PPS数据
			if ((ret = alloc_and_copy(out, NULL, 0, buf, nal_size)) < 0)    //这里是将前面的NALU数据传入函数中，使得函数在NALU 前面加入startcode
				goto fail;
		}

		// 将上面的数据，无论是关键帧、非关键帧 都组织好，输出到目标文件中去
		len = fwrite(out->data, 1, out->size, dst_fd);
		if (len != out->size) {
			av_log(NULL, AV_LOG_DEBUG, "Warning, length of writed data isn`t equal pkt.size(%d,%d)\n", len, out->size);

			fflush(dst_fd);
			// 开始判断下一个nalu
			buf += nal_size;
			cumul_size += nal_size + nal_size_len; // 算上前缀长度才能对应
		}
	} while (cumul_size < buf_size); // 循环继续条件

fail:   // 进行统一错误处理
	av_packet_free(&out);
	return ret;
}

int TransMP4ToH264(const char* src, const char* dst) {
	int err_code;       // 获取返回值
	char errors[1024]; // 获取ffmpeg返回根据错误码返回的错误信息
	
	av_log_set_level(AV_LOG_INFO);  // 设置日志级别
	
	if (!src || !dst) {
	    av_log(NULL, AV_LOG_ERROR, "the file path of src or dst can`t be empty!!\n");
	    return -1;
	}
	
	av_register_all();  // 初始化libavformat并注册所有muxer、demuxer和协议
	
	// 打开输入多媒体文件，获取上格式下文
	AVFormatContext * fmt_ctx = NULL;
	err_code = avformat_open_input(&fmt_ctx, src, NULL, NULL);
	if (err_code < 0) {
	    av_strerror(err_code, errors, 1024);
	    av_log(NULL, AV_LOG_ERROR, "open media %s file failure : %d,(%s)!!!\n", src, err_code, errors);
	    return -1;
	}
	
	// 获取找到其中最佳的一路视频流
	int video_idx;
	// 媒体文件句柄 / 流类型 / 请求的流编号(-1则自动去找) / 相关流索引号（比如音频对应的视频流索引号），不指定则-1
	// 如果非空，则返回所选流的解码器（指针获取） / flag当前未定义
	video_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	if (video_idx < 0) {
		av_log(NULL, AV_LOG_DEBUG, "Can`t find %s stream in input file (%s)!!!\n",
		av_get_media_type_string(AVMEDIA_TYPE_VIDEO), src);  // 去获取AVMEDIA_TYPE_VIDEO对应的string
		avformat_close_input(&fmt_ctx); // 释放前面的空间
		return -1;
	}
	
	// 输出我们获取的流的元信息
	err_code = avformat_find_stream_info(fmt_ctx, 0); // 获取流详细信息,0表示没有额外参数
	if (err_code < 0) {
	    av_log(NULL, AV_LOG_WARNING, "Can`t get detail stream information!\n");
	}
	// 打印我们要获取的流的元数据
	av_dump_format(fmt_ctx, video_idx, src, 0); //// video_idx是流的索引值,，0表示输入/输出流，由于是输入文件，所以为0
	
	// 打开目标文件
	FILE * dst_fd = fopen(dst, "wb");
	if (!dst_fd) {
	    av_log(NULL, AV_LOG_ERROR, "Can`t open destination file(%s)\n", dst);
	    avformat_close_input(&fmt_ctx); // 释放前面的空间
	    return -1;
	}
	
	// 开始从流中读取数据包
	// 初始化包结构
	AVPacket pkt;
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;
	
	while (av_read_frame(fmt_ctx, &pkt) >= 0) {  // 循环获取下一个包
	    if (pkt.stream_index == video_idx) {  // 是我们想要的数据包
	        h264_mp4toannexb(fmt_ctx, &pkt, dst_fd);         // 开始进行包写入，先将AVCC格式数据转为AnnexB格式，然后写入目标文件中去
		}
		// 对每一个获取的包进行减引用
		av_packet_unref(&pkt);
	}
	
	// 开始进行空间释放
	avformat_close_input(&fmt_ctx);
	if (dst_fd) {
	    fclose(dst_fd);
	
	}
	
	return 0;
}

void log_packet(const AVFormatContext* fmt_ctx, const AVPacket* pkt, const char* tag)
{
    AVRational* time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
        tag,
        av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
        av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
        av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
        pkt->stream_index);
}

int TransMP4toFLV(const char* src, const char* dst) {
    AVOutputFormat* ofmt = NULL;    //输出格式
    AVFormatContext* ifmt_ctx = NULL, * ofmt_ctx = NULL;    //输入、输出上下文

    AVPacket pkt;    //数据包
    const char* in_filename, * out_filename;

    int ret, i;
    int stream_idx = 0;
    int* stream_mapping = NULL;        //数组:用来存放各个流通道的新索引值（对于不要的流，设置-1,对于需要的流从0开始递增
    int stream_mapping_size = 0;    //输入文件中流的总数量

    av_log_set_level(AV_LOG_INFO);
    av_register_all();

    //打开输入多媒体文件，获取上下文格式
    ret = avformat_open_input(&ifmt_ctx, src, NULL, NULL);//第三个参数强制指定AVFormatContext中AVInputFormat，一般设置为NULL，自动检测。第四个为附加选项
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can`t open input file %s \n", in_filename);
        goto fail;
    }

    //检索输入文件的流信息
    ret = avformat_find_stream_info(ifmt_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Fail to retrieve input stream information!\n");
        goto fail;
    }

    //打印关于输入或输出格式的详细信息，例如持续时间，比特率，流，容器，程序，元数据，边数据，编解码器和时基。
    av_dump_format(ifmt_ctx, 0, src, 0);    //第一个0表示流的索引值，第二个0表示是输入文件

    //为输出上下文环境分配空间
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, dst);    //第二个参数：指定AVFormatContext中的AVOutputFormat，用于确定输出格式。如果指定为NULL，可以设定后两个参数（format_name或者filename）由FFmpeg猜测输出格式。第三个参数为文件格式比如.flv,也可以通过第四个参数获取
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Can`t create output context!\n");
        ret = AVERROR_UNKNOWN;
        goto fail;
    }

    //记录输入文件的stream通道数目
    stream_mapping_size = ifmt_ctx->nb_streams;
    //为数组分配空间，sizeof(*stream_mapping)是分配了一个int空间，为stream_mapping分配了stream_mapping_size个int空间
    stream_mapping = (int*)av_mallocz_array(stream_mapping_size, sizeof(*stream_mapping));
    if (!stream_mapping) {
        ret = AVERROR(ENOMEM);    //内存不足
        goto fail;
    }

    //输出文件格式
    ofmt = ofmt_ctx->oformat;
    //遍历输入文件中的每一路流，对于每一路流都要创建一个新的流进行输出
    for (i = 0; i < stream_mapping_size; i++) {
        AVStream* out_stream = NULL;    //输出流
        AVStream* in_stream = ifmt_ctx->streams[i];    //输入流获取
        AVCodecParameters* in_codecpar = in_stream->codecpar;    //获取输入流的编解码参数

        //只保留音频、视频、字母流;对于其他流丢弃（实际上是设置对应的数组值为-1）
        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }
        //对于输出流的index重新编号，从0开始,写入stream_mapping数组对应空间中去
        stream_mapping[i] = stream_idx++;

        //重点：为输出格式上下文，创建一个对应的输出流
        out_stream = avformat_new_stream(ofmt_ctx, NULL);    //第二个参数为对应的视频所需要的编码方式，为NULL则自动推导
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate output stream\n");
            ret = AVERROR_UNKNOWN;
            goto fail;
        }

        //直接将输入流的编解码参数拷贝到输出流中
        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters\n");
            goto fail;
        }

        //详见：https://juejin.cn/post/6854573210579501070
        //avformat_write_header写入封装容器的头信息时，会检查codec_tag：
        //若AVStream->codecpar->codec_tag有值，则会校验AVStream->codecpar->codec_tag是否在封装格式（比如MAP4）支持的codec_tag列表中，若不在，就会打印错误信息；
        //若AVStream->codecpar->codec_tag为0，则会根据AVCodecID从封装格式的codec_tag列表中，找一个匹配的codec_tag。
        out_stream->codecpar->codec_tag = 0;
    }

    //打印要输出多媒体文件的详细信息
    av_dump_format(ofmt_ctx, 0, dst, 1);    //1表示输出文件

    if (!(ofmt->flags & AVFMT_NOFILE)) {    //查看文件格式状态，如果文件不存在（未打开），则开启文件
        ret = avio_open(&ofmt_ctx->pb, dst, AVIO_FLAG_WRITE);    //打开文件，可写
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Can`t open output file %s!\n", out_filename);
            goto fail;
        }
    }

    //开始写入新的多媒体文件头部
    ret = avformat_write_header(ofmt_ctx, NULL);    //NULL为附加选项
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can`t write format header into file: %s\n", out_filename);
        goto fail;
    }

    //循环写入多媒体数据
    while (1) {
        AVStream* in_stream, * out_stream;    //获取输入输出流
        //循环读取每一帧
        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0) {    //读取完成，退出喜欢
            break;
        }
        //获取输入流在stream_mapping中的数组值，看是否保留
        in_stream = ifmt_ctx->streams[pkt.stream_index];    //先获取所属的流的信息
        if (pkt.stream_index >= stream_mapping_size || stream_mapping[pkt.stream_index] < 0) {    //判断是否是我们想要的音频、视频、字幕流，不是的话就跳过
            av_packet_unref(&pkt);
            continue;
        }
        //需要对流进行重新编号（因为原来输入流部分被跳过），输出流编号应该从0开始递增;索引就是我们上面保存的数组值
        pkt.stream_index = stream_mapping[pkt.stream_index];    //按照输出流的编号对pakcet进行重新编号
        //根据上面的索引，获取ofmt_cxt输出格式上下文对应的输出流，进行处理
        out_stream = ofmt_ctx->streams[pkt.stream_index];

        //开始对pakcet进行时间基的转换，因为音视频的采用率不同，所以不进行转换，会导致时间不同步。最终使得音频对应音频刻度，视频对应视频刻度
        //PTS(Presentation Time Stamp, 显示时间戳)，是渲染用的时间戳，播放器会根据这个时间戳进行渲染播放
        //DTS(Decoding Time Stamp, 解码时间戳)，解码时间戳，在视频packet进行解码成frame的时候会使用到
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;

        log_packet(ofmt_ctx, &pkt, "out");

        //将处理好的packet写入输出文件中
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error muxing packet\n");
            break;
        }
        av_packet_unref(&pkt);
    }

    av_write_trailer(ofmt_ctx);    //写入文件尾部
fail:
    //关闭输入文件格式上下文
    avformat_close_input(&ifmt_ctx);
    //关闭输出文件
    if (ofmt_ctx && !(ofmt_ctx->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);

    avformat_free_context(ofmt_ctx);    //关闭输出格式上下文
    av_freep(&stream_mapping);    //释放数组空间
    if (ret < 0 && ret != AVERROR_EOF) {    //异常退出
        av_log(NULL, AV_LOG_ERROR, "Error occurred: %s\n", av_err2str(ret));
        return 1;
    }
    return 0;
}

int cut_video(char* in_filename, char* out_filename, int starttime, int endtime) {
    AVOutputFormat* ofmt = NULL;    //输出格式
    AVFormatContext* ifmt_ctx = NULL, * ofmt_ctx = NULL;    //输入、输出上下文

    AVPacket pkt;    //数据包

    int ret, i;
    int stream_idx = 0;
    int* stream_mapping = NULL;        //数组:用来存放各个流通道的新索引值（对于不要的流，设置-1,对于需要的流从0开始递增
    int stream_mapping_size = 0;    //输入文件中流的总数量

     //打开输入多媒体文件，获取上下文格式
    ret = avformat_open_input(&ifmt_ctx, in_filename, NULL, NULL);//第三个参数强制指定AVFormatContext中AVInputFormat，一般设置为NULL，自动检测。第四个为附加选项
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can`t open input file %s \n", in_filename);
        goto fail;
    }

    //检索输入文件的流信息
    ret = avformat_find_stream_info(ifmt_ctx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Fail to retrieve input stream information!\n");
        goto fail;
    }

    //打印关于输入或输出格式的详细信息，例如持续时间，比特率，流，容器，程序，元数据，边数据，编解码器和时基。
    av_dump_format(ifmt_ctx, 0, in_filename, 0);    //第一个0表示流的索引值，第二个0表示是输入文件

    //为输出上下文环境分配空间
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);    //第二个参数：指定AVFormatContext中的AVOutputFormat，用于确定输出格式。如果指定为NULL，可以设定后两个参数（format_name或者filename）由FFmpeg猜测输出格式。第三个参数为文件格式比如.flv,也可以通过第四个参数获取
    if (!ofmt_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Can`t create output context!\n");
        ret = AVERROR_UNKNOWN;
        goto fail;
    }

    //记录输入文件的stream通道数目
    stream_mapping_size = ifmt_ctx->nb_streams;
    //为数组分配空间，sizeof(*stream_mapping)是分配了一个int空间，为stream_mapping分配了stream_mapping_size个int空间
    stream_mapping = (int*)av_mallocz_array(stream_mapping_size, sizeof(*stream_mapping));
    if (!stream_mapping) {
        ret = AVERROR(ENOMEM);    //内存不足
        goto fail;
    }

    //输出文件格式
    ofmt = ofmt_ctx->oformat;
    //遍历输入文件中的每一路流，对于每一路流都要创建一个新的流进行输出
    for (i = 0; i < stream_mapping_size; i++) {
        AVStream* out_stream = NULL;    //输出流
        AVStream* in_stream = ifmt_ctx->streams[i];    //输入流获取
        AVCodecParameters* in_codecpar = in_stream->codecpar;    //获取输入流的编解码参数

        //只保留音频、视频、字母流;对于其他流丢弃（实际上是设置对应的数组值为-1）
        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }
        //对于输出流的index重新编号，从0开始,写入stream_mapping数组对应空间中去
        stream_mapping[i] = stream_idx++;

        //重点：为输出格式上下文，创建一个对应的输出流
        out_stream = avformat_new_stream(ofmt_ctx, NULL);    //第二个参数为对应的视频所需要的编码方式，为NULL则自动推导
        if (!out_stream) {
            av_log(NULL, AV_LOG_ERROR, "Failed to allocate output stream\n");
            ret = AVERROR_UNKNOWN;
            goto fail;
        }

        //直接将输入流的编解码参数拷贝到输出流中
        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Failed to copy codec parameters\n");
            goto fail;
        }

        //详见：https://juejin.cn/post/6854573210579501070
        //avformat_write_header写入封装容器的头信息时，会检查codec_tag：若AVStream->codecpar->codec_tag有值，则会校验AVStream->codecpar->codec_tag是否在封装格式（比如MAP4）支持的codec_tag列表中，若不在，就会打印错误信息；
        //若AVStream->codecpar->codec_tag为0，则会根据AVCodecID从封装格式的codec_tag列表中，找一个匹配的codec_tag。
        out_stream->codecpar->codec_tag = 0;
    }

    //打印要输出多媒体文件的详细信息
    av_dump_format(ofmt_ctx, 0, out_filename, 1);    //1表示输出文件

    if (!(ofmt->flags & AVFMT_NOFILE)) {    //查看文件格式状态，如果文件不存在（未打开），则开启文件
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);    //打开文件，可写
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Can`t open output file %s!\n", out_filename);
            goto fail;
        }
    }

    //开始写入新的多媒体文件头部
    ret = avformat_write_header(ofmt_ctx, NULL);    //NULL为附加选项
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can`t write format header into file: %s\n", out_filename);
        goto fail;
    }

    //--------------seek定位---------
    ret = av_seek_frame(ifmt_ctx, -1, starttime * AV_TIME_BASE, AVSEEK_FLAG_ANY);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can`t seek input file: %s\n", in_filename);
        goto fail;
    }

    //循环写入多媒体数据
    while (1) {
        AVStream* in_stream, * out_stream;    //获取输入输出流
        //循环读取每一帧
        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0) {    //读取完成，退出喜欢
            break;
        }
        //获取输入流在stream_mapping中的数组值，看是否保留
        in_stream = ifmt_ctx->streams[pkt.stream_index];    //先获取所属的流的信息
        if (pkt.stream_index >= stream_mapping_size || stream_mapping[pkt.stream_index] < 0) {    //判断是否是我们想要的音频、视频、字幕流，不是的话就跳过
            av_packet_unref(&pkt);
            continue;
        }
        //---------判断是否到结束时间----------
        if (av_q2d(in_stream->time_base) * pkt.pts > endtime) {    //av_q2d获取该流的时间基
            av_free_packet(&pkt);
            break;
        }

        //需要对流进行重新编号（因为原来输入流部分被跳过），输出流编号应该从0开始递增;索引就是我们上面保存的数组值
        pkt.stream_index = stream_mapping[pkt.stream_index];    //按照输出流的编号对pakcet进行重新编号
        //根据上面的索引，获取ofmt_cxt输出格式上下文对应的输出流，进行处理
        out_stream = ofmt_ctx->streams[pkt.stream_index];

        //开始对pakcet进行时间基的转换，因为音视频的采用率不同，所以不进行转换，会导致时间不同步。最终使得音频对应音频刻度，视频对应视频刻度
        //PTS(Presentation Time Stamp, 显示时间戳)，是渲染用的时间戳，播放器会根据这个时间戳进行渲染播放
        //DTS(Decoding Time Stamp, 解码时间戳)，解码时间戳，在视频packet进行解码成frame的时候会使用到
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;

        //将处理好的packet写入输出文件中
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error muxing packet\n");
            break;
        }
        av_packet_unref(&pkt);
    }

    av_write_trailer(ofmt_ctx);    //写入文件尾部
fail:
    //关闭输入文件格式上下文
    avformat_close_input(&ifmt_ctx);
    //关闭输出文件
    if (ofmt_ctx && !(ofmt_ctx->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);

    avformat_free_context(ofmt_ctx);    //关闭输出格式上下文
    av_freep(&stream_mapping);    //释放数组空间
    if (ret < 0 && ret != AVERROR_EOF) {    //异常退出
        av_log(NULL, AV_LOG_ERROR, "Error occurred: %s\n", av_err2str(ret));
        return 1;
    }
    return 0;
}

int PushStream(const char* src, const char* dst) {
	AVOutputFormat* ofmt = nullptr;
	AVFormatContext* ifmt_ctx = nullptr;
	AVFormatContext* ofmt_ctx = nullptr;
	AVPacket pkt;
	int video_index = 0;
    int frame_index = 0;
	int ret = 0;

	av_register_all();
	avformat_network_init();

	// Input
	ret = avformat_open_input(&ifmt_ctx, src, nullptr, nullptr);
	if (ret < 0) {
		printf("Failed to open input file!\n");
		av_log(nullptr, AV_LOG_ERROR, "Failed to open input file!\n");
		if (ifmt_ctx) {
			avformat_close_input(&ifmt_ctx);
			return -1;
		}
	}

	ret = avformat_find_stream_info(ifmt_ctx, nullptr);
	if (ret < 0) {
		printf("Failed to find stream info!\n");
		av_log(nullptr, AV_LOG_ERROR, "Failed to find stream info!\n");
		if (ifmt_ctx) {
			avformat_close_input(&ifmt_ctx);
			return -1;
		}
	}

	for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
		if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			video_index = i;
			break;
		}
	}

	av_dump_format(ifmt_ctx, 0, src, 0);

	// Output
	avformat_alloc_output_context2(&ofmt_ctx, ofmt, "flv", dst); // RTMP
	if (!ofmt_ctx) {
		printf("Failed to create output format context!\n");
		av_log(nullptr, AV_LOG_ERROR, "Failed to create output format context!\n");
		if (ifmt_ctx) {
			avformat_close_input(&ifmt_ctx);
		}
		if (ofmt_ctx) {
			avformat_free_context(ofmt_ctx);
		}
		return -1;
	}

	ofmt = ofmt_ctx->oformat;
	for (int i = 0; i < ofmt_ctx->nb_streams; i++) {
		AVStream* in_stream = ofmt_ctx->streams[i];
		AVStream* out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
		if (!out_stream) {
			printf("Failed to alloc output stream!\n");
			av_log(nullptr, AV_LOG_ERROR, "Failed to alloc output stream!\n");
			return -1;
		}

		ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
		if (ret < 0) {
			printf("Failed to copy context from input stream to output stream!\n");
			av_log(nullptr, AV_LOG_ERROR, "Failed to copy context from input stream to output stream!\n");
			return -1;
		}

		out_stream->codec->codec_tag = 0;
		if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
			out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		}
	}
	
	av_dump_format(ofmt_ctx, 0, dst, 1);
	if (!(ofmt->flags & AVFMT_NOFILE)) {
		ret = avio_open(&ofmt_ctx->pb, dst, AVIO_FLAG_WRITE);
        if (ret < 0) {
            printf("Failed to open output file!\n");
            av_log(nullptr, AV_LOG_ERROR, "Failed to open output file!\n");
            return -1;
        }
	}

    ret = avformat_write_header(ofmt_ctx, nullptr);
    if (ret < 0) {
        printf("Failed to write header to file!\n");
        av_log(nullptr, AV_LOG_ERROR, "Failed to write header to file!\n");
        return -1;
    }

    int start_time = av_gettime();
    while (1) {
        AVStream* in_stream = nullptr;
        AVStream* out_stream = nullptr;

        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0) {
            printf("Failed to read frame!\n");
            av_log(nullptr, AV_LOG_ERROR, "Failed to read frame!\n");
            break;
        }

        if (pkt.pts == AV_NOPTS_VALUE) {
            AVRational time_base1 = ifmt_ctx->streams[video_index]->time_base;
            int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(ifmt_ctx->streams[video_index]->r_frame_rate);
            //Parameters
            pkt.pts = (double)(frame_index * calc_duration) / (double)(av_q2d(time_base1) * AV_TIME_BASE);
            pkt.dts = pkt.pts;
            pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1) * AV_TIME_BASE);
        }
        //Important:Delay
        if (pkt.stream_index == video_index) {
            AVRational time_base = ifmt_ctx->streams[video_index]->time_base;
            AVRational time_base_q = { 1,AV_TIME_BASE };
            int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
            int64_t now_time = av_gettime() - start_time;
            if (pts_time > now_time)
                av_usleep(pts_time - now_time);

        }

        in_stream = ifmt_ctx->streams[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];
        /* copy packet */
        //转换PTS/DTS（Convert PTS/DTS）
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        //Print to Screen
        if (pkt.stream_index == video_index) {
            printf("Send %8d video frames to output URL\n", frame_index);
            frame_index++;
        }
        //ret = av_write_frame(ofmt_ctx, &pkt);
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);

        if (ret < 0) {
            printf("Error muxing packet\n");
            break;
        }

        av_free_packet(&pkt);
    }
    av_write_trailer(ofmt_ctx);

    avformat_close_input(&ifmt_ctx);
    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);
    if (ret < 0 && ret != AVERROR_EOF) {
        printf("Error occurred.\n");
        return -1;
    }
    return 0;
}

int flush_encoder(AVFormatContext* fmt_ctx, unsigned int stream_index) {
    int ret;
    int got_frame;
    AVPacket enc_pkt;
    if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
        AV_CODEC_CAP_DELAY))
        return 0;
    while (1) {
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_video2(fmt_ctx->streams[stream_index]->codec, &enc_pkt,
            NULL, &got_frame);
        av_frame_free(NULL);
        if (ret < 0)
            break;
        if (!got_frame) {
            ret = 0;
            break;
        }
        printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", enc_pkt.size);
        /* mux encoded frame */
        ret = av_write_frame(fmt_ctx, &enc_pkt);
        if (ret < 0)
            break;
    }
    return ret;
}

int VideoEncoder(const char* src, const char* dst) {
    AVCodec* pCodec = NULL;
    AVCodecContext* pCodecCtx = NULL;
    int i, ret, got_output;
    AVFrame* pFrame = NULL;
    AVPacket pkt;
    int y_size = 0;
    int framecnt = 0;

    char filename_in[] = "cam_yuv420p.yuv";

#if TEST_HEVC
    AVCodecID codec_id = AV_CODEC_ID_HEVC;
    char filename_out[] = "ds.hevc";
#else
    AVCodecID codec_id = AV_CODEC_ID_H264;
    char filename_out[] = "cam.h264";
#endif

    int in_w = 640, in_h = 480;
    int framenum = 100;

    avcodec_register_all();

    //Input raw data
    FILE* fp_in = fopen(filename_in, "rb");
    if (!fp_in) {
        printf("Could not open %s\n", filename_in);
        return -1;
    }
    //Output bitstream
    FILE* fp_out = fopen(filename_out, "wb");
    if (!fp_out) {
        printf("Could not open %s\n", filename_out);
        return -1;
    }

    pCodec = avcodec_find_encoder(codec_id);
    if (!pCodec) {
        printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        printf("Could not allocate video codec context\n");
        return -1;
    }
    pCodecCtx->bit_rate = 400000;
    pCodecCtx->width = in_w;
    pCodecCtx->height = in_h;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->gop_size = 10;
    pCodecCtx->max_b_frames = 1;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec_id == AV_CODEC_ID_H264)
        av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    if (!pFrame) {
        printf("Could not allocate video frame\n");
        return -1;
    }
    pFrame->format = pCodecCtx->pix_fmt;
    pFrame->width = pCodecCtx->width;
    pFrame->height = pCodecCtx->height;

    ret = av_image_alloc(pFrame->data, pFrame->linesize, pCodecCtx->width, pCodecCtx->height,
        pCodecCtx->pix_fmt, 16);
    if (ret < 0) {
        printf("Could not allocate raw picture buffer\n");
        return -1;
    }

    y_size = pCodecCtx->width * pCodecCtx->height;
    //Encode
    for (i = 0; i < framenum; i++) {
        av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;
        //Read raw YUV data
        if (fread(pFrame->data[0], 1, y_size, fp_in) <= 0 ||		// Y
            fread(pFrame->data[1], 1, y_size / 4, fp_in) <= 0 ||	// U
            fread(pFrame->data[2], 1, y_size / 4, fp_in) <= 0) {	// V
            return -1;
        }
        else if (feof(fp_in)) {
            break;
        }

        pFrame->pts = i;
        /* encode the image */
        //ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_output);
        ret = avcodec_send_frame(pCodecCtx, pFrame);
        if (ret < 0) {
            printf("Error encoding frame\n");
            return -1;
        }

        printf("Succeed to encode frame: %5d\tsize:%5d\n", framecnt, pkt.size);
        framecnt++;
        ret = avcodec_receive_packet(pCodecCtx, &pkt);
        fwrite(pkt.data, 1, pkt.size, fp_out);
        av_free_packet(&pkt);
    }
    //Flush Encoder
    for (got_output = 1; got_output; i++) {
        //ret = avcodec_encode_video2(pCodecCtx, &pkt, NULL, &got_output);
        ret = avcodec_send_frame(pCodecCtx, pFrame);
        if (ret < 0) {
            printf("Error encoding frame\n");
            return -1;
        }
        printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", pkt.size);
        fwrite(pkt.data, 1, pkt.size, fp_out);
        av_free_packet(&pkt);
    }

    fclose(fp_out);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);
    av_freep(&pFrame->data[0]);
    av_frame_free(&pFrame);

    return 0;
}

int VideoDecoder(const char* src, const char* dst) {
    AVFormatContext* pFormatCtx;
    int				i, videoindex;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;
    AVFrame* pFrame, * pFrameYUV;
    unsigned char* out_buffer;
    AVPacket* packet;
    int y_size;
    int ret, got_picture;
    struct SwsContext* img_convert_ctx;

    char filepath[] = "qwe.mp4";

    FILE* fp_yuv = fopen("qwe_output.yuv", "wb+");

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0) {
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Couldn't find stream information.\n");
        return -1;
    }

    videoindex = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }
    
    if (videoindex == -1) {
        printf("Didn't find a video stream.\n");
        return -1;
    }

    pCodecCtx = pFormatCtx->streams[videoindex]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        printf("Codec not found.\n");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec.\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
        AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

    packet = (AVPacket*)av_malloc(sizeof(AVPacket));
    //Output Info-----------------------------
    printf("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx, 0, filepath, 0);
    printf("-------------------------------------------------\n");
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    while (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == videoindex) {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0) {
                printf("Decode Error.\n");
                return -1;
            }
            if (got_picture) {
                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameYUV->data, pFrameYUV->linesize);

                y_size = pCodecCtx->width * pCodecCtx->height;
                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);    //Y 
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V
                printf("Succeed to decode 1 frame!\n");

            }
        }
        av_free_packet(packet);
    }
    //flush decoder
    //FIX: Flush Frames remained in Codec
    while (1) {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
        if (ret < 0)
            break;
        if (!got_picture)
            break;
        sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
            pFrameYUV->data, pFrameYUV->linesize);

        int y_size = pCodecCtx->width * pCodecCtx->height;
        fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);    //Y 
        fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
        fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V

        printf("Flush Decoder: Succeed to decode 1 frame!\n");
    }

    sws_freeContext(img_convert_ctx);

    fclose(fp_yuv);

    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}

//test different codec
#define TEST_H264  1
#define TEST_HEVC  0
int Decoder_H264ToYUV() {
    AVCodec* pCodec;
    AVCodecContext* pCodecCtx = NULL;
    AVCodecParserContext* pCodecParserCtx = NULL;

    FILE* fp_in;
    FILE* fp_out;
    AVFrame* pFrame;

    const int in_buffer_size = 4096;
    uint8_t in_buffer[in_buffer_size + AV_INPUT_BUFFER_PADDING_SIZE] = { 0 };
    uint8_t* cur_ptr;
    int cur_size;
    AVPacket packet;
    int ret, got_picture;
    int y_size;


#if TEST_HEVC
    enum AVCodecID codec_id = AV_CODEC_ID_HEVC;
    char filepath_in[] = "bigbuckbunny_480x272.hevc";
#elif TEST_H264
    AVCodecID codec_id = AV_CODEC_ID_H264;
    char filepath_in[] = "qwe_PA.h264";
#else
    AVCodecID codec_id = AV_CODEC_ID_MPEG2VIDEO;
    char filepath_in[] = "bigbuckbunny_480x272.m2v";
#endif

    char filepath_out[] = "qwe_PA_123.yuv";
    int first_time = 1;


    //av_log_set_level(AV_LOG_DEBUG);

    avcodec_register_all();

    pCodec = avcodec_find_decoder(codec_id);
    if (!pCodec) {
        printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        printf("Could not allocate video codec context\n");
        return -1;
    }

    pCodecParserCtx = av_parser_init(codec_id);
    if (!pCodecParserCtx) {
        printf("Could not allocate video parser context\n");
        return -1;
    }

    //if(pCodec->capabilities&CODEC_CAP_TRUNCATED)
    //    pCodecCtx->flags|= CODEC_FLAG_TRUNCATED; 

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }
    //Input File
    fp_in = fopen(filepath_in, "rb");
    if (!fp_in) {
        printf("Could not open input stream\n");
        return -1;
    }
    //Output File
    fp_out = fopen(filepath_out, "wb");
    if (!fp_out) {
        printf("Could not open output YUV file\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    av_init_packet(&packet);

    while (1) {

        cur_size = fread(in_buffer, 1, in_buffer_size, fp_in);
        if (cur_size == 0)
            break;
        cur_ptr = in_buffer;

        while (cur_size > 0) {

            int len = av_parser_parse2(
                pCodecParserCtx, pCodecCtx,
                &packet.data, &packet.size,
                cur_ptr, cur_size,
                AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

            cur_ptr += len;
            cur_size -= len;

            if (packet.size == 0)
                continue;

            //Some Info from AVCodecParserContext
            printf("[Packet]Size:%6d\t", packet.size);
            switch (pCodecParserCtx->pict_type) {
            case AV_PICTURE_TYPE_I: printf("Type:I\t"); break;
            case AV_PICTURE_TYPE_P: printf("Type:P\t"); break;
            case AV_PICTURE_TYPE_B: printf("Type:B\t"); break;
            default: printf("Type:Other\t"); break;
            }
            printf("Number:%4d\n", pCodecParserCtx->output_picture_number);

            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
            if (ret < 0) {
                printf("Decode Error.\n");
                return ret;
            }
            if (got_picture) {
                if (first_time) {
                    printf("\nCodec Full Name:%s\n", pCodecCtx->codec->long_name);
                    printf("width:%d\nheight:%d\n\n", pCodecCtx->width, pCodecCtx->height);
                    first_time = 0;
                }
                //Y, U, V
                for (int i = 0; i < pFrame->height; i++) {
                    fwrite(pFrame->data[0] + pFrame->linesize[0] * i, 1, pFrame->width, fp_out);
                }
                for (int i = 0; i < pFrame->height / 2; i++) {
                    fwrite(pFrame->data[1] + pFrame->linesize[1] * i, 1, pFrame->width / 2, fp_out);
                }
                for (int i = 0; i < pFrame->height / 2; i++) {
                    fwrite(pFrame->data[2] + pFrame->linesize[2] * i, 1, pFrame->width / 2, fp_out);
                }

                printf("Succeed to decode 1 frame!\n");
            }
        }

    }

    //Flush Decoder
    packet.data = NULL;
    packet.size = 0;
    while (1) {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, &packet);
        if (ret < 0) {
            printf("Decode Error.\n");
            return ret;
        }
        if (!got_picture) {
            break;
        }
        else {
            //Y, U, V
            for (int i = 0; i < pFrame->height; i++) {
                fwrite(pFrame->data[0] + pFrame->linesize[0] * i, 1, pFrame->width, fp_out);
            }
            for (int i = 0; i < pFrame->height / 2; i++) {
                fwrite(pFrame->data[1] + pFrame->linesize[1] * i, 1, pFrame->width / 2, fp_out);
            }
            for (int i = 0; i < pFrame->height / 2; i++) {
                fwrite(pFrame->data[2] + pFrame->linesize[2] * i, 1, pFrame->width / 2, fp_out);
            }

            printf("Flush Decoder: Succeed to decode 1 frame!\n");
        }
    }

    fclose(fp_in);
    fclose(fp_out);


    av_parser_close(pCodecParserCtx);

    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);

    return 0;
}

int AudioEncoder(const char* src, const char* dst) {
    AVFormatContext* pFormatCtx;
    AVOutputFormat* fmt;
    AVStream* audio_st;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;

    uint8_t* frame_buf;
    AVFrame* pFrame;
    AVPacket pkt;

    int got_frame = 0;
    int ret = 0;
    int size = 0;

    FILE* in_file = NULL;	                        //Raw PCM data
    int framenum = 1000;                          //Audio frame number
    const char* out_file = "liming.aac";          //Output URL
    FILE* pOutput = fopen(out_file, "wb+");
    int i;

    in_file = fopen("03情归于尽_output.pcm", "rb");

    av_register_all();

    //Method 1.
    pFormatCtx = avformat_alloc_context();
    fmt = av_guess_format(NULL, out_file, NULL);
    pFormatCtx->oformat = fmt;

    //Method 2.
    //avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, out_file);
    //fmt = pFormatCtx->oformat;

    //Open output URL
    if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0) {
        printf("Failed to open output file!\n");
        return -1;
    }

    audio_st = avformat_new_stream(pFormatCtx, 0);
    if (audio_st == NULL) {
        return -1;
    }

    pCodecCtx = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(pCodecCtx, audio_st->codecpar);
    pCodecCtx->codec_id = fmt->audio_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    pCodecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    pCodecCtx->sample_rate = 44100;
    pCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
    pCodecCtx->channels = av_get_channel_layout_nb_channels(pCodecCtx->channel_layout);
    pCodecCtx->bit_rate = 64000;
    av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);

    //Show some information
    av_dump_format(pFormatCtx, 0, out_file, 1);

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        printf("Can not find encoder!\n");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Failed to open encoder!\n");
        return -1;
    }
    pFrame = av_frame_alloc();
    pFrame->nb_samples = pCodecCtx->frame_size;
    pFrame->format = pCodecCtx->sample_fmt;

    size = av_samples_get_buffer_size(NULL, pCodecCtx->channels, pCodecCtx->frame_size, pCodecCtx->sample_fmt, 1);
    frame_buf = (uint8_t*)av_malloc(size);
    avcodec_fill_audio_frame(pFrame, pCodecCtx->channels, pCodecCtx->sample_fmt, (const uint8_t*)frame_buf, size, 1);

    //Write Header
    avformat_write_header(pFormatCtx, NULL);

    av_new_packet(&pkt, size);

    for (i = 0; i < framenum; i++) {
        //Read PCM
        if (fread(frame_buf, 1, size, in_file) <= 0) {
            printf("Failed to read raw data! \n");
            return -1;
        }
        else if (feof(in_file)) {
            break;
        }
        pFrame->data[0] = frame_buf;  //PCM Data
        
        pFrame->pts = i * 100;
        got_frame = 0;
        //Encode
        //ret = avcodec_encode_audio2(pCodecCtx, &pkt, pFrame, &got_frame);
        ret = avcodec_send_frame(pCodecCtx, pFrame);
        if (ret < 0) {
            printf("Failed to encode!\n");
            return -1;
        }

        printf("Succeed to encode 1 frame! \tsize:%5d\n", pkt.size);
        pkt.stream_index = audio_st->index;
        ret = avcodec_receive_packet(pCodecCtx, &pkt);
        if (ret < 0) {
            printf("Failed to encode!\n");
            return -1;
        }
        fwrite(pkt.data, 1, pkt.size, pOutput);
        av_packet_unref(&pkt);
        //ret = av_write_frame(pFormatCtx, &pkt);
        //av_free_packet(&pkt);
    }

    //Flush Encoder
    ret = flush_encoder(pFormatCtx, 0);
    if (ret < 0) {
        printf("Flushing encoder failed\n");
        return -1;
    }

    //Write Trailer
    av_write_trailer(pFormatCtx);

    //Clean
    if (audio_st) {
        avcodec_close(audio_st->codec);
        av_free(pFrame);
        av_free(frame_buf);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

    fclose(in_file);

    return 0;
}

int AudioDecoder(const char* src, const char* dst) {
    AVFormatContext* pFormatCtx = NULL;
    AVCodecContext* pCodecCtx = NULL;
    AVCodec* pCodec = NULL;
    AVPacket* packet = NULL;
    AVFrame* pFrame = NULL;

    uint8_t* out_buf = NULL;
    int got_pic = 0;
    int index = 0;
    int64_t in_channel_layout = 0;
    struct SwrContext* au_convert_ctx = NULL;

    FILE* pFile = fopen("03情归于尽_output.pcm", "wb+");
    char url[] = "03情归于尽.mp3";
    int ret = 0;

    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) < 0)
    {
        printf("Could not open input stream!\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        printf("Could not find stream information!\n");
        return -1;
    }

    av_dump_format(pFormatCtx, 0, url, 0);

    int audioStream = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codec->codec_type)
        {
            audioStream = i;
            break;
        }
    }

    if (-1 == audioStream)
    {
        printf("Didn't find a audio stream!\n");
        return -1;
    }

    pCodecCtx = pFormatCtx->streams[audioStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (NULL == pCodec)
    {
        printf("Codec not found!\n");
        return -1;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("Could not open codec!\n");
        return -1;
    }

    packet = (AVPacket*)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);

    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
    int out_nb_samples = pCodecCtx->frame_size;
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    int out_sample_rate = 44100;
    //获取声道数
    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
    //获取音频文件需要的缓存大小
    int out_buf_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
    out_buf = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    pFrame = av_frame_alloc();
    //为给定的声道数获取默认声道布局
    in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);

    au_convert_ctx = swr_alloc();
    au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate, in_channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0, NULL);
    swr_init(au_convert_ctx);

    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        if (packet->stream_index == audioStream)
        {
            ret = avcodec_decode_audio4(pCodecCtx, pFrame, &got_pic, packet);
            if (ret < 0)
            {
                printf("Error in decoding audio frame!\n");
                return -1;
            }

            if (got_pic > 0)
            {
                swr_convert(au_convert_ctx, &out_buf, MAX_AUDIO_FRAME_SIZE, (const uint8_t**)pFrame->data, pFrame->nb_samples);
                printf("index : %5d\t pts : %lld\t packet size : %d\n", index, packet->pts, packet->size);
                fwrite(out_buf, 1, out_buf_size, pFile);
                index++;
            }
        }
        av_free_packet(packet);
    }

    swr_free(&au_convert_ctx);
    fclose(pFile);

    av_free(out_buf);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}

/*
int CaptureDesktop() {
    AVFormatContext* pFormatCtx;
    AVStream* videoStream;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;
    AVFrame* pFrame, * pFrameYUV;
    AVPacket* pPacket;
    SwsContext* pImgConvertCtx;

    int videoIndex = -1;
    unsigned int i = 0;

    SDL_Window* screen;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Rect sdlRect;

    int screen_w = 0;
    int screen_h = 0;

    printf("Starting...\n");

    //register device
    avdevice_register_all();

    pFormatCtx = avformat_alloc_context();

    //use gdigrab
    AVInputFormat* ifmt = av_find_input_format("gdigrab");
    if (!ifmt)
    {
        printf("can't find input device.\n");
        return -1;
    }

    AVDictionary* options = NULL;
    if (avformat_open_input(&pFormatCtx, "desktop", ifmt, &options) != 0)
    {
        printf("can't open input stream.\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        printf("can't find stream information.\n");
        return -1;
    }

    for (i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoIndex = i;
            break;
        }
    }

    if (videoIndex == -1)
    {
        printf("can't find a video stream.\n");
        return -1;
    }

    videoStream = pFormatCtx->streams[videoIndex];
    pCodec = avcodec_find_decoder(videoStream->codecpar->codec_id);
    if (pCodec == NULL)
    {
        printf("codec not found.\n");
        return -1;
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx)
    {
        printf("can't alloc codec context.\n");
        return -1;
    }

    avcodec_parameters_to_context(pCodecCtx, videoStream->codecpar);

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("can't open codec.\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));

    unsigned char* outBuffer = (unsigned char*)av_malloc(
        av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width,
            pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, outBuffer,
        AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

    pImgConvertCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
        pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
        AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

#if OUTPUT_YUV420P
    FILE* fpYUV = fopen("output.yuv", "wb+");
#endif

#if OUTPUT_H264
    AVCodecContext* pH264CodecCtx;
    AVCodec* pH264Codec;

    FILE* fpH264 = fopen("output.h264", "wb+");

    //查找H264编码器
    pH264Codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!pH264Codec)
    {
        printf("can't find h264 codec.\n");
        return -1;
    }

    pH264CodecCtx = avcodec_alloc_context3(pH264Codec);
    pH264CodecCtx->codec_id = AV_CODEC_ID_H264;
    pH264CodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pH264CodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pH264CodecCtx->width = pCodecCtx->width;
    pH264CodecCtx->height = pCodecCtx->height;
    pH264CodecCtx->time_base.num = 1;
    pH264CodecCtx->time_base.den = 15;	//帧率（即一秒钟多少张图片）
    pH264CodecCtx->bit_rate = 800000;	//比特率（调节这个大小可以改变编码后视频的质量）
    pH264CodecCtx->gop_size = 12;
    pH264CodecCtx->qmin = 10;
    pH264CodecCtx->qmax = 51;
    //some formats want stream headers to be separate
    if (pH264CodecCtx->flags & AVFMT_GLOBALHEADER)
    {
        pH264CodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    // set option
    AVDictionary* params = NULL;
    //H.264
    av_dict_set(&params, "preset", "superfast", 0);
    av_dict_set(&params, "tune", "zerolatency", 0);	//实现实时编码
    if (avcodec_open2(pH264CodecCtx, pH264Codec, &params) < 0)
    {
        printf("can't open video encoder.\n");
        return -1;
    }

#endif

    //SDL handle
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
    {
        printf("can't initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    screen_w = pCodecCtx->width;
    screen_h = pCodecCtx->height;

    screen = SDL_CreateWindow("Simplest ffmpeg device(screen capture)",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h,
        SDL_WINDOW_OPENGL);
    if (!screen)
    {
        printf("SDL: can't create window - exiting: %s\n", SDL_GetError());
        return -1;
    }

    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);

    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = screen_w;
    sdlRect.h = screen_h;

    while (av_read_frame(pFormatCtx, pPacket) >= 0)
    {
        if (pPacket->stream_index == videoIndex)
        {
            int ret = avcodec_send_packet(pCodecCtx, pPacket);
            if (ret < 0)
            {
                printf("Decode error.\n");
                return -1;
            }

            if (avcodec_receive_frame(pCodecCtx, pFrame) >= 0)
            {
                sws_scale(pImgConvertCtx,
                    (const unsigned char* const*)pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data,
                    pFrameYUV->linesize);

#if OUTPUT_YUV420P
                int y_size = pCodecCtx->width * pCodecCtx->height;
                fwrite(pFrameYUV->data[0], 1, y_size, fpYUV);		//Y
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fpYUV);	//U
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fpYUV);	//V
#endif

#if OUTPUT_H264
                pFrameYUV->format = pCodecCtx->pix_fmt;
                pFrameYUV->width = pCodecCtx->width;
                pFrameYUV->height = pCodecCtx->height;
                int ret = avcodec_send_frame(pH264CodecCtx, pFrameYUV);
                if (ret < 0)
                {
                    printf("failed to encode.\n");
                    return -1;
                }

                if (avcodec_receive_packet(pH264CodecCtx, pPacket) >= 0)
                {
                    ret = fwrite(pPacket->data, 1, pPacket->size, fpH264);
                    if (ret < 0)
                    {
                        printf("write into output.h264 failed.\n");
                    }
                }
#endif
                SDL_UpdateTexture(sdlTexture, &sdlRect, pFrameYUV->data[0],
                    pFrameYUV->linesize[0]);
                SDL_RenderClear(sdlRenderer);
                SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);
                SDL_RenderPresent(sdlRenderer);
                SDL_Delay(40);
            }
        }

        av_packet_unref(pPacket);
    }

    sws_freeContext(pImgConvertCtx);

#if OUTPUT_YUV420P
    fclose(fpYUV);
#endif

#if OUTPUT_H264
    fclose(fpH264);
#endif

    SDL_Quit();

    av_free(outBuffer);
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avcodec_close(pH264CodecCtx);
    avformat_close_input(&pFormatCtx);


    return 0;
}
*/

// YUV to H264
int VideoEncoder_New() {
    AVCodecContext* pCodecCtx = NULL;
    AVCodec* pCodec = NULL;

    FILE* pInput = fopen("cam_yuv420p.yuv", "rb+");
    if (!pInput) {
        printf("Failed to open input file!\n");
        return -1;
    }
    FILE* pOutput = fopen("cam.h264", "wb+");
    if (!pOutput) {
        printf("Failed to open output file!\n");
        return -1;
    }

    av_register_all();

    pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!pCodec) {
        printf("Failed to find encoder!\n");
        return -1;
    }

    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        printf("Failed to allocate AVCodecContext!\n");
        return -1;
    }
    pCodecCtx->bit_rate = 400000;
    pCodecCtx->width = 640;
    pCodecCtx->height = 480;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->gop_size = 10;
    pCodecCtx->max_b_frames = 1;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    int ret = avcodec_open2(pCodecCtx, pCodec, NULL);
    if (ret < 0) {
        printf("Failed to open encoder!\n");
        return -1;
    }

    AVFrame* pFrame = av_frame_alloc();
    if (!pFrame) {
        printf("Failed to allocate AVFrame!\n");
        return -1;
    }
    pFrame->format = pCodecCtx->pix_fmt;
    pFrame->width = pCodecCtx->width;
    pFrame->height = pCodecCtx->height;

    ret = av_image_alloc(pFrame->data, pFrame->linesize, pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 16);
    if (ret < 0) {
        printf("Failed to allocate image!\n");
        return -1;
    }

    int y_size = pCodecCtx->width * pCodecCtx->height;
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    // 不是从流或者媒体文件中读取 AVPacket，故不作刷新编码器处理
    for (int i = 0; i < 100; i++) {
        
        if (fread(pFrame->data[0], 1, y_size, pInput) <= 0 ||
            fread(pFrame->data[1], 1, y_size / 4, pInput) <= 0 ||
            fread(pFrame->data[2], 1, y_size / 4, pInput) <= 0) {
            return -1;
        }
        else if (feof(pInput)) {
            printf("End of File!\n");
            break;
        }

        pFrame->pts = i;
        ret = avcodec_send_frame(pCodecCtx, pFrame);
        if (ret < 0) {
            printf("Failed to send frame to encoder!\n");
            return -1;
        }

        printf("Succeed to decode %d frame!\n", i + 1);
        ret = avcodec_receive_packet(pCodecCtx, &pkt);
        fwrite(pkt.data, 1, pkt.size, pOutput);
        av_packet_unref(&pkt);
    }

    while (1) {
        printf("Flush Encoder: Succeed to encode 1 frame!\n");
        ret = avcodec_receive_packet(pCodecCtx, &pkt);
        if (ret < 0) {
            printf("Error, Failed to encode! \n");
            break;
        }
        fwrite(pkt.data, 1, pkt.size, pOutput);
        av_packet_unref(&pkt);
    }

    fclose(pOutput);
    fclose(pInput);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);
    av_freep(&pFrame->data[0]);
    av_frame_free(&pFrame);

    return 0;
}

// MP4 to YUV --- 解封装 ==》 解码
int VideoDecoder_New() {
    AVFormatContext* pFormatCtx = NULL;

    av_register_all();

    int ret = avformat_open_input(&pFormatCtx, "qwe.mp4", NULL, NULL);
    if (ret < 0) {
        printf("Failed to open input file!\n");
        return -1;
    }

    ret = avformat_find_stream_info(pFormatCtx, NULL);
    if (ret < 0) {
        printf("Failed to find stream info!\n");
        return -1;
    }

    int video_index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_index < 0) {
        printf("Failed to find correct stream!\n");
        return -1;
    }

    av_dump_format(pFormatCtx, video_index, "qwe.mp4", 0);

    AVCodecContext* pCodecCtx = pFormatCtx->streams[video_index]->codec;
    av_opt_set(pCodecCtx->priv_data, "tune", "zerolatency", 0);
    AVCodec* pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (!pCodec) {
        printf("Failed to find decoder!\n");
        return -1;
    }

    ret = avcodec_open2(pCodecCtx, pCodec, NULL);
    if (ret < 0) {
        printf("Failed to open decoder!\n");
        return -1;
    }

    AVFrame* pFrame = av_frame_alloc();
    if (!pFrame) {
        printf("Failed to allocate frame!\n");
        return -1;
    }
    AVFrame* pFrameYUV = av_frame_alloc();
    if (!pFrameYUV) {
        printf("Failed to allocate frame!\n");
        return -1;
    }
    unsigned char* out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer,
        AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);

    SwsContext* img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    AVPacket pkt;
    int y_size = pCodecCtx->width * pCodecCtx->height;
    FILE* pOutput = fopen("test.yuv", "wb+");
    if (!pOutput) {
        printf("Failed to open output file!\n");
        return -1;
    }

    int i = 0;
    while (av_read_frame(pFormatCtx, &pkt) >= 0) {
        if (video_index == pkt.stream_index) {
            ret = avcodec_send_packet(pCodecCtx, &pkt);
            if (ret < 0) {
                printf("Failed to send packet to decoder!\n");
                return -1;
            }

            while (1) {
                ret = avcodec_receive_frame(pCodecCtx, pFrame);
                if (ret < 0) {
                    printf("Error, Failed to encode! \n");
                    break;
                }
                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameYUV->data, pFrameYUV->linesize);
                fwrite(pFrameYUV->data[0], 1, y_size, pOutput);
                fwrite(pFrameYUV->data[1], 1, y_size / 4, pOutput);
                fwrite(pFrameYUV->data[2], 1, y_size / 4, pOutput);
                printf("Succeed to decode %d frame!\n", i + 1);
                i++;
            }
        }
        av_packet_unref(&pkt);
    }

    while (1) {
        if (video_index == pkt.stream_index) {
            ret = avcodec_send_packet(pCodecCtx, &pkt);
            if (ret < 0) {
                printf("Failed to send packet to decoder!\n");
                return -1;
            }

            while (1) {
                ret = avcodec_receive_frame(pCodecCtx, pFrame);
                if (ret < 0) {
                    printf("Error, Failed to encode! \n");
                    break;
                }
                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameYUV->data, pFrameYUV->linesize);
                fwrite(pFrameYUV->data[0], 1, y_size, pOutput);
                fwrite(pFrameYUV->data[1], 1, y_size / 4, pOutput);
                fwrite(pFrameYUV->data[2], 1, y_size / 4, pOutput);
                printf("Succeed to decode %d frame!\n", i + 1);
                i++;
            }
        }
        av_packet_unref(&pkt);
    }

    sws_freeContext(img_convert_ctx);

    fclose(pOutput);

    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}

// 音视频分离器
int Demuxer() {
    AVOutputFormat* ofmt_a = NULL, * ofmt_v = NULL;
    //（Input AVFormatContext and Output AVFormatContext）
    AVFormatContext* ifmt_ctx = NULL, * ofmt_ctx_a = NULL, * ofmt_ctx_v = NULL;
    AVPacket pkt;
    int ret, i;
    int videoindex = -1, audioindex = -1;
    int frame_index = 0;

    const char* in_filename = "qwe.mp4";//Input file URL
    //char *in_filename  = "cuc_ieschool.mkv";
    const char* out_filename_v = "qwe_demuxer.h264";//Output file URL
    //char *out_filename_a = "cuc_ieschool.mp3";
    const char* out_filename_a = "qwe_demuxer.aac";

    av_register_all();
    //Input
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        printf("Could not open input file.");
        goto end;
    }
    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        printf("Failed to retrieve input stream information");
        goto end;
    }

    //Output
    avformat_alloc_output_context2(&ofmt_ctx_v, NULL, NULL, out_filename_v);
    if (!ofmt_ctx_v) {
        printf("Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }
    ofmt_v = ofmt_ctx_v->oformat;

    avformat_alloc_output_context2(&ofmt_ctx_a, NULL, NULL, out_filename_a);
    if (!ofmt_ctx_a) {
        printf("Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        goto end;
    }
    ofmt_a = ofmt_ctx_a->oformat;

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        //Create output AVStream according to input AVStream
        AVFormatContext* ofmt_ctx;
        AVStream* in_stream = ifmt_ctx->streams[i];
        AVStream* out_stream = NULL;

        if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            out_stream = avformat_new_stream(ofmt_ctx_v, in_stream->codec->codec);
            ofmt_ctx = ofmt_ctx_v;
        }
        else if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioindex = i;
            out_stream = avformat_new_stream(ofmt_ctx_a, in_stream->codec->codec);
            ofmt_ctx = ofmt_ctx_a;
        }
        else {
            break;
        }

        if (!out_stream) {
            printf("Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }
        //Copy the settings of AVCodecContext
        if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) {
            printf("Failed to copy context from input to output stream codec context\n");
            goto end;
        }
        out_stream->codec->codec_tag = 0;

        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    //Dump Format------------------
    printf("\n==============Input Video=============\n");
    av_dump_format(ifmt_ctx, 0, in_filename, 0);
    printf("\n==============Output Video============\n");
    av_dump_format(ofmt_ctx_v, 0, out_filename_v, 1);
    printf("\n==============Output Audio============\n");
    av_dump_format(ofmt_ctx_a, 0, out_filename_a, 1);
    printf("\n======================================\n");
    //Open output file
    if (!(ofmt_v->flags & AVFMT_NOFILE)) {
        if (avio_open(&ofmt_ctx_v->pb, out_filename_v, AVIO_FLAG_WRITE) < 0) {
            printf("Could not open output file '%s'", out_filename_v);
            goto end;
        }
    }

    if (!(ofmt_a->flags & AVFMT_NOFILE)) {
        if (avio_open(&ofmt_ctx_a->pb, out_filename_a, AVIO_FLAG_WRITE) < 0) {
            printf("Could not open output file '%s'", out_filename_a);
            goto end;
        }
    }

    //Write file header
    if (avformat_write_header(ofmt_ctx_v, NULL) < 0) {
        printf("Error occurred when opening video output file\n");
        goto end;
    }
    if (avformat_write_header(ofmt_ctx_a, NULL) < 0) {
        printf("Error occurred when opening audio output file\n");
        goto end;
    }

#if USE_H264BSF
    AVBitStreamFilterContext* h264bsfc = av_bitstream_filter_init("h264_mp4toannexb");
#endif

    while (1) {
        AVFormatContext* ofmt_ctx;
        AVStream* in_stream, * out_stream;
        //Get an AVPacket
        if (av_read_frame(ifmt_ctx, &pkt) < 0)
            break;
        in_stream = ifmt_ctx->streams[pkt.stream_index];


        if (pkt.stream_index == videoindex) {
            out_stream = ofmt_ctx_v->streams[0];
            ofmt_ctx = ofmt_ctx_v;
            printf("Write Video Packet. size:%d\tpts:%lld\n", pkt.size, pkt.pts);
#if USE_H264BSF
            av_bitstream_filter_filter(h264bsfc, in_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif
        }
        else if (pkt.stream_index == audioindex) {
            out_stream = ofmt_ctx_a->streams[0];
            ofmt_ctx = ofmt_ctx_a;
            printf("Write Audio Packet. size:%d\tpts:%lld\n", pkt.size, pkt.pts);
        }
        else {
            continue;
        }


        //Convert PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        pkt.stream_index = 0;
        //Write
        if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) {
            printf("Error muxing packet\n");
            break;
        }
        //printf("Write %8d frames to output file\n",frame_index);
        av_free_packet(&pkt);
        frame_index++;
    }

#if USE_H264BSF
    av_bitstream_filter_close(h264bsfc);
#endif

    //Write file trailer
    av_write_trailer(ofmt_ctx_a);
    av_write_trailer(ofmt_ctx_v);
end:
    avformat_close_input(&ifmt_ctx);
    /* close output */
    if (ofmt_ctx_a && !(ofmt_a->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx_a->pb);

    if (ofmt_ctx_v && !(ofmt_v->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx_v->pb);

    avformat_free_context(ofmt_ctx_a);
    avformat_free_context(ofmt_ctx_v);


    if (ret < 0 && ret != AVERROR_EOF) {
        printf("Error occurred.\n");
        return -1;
    }
    return 0;
}

int Test() {
    

    return 0;
}

int main()
{
    Demuxer();
	return 0;
}
