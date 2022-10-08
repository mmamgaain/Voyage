#pragma once
#include "voyage.hpp"

namespace Voyage {
	class Source {
		public:
			Source(const int& buffer, const glm::vec3& position = {0.0, 0.0, 0.0}, const glm::vec3& velocity = {0.0, 0.0, 0.0}, const bool& startPlaying = false);

			~Source();

			const glm::vec3& getPosition();

			const glm::vec3& getVelocity();

			void setPosition(const glm::vec3& pos);

			void setVelocity(const glm::vec3& vel);

			void setDistanceModelVariables(const float& rolloffFactor, const float& referenceDist, const float& maxDistance);

			float& getRolloffFactor();

			float& getReferenceDistance();

			float& getMaxDistance();

			void play(unsigned int* buffer = NULL);

			void bindSound(const unsigned int& buffer);

			void pause();

			void stop();

			const bool& isPlaying();

			void setLooping(const bool& loop);

			const bool& isLooping();

			void dispose();
		private:
			unsigned int id;
			glm::vec3 position, velocity;
			float rolloffFactor = 1, referenceDist = 6, maxDistance = 15, maxVolume = 1, minVolume = 0;
			bool playing, looping;

			void loadDistanceModelVariables();
	};

}
