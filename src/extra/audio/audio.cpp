#include "audio.hpp"
#include "openal/alext.h"
#include "sndfile.h"
#include "minimp3.h"

namespace Voyage {

	ALCdevice* AudioMaster::device;
	ALCcontext* AudioMaster::context;
	std::vector<unsigned int> AudioMaster::buffers;
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

		unsigned int err = alGetError();
		if(err != AL_NO_ERROR) fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
	}

	void AudioMaster::setListenerData(const float& x, const float& y, const float& z, const float& vx, const float& vy, const float& vz) { alListener3f(AL_POSITION, x, y, z); alListener3f(AL_VELOCITY, vx, vy, vz); }

	void AudioMaster::setDistanceModel(int& model) { alDistanceModel(model); }

	unsigned int AudioMaster::loadSound(const char* filename) {
		unsigned int source = 0;
		alGenSources(1, &source);
		unsigned int buffer = loadSoundBuffer(filename);
		alSourcei(source, AL_BUFFER, (unsigned int)buffer);
		assert(alGetError() == AL_NO_ERROR && "Failed to setup sound source");
		return buffer;
	}

	void AudioMaster::dispose() {
		alDeleteBuffers(buffers.size(), buffers.data());
		buffers.clear();
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	unsigned int AudioMaster::loadSoundBuffer(const char* filename) {
		int err, format;
		unsigned int buffer = 0;
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

}
