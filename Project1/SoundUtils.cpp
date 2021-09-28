#pragma once
#include "SoundUtils.h"

FMOD::System* _system = NULL;
FMOD_RESULT _result = FMOD_OK;
std::map<std::string, FMOD::Sound*> sounds;

FMOD::Sound* CreateSound(FMOD::System* system, std::string audioFile, std::string soundName)
{
	std::stringstream ss;
	ss << SOLUTION_DIR << "common\\assets\\audio\\" << audioFile;

	FMOD::Sound* sound = NULL;
	_result = system->createSound(ss.str().c_str(), FMOD_LOOP_OFF, 0, &sound);

	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create sound.");
		return NULL;
	}

	sounds.insert(std::pair<std::string, FMOD::Sound*>(soundName, sound)); // Store our newly loaded sound into a map indexed by the soundName

	ss.flush();
	return sound;
}

FMOD::Sound* GetSound(std::string soundName)
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


void PlaySound(FMOD::Channel* channel, std::string soundName, bool isPaused)
{
	FMOD::Sound* sound = GetSound(soundName);
	if (sound != NULL)
	{
		_system->playSound(sound, 0, isPaused, &channel);
	}
}

void SetPaused(FMOD::Channel* channel, bool isPaused)
{
	_result = channel->setPaused(isPaused);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to pause channel.");
	}
}

void SetFrequency(FMOD::Channel* channel, float frequency)
{
	_result = channel->setFrequency(frequency);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set channel frequency.");
	}
}

void SetVolume(FMOD::Channel* channel, float volume)
{
	_result = channel->setVolume(volume);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set channel volume.");
	}
}

void SetPan(FMOD::Channel* channel, float pan)
{
	_result = channel->setPan(pan);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set channel pan.");
	}
}

void SetSpeed(std::string soundName, float speed)
{
	FMOD::Sound* sound = GetSound(soundName);
	if (sound == NULL)
	{
		fprintf(stderr, "Failed to set sound speed.");
		return;
	}

	SetSpeed(sound, speed);
}

void SetSpeed(FMOD::Sound* sound, float speed)
{
	_result = sound->setMusicSpeed(speed);
	if (_result != FMOD_OK)
	{
		fprintf(stderr, "Unable to set sound speed.");
	}
}