#include "Voyage/audio.hpp"
#include "openal/al.h"
#include "openal/alext.h"
#include "sndfile.h"
#include "minimp3/minimp3.h"
#include "minimp3/minimp3_ex.h"
#include <cstdio>
#include <string_view>

namespace Voyage {

	ALCdevice* AudioMaster::device;
	ALCcontext* AudioMaster::context;
	std::vector<uint32_t> AudioMaster::buffers;
	glm::vec3 AudioMaster::listenerPosition, AudioMaster::listenerVelocity;

	void AudioMaster::init() {
		device = alcOpenDevice(NULL);
		if(!device) {
			std::cout << "OpenAL device could not be determined and opened." << std::endl;
			return;
		}
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
		bool g_bEAX = alIsExtensionPresent("EAX2.0");

		uint32_t err = alGetError();
		if(err != AL_NO_ERROR) fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
	}

	void AudioMaster::setListenerData(const float& x, const float& y, const float& z, const float& vx, const float& vy, const float& vz) { alListener3f(AL_POSITION, x, y, z); alListener3f(AL_VELOCITY, vx, vy, vz); }

	void AudioMaster::setDistanceModel(int& model) { alDistanceModel(model); }

	uint32_t AudioMaster::loadSound(const char* const filename) {
		std::string_view file_ext(filename + strlen(filename) - 3, 2);
		uint32_t buffer;
		if(file_ext == "mp") buffer = loadSoundBufferMP3(filename);
		else buffer = loadSoundBuffer(filename);
		uint32_t source;
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, (uint32_t)buffer);
		assert(alGetError() == AL_NO_ERROR && "Failed to setup sound source");
		return buffer;
	}

	void AudioMaster::dispose() {
		alDeleteBuffers(buffers.size(), buffers.data());
		buffers.clear();
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	uint32_t AudioMaster::loadSoundBuffer(const char* filename) {
		int err, format;
		uint32_t buffer = 0;
		SNDFILE *sndfile;
		SF_INFO sfinfo;
		short *membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		sndfile = sf_open(filename, SFM_READ, &sfinfo);
		if(!sndfile) {
			fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
			return 0;
		}
		if(sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX/sizeof(short))/sfinfo.channels) {
			fprintf(stderr, "Bad sample count in %s\n", filename);
			sf_close(sndfile);
			return 0;
		}

		// Get the sound format
		format = AL_NONE;
		if(sfinfo.channels == 1) format = AL_FORMAT_MONO16;
		else if(sfinfo.channels == 2) format = AL_FORMAT_STEREO16;
		else if(sfinfo.channels == 3) {
			if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) format = AL_FORMAT_BFORMAT2D_16;
		}
		else if(sfinfo.channels == 4) {
			if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) format = AL_FORMAT_BFORMAT3D_16;
		}

		if(!format) {
			fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
			sf_close(sndfile);
			return 0;
		}

		// Decode the whole audio file to a buffer
		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if(num_frames < 1) {
			free(membuf);
			sf_close(sndfile);
			fprintf(stderr, "Failed to read samples in %s", filename);
			return 0;
		}
		num_bytes = (ALsizei) (num_frames * sfinfo.channels) * (ALsizei) sizeof(short);
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);
		free(membuf);
		sf_close(sndfile);

		err = alGetError();
		if(err != AL_NO_ERROR) {
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if(buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
			return 0;
		}
		buffers.push_back(buffer);

		return buffer;
	}

	uint32_t AudioMaster::loadSoundBufferMP3(const char* filename) {
		uint32_t buffer;
		mp3dec_t mp3d;
		mp3dec_init(&mp3d);
		mp3dec_file_info_t info;

		if(mp3dec_load(&mp3d, filename, &info, NULL, NULL)) fprintf(stderr, "Error while loading %s MP3 file\n", filename);

		alGenBuffers(1, &buffer);
		alBufferData(buffer, info.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, info.buffer, info.samples, info.hz);

		int err = alGetError();
		if(err != AL_NO_ERROR) {
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if(buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
			delete info.buffer;
			alDeleteBuffers(1, &buffer);
			return 0;
		}
		buffers.push_back(buffer);
		delete info.buffer;

		return buffer;
	}

}
