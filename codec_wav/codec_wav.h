/*
zhaozch3
2019.10.18

1.�򵥵�wav&pcm����뷽����
2.��֧��16λ��˫�������ݣ�
3.��֧�ֺ��и������ݵ�wav�ļ���
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

	bool decoder(const char* wav_file);
	bool encoder(const char* wav_file);

	bool decoder_pcm(const char* pcm_file, int16_t channel);
	bool encoder_pcm(const char* pcm_file);
};

#endif
