#pragma once
#include <voyage.hpp>
#include "openal/al.h"
#include "openal/alc.h"

namespace Voyage {
	class AudioMaster {
		public:
			static const int DISTANCE_ATTENUATION_MODEL_EXPONENT 		 = AL_EXPONENT_DISTANCE,
						 	 DISTANCE_ATTENUATION_MODEL_EXPONENT_CLAMPED = AL_EXPONENT_DISTANCE_CLAMPED,
						 	 DISTANCE_ATTENUATION_MODEL_LINEAR 			 = AL_LINEAR_DISTANCE,
						 	 DISTANCE_ATTENUATION_MODEL_LINEAR_CLAMPED 	 = AL_LINEAR_DISTANCE_CLAMPED,
						 	 DISTANCE_ATTENUATION_MODEL_INVERSE 		 = AL_INVERSE_DISTANCE,
						 	 DISTANCE_ATTENUATION_MODEL_INVERSE_CLAMPED  = AL_INVERSE_DISTANCE_CLAMPED;

			AudioMaster() = delete;

			~AudioMaster() = delete;

			static void init();

			static void setListenerData(const float& x, const float& y, const float& z, const float& vx, const float& vy, const float& vz);

			static void setDistanceModel(int& model);

			static unsigned int loadSound(const char* filename);

			static void dispose();
		private:
			static ALCdevice* device;
			static ALCcontext* context;
			static std::vector<unsigned int> buffers;
			static glm::vec3 listenerPosition, listenerVelocity;

			static unsigned int loadSoundBuffer(const char* filename);

	};
}
