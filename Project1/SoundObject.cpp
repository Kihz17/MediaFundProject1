#pragma once
#include "SoundObject.h"

SoundObject::SoundObject()
{
	this->channel = NULL;
}

SoundObject::~SoundObject()
{
}

FMOD::Sound* SoundObject::GetSound(std::string soundName)
{
	std::map<std::string, FMOD::Sound*>::iterator it = sounds.find(soundName);
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
		_system->playSound(sound, 0, isPaused, &this->channel);
	}
}

void SoundObject::Pause()
{
	_result = this->channel->setPaused(true);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to pause channel.");
	}
}

void SoundObject::UnPause()
{
	_result = this->channel->setPaused(false);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to unpause channel.");
	}
}

void SoundObject::SetFrequency(float frequency)
{
	_result = this->channel->setFrequency(frequency);
	if (_result != FMOD_OK) 
	{
		fprintf(stderr, "Unable to set channel frequency.");
	}
}

void SoundObject::SetVolume(float volume)
{
	_result = this->channel->setVolume(volume);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set channel volume.");
	}
}

void SoundObject::SetPan(float pan)
{
	_result = this->channel->setPan(pan);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set channel pan.");
	}
}

void SoundObject::SetSpeed(std::string soundName, float speed)
{
	FMOD::Sound* sound = this->GetSound(soundName);
	if (sound == NULL)
	{
		fprintf(stderr, "Failed to set sound speed.");
		return;
	}

	this->SetSpeed(sound, speed);
}

void SoundObject::SetSpeed(FMOD::Sound* sound, float speed)
{
	_result = sound->setMusicSpeed(speed);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set sound speed.");
	}
}