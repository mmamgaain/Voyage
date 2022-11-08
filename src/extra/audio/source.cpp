#include "Voyage/source.hpp"
#include "openal/al.h"
#include "openal/alc.h"

namespace Voyage {
	Source::Source(const int& buffer, const glm::vec3& position, const glm::vec3& velocity, const bool& startPlaying): id(0), position({0, 0, 0}), velocity({0, 0, 0}), rolloffFactor(1), referenceDist(6), maxDistance(15), volume(1), playing(false), looping(false) {
		alGenSources(1, &id);
		alSourcef(id, AL_MAX_GAIN, 1);
		alSourcef(id, AL_MIN_GAIN, 0);
		alSourcef(id, AL_PITCH, 1);
		alSourcef(id, AL_GAIN, volume);
		alSource3f(id, AL_POSITION, position.x, position.y, position.z);
		alSource3f(id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		setPosition(position);
		setVelocity(velocity);
		bindSound(buffer);
		loadDistanceModelVariables();
		if(startPlaying) play();
	}

	Source::~Source() { dispose(); }

	const glm::vec3& Source::getPosition() const { return position; }

	const glm::vec3& Source::getVelocity() const { return velocity; }

	void Source::setPosition(const glm::vec3& pos) { position.x = pos.x; position.y = pos.y; position.z = pos.z; }

	void Source::setVelocity(const glm::vec3& vel) { velocity.x = vel.x; velocity.y = vel.y; velocity.z = vel.z; }

	void Source::setVolume(const float& volume) { this->volume = volume; }

	float& Source::getRolloffFactor() { return rolloffFactor; }

	float& Source::getReferenceDistance() { return referenceDist; }

	float& Source::getMaxDistance() { return maxDistance; }

	const float& Source::getVolume() const { return volume; }

	void Source::play(unsigned int* const buffer) { if(!playing) { if(buffer) bindSound(*buffer); alSourcePlay(id); playing = true; } }

	void Source::togglePlay() { if(playing) pause(); else play(); }

	void Source::bindSound(const unsigned int& buffer) { alSourcei(id, AL_BUFFER, buffer); }

	void Source::pause() { playing = false; alSourcePause(id); }

	void Source::stop() { playing = false; alSourceStop(id); }

	const bool& Source::isPlaying() { return playing; }

	void Source::setLooping(const bool& loop) { looping = loop; alSourcei(id, AL_LOOPING, loop); }

	const bool& Source::isLooping() { return looping; }

	void Source::dispose() { alDeleteSources(1, &id); }

	void Source::loadDistanceModelVariables() {
		alSourcef(id, AL_ROLLOFF_FACTOR, rolloffFactor);
		alSourcef(id, AL_REFERENCE_DISTANCE, referenceDist);
		alSourcef(id, AL_MAX_DISTANCE, maxDistance);
	}

}
