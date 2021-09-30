#pragma once
#include <FMOD/fmod.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iostream>

extern FMOD::System* _system;
extern FMOD_RESULT _result;
extern std::map<std::string, FMOD::Sound*> sounds; // Holds all loaded sounds

FMOD::Sound* CreateSound(FMOD::System* system, std::string audioFile, std::string soundName);

// Gets a sound from our sounds map. Returns NULL if the sound is not found
FMOD::Sound* GetSound(std::string soundName);

// Play a sound for this object
FMOD::Channel** PlaySound(FMOD::Channel* channel, std::string soundName, bool isPaused);

// Stop a channel from playing
void StopSound(FMOD::Channel* channel);

// Pause the audio currenty playing
void SetPaused(FMOD::Channel* channel, bool isPaused);

// Set the channel's frequency
void SetFrequency(FMOD::Channel* channel, float frequency);

// Set the channel's volume
void SetVolume(FMOD::Channel* channel, float volume);

// Set the channel's volume
void SetPan(FMOD::Channel* channel, float pan);

// Set the channel's volume
void SetSpeed(std::string soundName, float speed);

// Set a sound's playback speed
void SetSpeed(FMOD::Sound* sound, float speed);
