#pragma once
#include "SoundObject.h"

SoundObject::SoundObject(FMOD::System* system)
{
	this->system = system;
	this->channel = NULL;
}

SoundObject::~SoundObject()
{
	// Release all of out sounds from memory
	std::map<std::string, FMOD::Sound*>::iterator it = sounds.begin();
	while (it != sounds.end())
	{
		FMOD::Sound* sound = it->second;
		FMOD_RESULT result = sound->release();
		if (result != FMOD_OK) 
		{
			fprintf(stderr, "Unable to release sound");
		}
	}
	sounds.clear();
}

void SoundObject::LoadSound(std::string soundFile, std::string soundName)
{
	FMOD::Sound* newSound = CreateSound(this->system, soundFile);
	if (newSound == NULL)
	{
		std::string error = "There was an error loading sound " + soundFile + " as " + soundName + ".";
		fprintf(stderr, error.c_str());
	}

	this->sounds.insert(std::pair<std::string, FMOD::Sound*>(soundName, newSound)); // Store our newly loaded sound into a map indexed by the soundName
}

FMOD::Sound* SoundObject::GetSound(std::string soundName)
{
	std::map<std::string, FMOD::Sound*>::iterator it = this->sounds.find(soundName);
	if (it != sounds.end()) // We found the sound
	{
		return it->second;
	}
	else // No sound was found
	{
		return NULL;
	}
}

void SoundObject::PlaySound(std::string soundName, bool isPaused)
{
	FMOD::Sound* sound = this->GetSound(soundName);
	if (sound != NULL)
	{
		this->system->playSound(sound, 0, isPaused, &this->channel);
	}
}

void SoundObject::Pause()
{
	this->channel->setPaused(true);
}

void SoundObject::UnPause()
{
	this->channel->setPaused(false);
}