#pragma once
#include <voyage.hpp>

namespace Voyage {
	class Source {
		public:
			Source(const int& buffer, const glm::vec3& position = {0.0, 0.0, 0.0}, const glm::vec3& velocity = {0.0, 0.0, 0.0}, const bool& startPlaying = false);

			~Source();

			const glm::vec3& getPosition() const;

			const glm::vec3& getVelocity() const;

			void setPosition(const glm::vec3& pos);

			void setVelocity(const glm::vec3& vel);

			void setVolume(const float& volume);

			void setDistanceModelVariables(const float& rolloffFactor, const float& referenceDist, const float& maxDistance);

			float& getRolloffFactor();

			float& getReferenceDistance();

			float& getMaxDistance();

			const float& getVolume() const;

			void play(unsigned int* const buffer = nullptr);

			void togglePlay();

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
			float rolloffFactor, referenceDist, maxDistance, volume;
			bool playing, looping;

			void loadDistanceModelVariables();
	};

}
