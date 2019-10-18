/*
zhaozch3
2019.10.18

1.简单的wav&pcm编解码方法；
2.仅支持16位单双声道数据；
3.不支持含有附加数据的wav文件；
*/

#ifndef CODEC_WAV
#define CODEC_WAV

#include<cstdint>

struct header {
	char ckid[5], fcc[5], fmt[5], ckid2[5];
	int32_t cksize, cksize_in, SamplesPerSec, BytesPerSec, cksize2;
	int16_t FormatTag, Channels, BlockAlign, BitsPerSample;
};

class codec_wav {
public:
	header _header;

	int32_t _sample_rate;
	int16_t _channel;
	int32_t _data_size;

	int16_t* _pcm_data_L;
	int16_t* _pcm_data_R;

	codec_wav();
	~codec_wav();
	
	//wav文件 -> pcm流
	bool decoder(const char* wav_file);
	//pcm流 -> wav文件
	bool encoder(const char* wav_file);
};

//pcm文件 -> pcm流
bool decoder_pcm(const char* pcm_file, int16_t* _pcm_data_L, int16_t* _pcm_data_R, int32_t& _data_size, const int16_t _channel);
//pcm流 -> pcm文件
bool encoder_pcm(const char* pcm_file, int16_t* _pcm_data_L, int16_t* _pcm_data_R, const int32_t _data_size, const int16_t _channel);

#endif
