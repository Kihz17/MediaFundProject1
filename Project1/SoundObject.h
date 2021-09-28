#pragma once
#include <vector>
#include "SoundUtils.h"

// Represents an object that can play sounds. This class is only a "Helper" class to allow to play and modify sounds with simple functions
// Ideally we will have an object with 1 or more SoundObjects as members
class SoundObject
{
public:
	SoundObject();

	virtual ~SoundObject();

	// Gets a sound from our sounds map. Returns NULL if the sound is not found
	virtual FMOD::Sound* GetSound(std::string soundName);

	// Play a sound for this object
	virtual void PlaySound(std::string soundName, bool isPaused);

	// Pause the audio currenty playing
	virtual void Pause();

	// UnPause the audio currenty playing
	virtual void UnPause();

	// Set the channel's frequency
	virtual void SetFrequency(float frequency);

	// Set the channel's volume
	virtual void SetVolume(float volume);

	// Set the channel's volume
	virtual void SetPan(float pan);

	// Set the channel's volume
	virtual void SetSpeed(std::string soundName, float speed);

	// Set a sound's playback speed
	virtual void SetSpeed(FMOD::Sound* sound, float speed);

private:
	FMOD::Channel* channel; // For now we will assign a channel to each object. From what I've understood, you need multiple channels to play multiple sounds at once.
};