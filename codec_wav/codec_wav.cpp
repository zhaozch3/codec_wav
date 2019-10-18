#include "codec_wav.h"
#include <iostream>

codec_wav::codec_wav() {
	_data_size = 0;
	_pcm_data_L = NULL;
	_pcm_data_R = NULL;
}

codec_wav::~codec_wav() {
	if (_pcm_data_L) {
		free(_pcm_data_L);
	}
	if (_pcm_data_R) {
		free(_pcm_data_R);
	}
}

bool codec_wav::decoder(const char* wav_file) {
	FILE* fp = fopen(wav_file, "rb");
	if (fp) {
		fread(_header.ckid, sizeof(char), 4, fp); _header.ckid[4] = '\0';
		if (!strcmp(_header.ckid, "RIFF")) {
			fread(&_header.cksize, sizeof(int32_t), 1, fp);
			fread(_header.fcc, sizeof(char), 4, fp); _header.fcc[4] = '\0';
			if (!strcmp(_header.fcc, "WAVE")) {
				fread(_header.fmt, sizeof(char), 4, fp); _header.fmt[4] = '\0';
				fread(&_header.cksize_in, sizeof(int32_t), 1, fp);
				fread(&_header.FormatTag, sizeof(int16_t), 1, fp);
				fread(&_header.Channels, sizeof(int16_t), 1, fp);
				_channel = _header.Channels;
				fread(&_header.SamplesPerSec, sizeof(int32_t), 1, fp);
				_sample_rate = _header.SamplesPerSec;
				fread(&_header.BytesPerSec, sizeof(int32_t), 1, fp);
				fread(&_header.BlockAlign, sizeof(int16_t), 1, fp);
				fread(&_header.BitsPerSample, sizeof(int16_t), 1, fp);
				//数据处理
				fread(_header.ckid2, sizeof(char), 4, fp); _header.ckid2[4] = '\0';
				fread(&_header.cksize2, sizeof(int32_t), 1, fp);
				if (_channel == 1) {
					_data_size = _header.cksize2 / sizeof(int16_t);
				}
				else if (_channel == 2) {
					_data_size = _header.cksize2 / sizeof(int16_t) / 2;
				}
				//声道处理
				int16_t* buffer = (int16_t*)malloc(_header.cksize2);
				fread(buffer, sizeof(int16_t), _header.cksize2 / sizeof(int16_t), fp);
				if (_channel == 1) {
					_pcm_data_L = (int16_t*)malloc(_header.cksize2);
					memcpy(_pcm_data_L, buffer, _header.cksize2);
				}
				else if (_channel == 2) {
					_pcm_data_L = (int16_t*)malloc(_header.cksize2 / 2);
					_pcm_data_R = (int16_t*)malloc(_header.cksize2 / 2);
					for (int i = 0; i < _data_size; i++) {
						_pcm_data_L[i] = buffer[2 * i];
						_pcm_data_R[i] = buffer[2 * i + 1];
					}
				}
				free(buffer);
				fclose(fp);
				return true;
			}
		}
	}
	return false;
}

bool codec_wav::encoder(const char* wav_file) {
	if (_pcm_data_L == NULL) {
		return false;
	}
	FILE* fp = fopen(wav_file, "wb");
	if (fp) {
		fwrite(_header.ckid, sizeof(char), 4, fp);
		fwrite(&_header.cksize, sizeof(int32_t), 1, fp);
		fwrite(_header.fcc, sizeof(char), 4, fp);
		fwrite(_header.fmt, sizeof(char), 4, fp);
		fwrite(&_header.cksize_in, sizeof(int32_t), 1, fp);
		fwrite(&_header.FormatTag, sizeof(int16_t), 1, fp);
		fwrite(&_header.Channels, sizeof(int16_t), 1, fp);
		fwrite(&_header.SamplesPerSec, sizeof(int32_t), 1, fp);
		fwrite(&_header.BytesPerSec, sizeof(int32_t), 1, fp);
		fwrite(&_header.BlockAlign, sizeof(int16_t), 1, fp);
		fwrite(&_header.BitsPerSample, sizeof(int16_t), 1, fp);
		fwrite(_header.ckid2, sizeof(char), 4, fp);
		fwrite(&_header.cksize2, sizeof(int32_t), 1, fp);
		if (_header.Channels == 1) {
			fwrite(_pcm_data_L, sizeof(int16_t), _data_size, fp);
		}
		else if (_header.Channels == 2) {
			for (int i = 0; i < _data_size; i++) {
				fwrite(_pcm_data_L + i, sizeof(int16_t), 1, fp);
				fwrite(_pcm_data_R + i, sizeof(int16_t), 1, fp);
			}
		}
		fclose(fp);
		return true;
	}
	return false;
}

bool decoder_pcm(const char* pcm_file, int16_t* _pcm_data_L, int16_t* _pcm_data_R, int32_t& _data_size, const int16_t _channel) {
	FILE* fp = fopen(pcm_file, "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		_data_size = ftell(fp) / 2 / _channel;
		fseek(fp, 0, SEEK_SET);
		if (_channel == 1) {
			_pcm_data_L = (int16_t*)malloc(_data_size * sizeof(int16_t));
			fread(_pcm_data_L, sizeof(int16_t), _data_size, fp);
		}
		else if (_channel == 2) {
			_pcm_data_L = (int16_t*)malloc(_data_size * sizeof(int16_t));
			_pcm_data_R = (int16_t*)malloc(_data_size * sizeof(int16_t));
			for (int i = 0; i < _data_size; i++) {
				fread(_pcm_data_L + i, sizeof(int16_t), 1, fp);
				fread(_pcm_data_R + i, sizeof(int16_t), 1, fp);
			}
		}
		fclose(fp);
		return true;
	}
	return false;
}

bool encoder_pcm(const char* pcm_file, int16_t* _pcm_data_L, int16_t* _pcm_data_R, const int32_t _data_size, const int16_t _channel) {
	if (_pcm_data_L == NULL) {
		return false;
	}
	FILE* fp = fopen(pcm_file, "wb");
	if (fp) {
		if (_channel == 1) {
			fwrite(_pcm_data_L, sizeof(int16_t), _data_size, fp);
		}
		else if (_channel == 2) {
			for (int i = 0; i < _data_size; i++) {
				fwrite(_pcm_data_L + i, sizeof(int16_t), 1, fp);
				fwrite(_pcm_data_R + i, sizeof(int16_t), 1, fp);
			}
		}
		fclose(fp);
		return true;
	}
	return false;
}
