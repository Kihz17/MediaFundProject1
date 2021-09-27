#pragma once
#include <vector>
#include <string>
#include <map>
#include <FMOD/fmod.hpp>
#include "SoundUtils.h"

// Represents an object that can play multiple sounds
// Ideally we will have an object that derives from this class to load "object" specifc sounds into. 
// Example:
// We create a new class called "Truck" that derives from this class
// We load a sound honk.mp3 as "Honk" and break.mp3 as "Break"
// We can now create a function for each sound (Honk() and Break()) which will play the corresponding sound by indexing our sounds map by its key ("Honk" or "Break")
class SoundObject
{
public:
	SoundObject(FMOD::System* system);

	virtual ~SoundObject();

	// Loads a sound into this object
	virtual void LoadSound(std::string soundFile, std::string soundName);

	// Gets a sound from our sounds map. Returns NULL if the sound is not found
	virtual FMOD::Sound* GetSound(std::string soundName);

	// Play a sound for this object
	virtual void PlaySound(std::string soundName, bool isPaused);

	// Pause the audio currenty playing
	virtual void Pause();

	// UnPause the audio currenty playing
	virtual void UnPause();
private:
	FMOD::System* system; // We need a pointer to the system to be able to load an unload sounds
	FMOD::Channel* channel; // For now we will assign a channel to each object. From what I've understood, you need multiple channels to play multiple sounds at once.
	std::map<std::string, FMOD::Sound*> sounds; // Holds all of this object's loaded sounds
};