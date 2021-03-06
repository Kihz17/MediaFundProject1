#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <vector>
#include <rssgl/GLText.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "SoundUtils.h"
#include "TextUtils.h"
#include "FileUtils.h"

const int maxFmodChannels = 32;

GLuint _windowWidth = 1024;
GLuint _windowHeight = 768;

GLFWwindow* _window = NULL;
std::string _appName = "Project 1";
FMOD::Channel* _channel = NULL;

bool _isPaused = false;
bool _isMuted = false;
float vol = 1.0f; //volume
float pitch = 1.0f; //pitch which controls frequency which is proportional to speed
float pos = 0; //position of audio coming out of headphone (left, right, or both)

static const char* vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec4 vertex;\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	 gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
"    TexCoords = vertex.zw;\n"
"}\n";

static const char* fragment_shader =
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"
"void main()\n"
"{\n"
"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"    color = vec4(textColor, 1.0) * sampled;\n"
"}\n";

Shader* shader;

std::vector<std::string> _audioAssetsList;
FMOD::Channel* bgmChannel = NULL;
FMOD::Channel* effChannel = NULL; //copy sound effects of effectChannels to _channel for editing purposes
std::vector<FMOD::Channel*> effectChannels;

std::map<std::string, FMOD::Channel*> playingSounds;
std::vector<std::string> keys;
int keyIndex = 0;

// Represents one "step" in the story
struct StoryStep
{
	std::string description;
	std::string theme;
	std::vector<std::string> effects;
	long waitTime;

public:
	void PlayTheme()
	{
		if (!theme.empty())
		{
			bgmChannel = *PlaySound(bgmChannel, theme, false);
			keys.push_back(theme);
			playingSounds.insert(std::pair<std::string, FMOD::Channel*>(theme, bgmChannel));
		}
	}

	void PlayEffects()
	{
		FMOD::Channel* channel = NULL;
		for (std::string effect : effects)
		{
			effChannel = *PlaySound(channel, effect, false);
			//effectChannels.push_back(*PlaySound(channel, effect, false));
			keys.push_back(effect);
			playingSounds.insert(std::pair<std::string, FMOD::Channel*>(effect, channel));
		}
	}
};

StoryStep story[10] =
{
	{"Once Upon a time...", "music_magical_story_intro.wav",  {"bird_small_song_call_chirp_02.wav"}, 20},
	{"In the calm forest of Greenwood, there was a gnome called Chepart ...", "",  {"background_crowd_people_chatter_loop_02.wav"}, 120},
	{"He lived in the city of Hazelward over the top of the trees ...", "",  {}, 120},
	{"He loved how the people there were so festive ....", "music_calm_green_lake_serenade.wav",  {}, 80},
	{"Chepart used to enjoy fishing at Crystal lake every afternoon ...", "",  {"river_stream_daytime_flowing_water_insects_birds_loop_01.wav","crickets_chirping_night_ambience_loop.wav" }, 120},
	{"One day he returned from the lake and had a strange feeling ...", "",  { "fantasy_jungle_forrest_loop_01.wav","swamp_bayou_frogs_birds_daytime_loop1.wav" }, 120},
	{"When he arrived at Hazelward, everyone in the city had become stone ...", "cinematic_LowDrone1.wav",  {}, 120},
	{"What in the world could have done such terrible thing ...", "",  { "bird_crow_call_caw_squawk_01.wav","shimmer_sparkle_loop_02.wav" }, 80},
	{"", "music_cinematic_reveal.wav",  { "cinematic_deep_low_whoosh_impact_02.wav" }, 120},
	{"", "", {}, 120}
};

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO: Key callbacks for playing sound and whatnot
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) { //edit background music
		_channel = &(*bgmChannel);
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS){ //edit sound effects
		_channel = &(*effChannel);
	}
	//controls are the same 1 or 2 just determines which one you are editing
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	/*
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		keyIndex = std::min((int) (keys.size() - 1), keyIndex + 1);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		keyIndex = std::max(0, keyIndex - 1);
	}
	*/
	else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		//if current sound is paused=>unpause else pause
		if (_channel) {
			_result = _channel->getPaused(&_isPaused);
			_result = _channel->setPaused(!_isPaused);
			_result = _channel->getPaused(&_isPaused);
		}
	}
	//Cancel all music from playing
	else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (_channel) {
			_result = _channel->stop();
		}
	}
	//Mute
	else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (_channel) {
			_result = _channel->getMute(&_isMuted);
			_result = _channel->setMute(!_isMuted);
			_result = _channel->getMute(&_isMuted);
		}
	}
	//Volume control
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && vol > 0.1f) {
		if (_channel) {
			_result = _channel->getAudibility(&vol);
			_result = _channel->setVolume(vol - 0.1f);
			_result = _channel->getAudibility(&vol);
		}
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && vol < 1.0f) {
		if (_channel) {
			_result = _channel->getAudibility(&vol);
			_result = _channel->setVolume(vol + 0.1f);
			_result = _channel->getAudibility(&vol);
		}
	}
	//pitch control
	else if (key == GLFW_KEY_U && action == GLFW_PRESS && pitch < 2.0f) {
		if (_channel) {
			_result = _channel->getPitch(&pitch);
			_result = _channel->setPitch(pitch + 0.01f);
			_result = _channel->getPitch(&pitch);
		}
	}
	else if (key == GLFW_KEY_J && action == GLFW_PRESS && pitch > 0.0f) {
		if (_channel) {
			_result = _channel->getPitch(&pitch);
			_result = _channel->setPitch(pitch - 0.01f);
			_result = _channel->getPitch(&pitch);
		}
	}
	//panning
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {//right headphone
		if (_channel) {
			pos = -1;
			_result = _channel->setPan(pos);
		}
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {//left headphone
		if (_channel) {
			pos = 1;
			_result = _channel->setPan(pos);
		}
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {//center
		if (_channel) {
			pos = 0;
			_channel->setPan(pos);
		}
	}
}

void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "FMOD Error(%d): %s", error, description);
}


bool init();
bool initGLFW();
bool initGL();
bool initFreeType();
bool initFMOD();
void shutDown();

int main(int argc, char* argv) 
{
	if (!init()) {
		fprintf(stderr, "Unable to initialize app");
		exit(EXIT_FAILURE);
	}

	int currentStoryIndex = 0;
	float lineHeight = 500.0f;
	int storyLength = sizeof(story) / sizeof(story[0]);
	int ticks = 0;
	std::vector<std::pair<std::string, float>> linesToRender;

	StoryStep& step = story[currentStoryIndex];
	FMOD::Channel* lastThemeChannel = NULL;

	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Playing the story
		bool finishedStory = currentStoryIndex >= storyLength;
		if (!finishedStory && ticks % step.waitTime == 0)
		{
			StoryStep lastStep = step;

			// TODO: Do the sound modification here (frequency, volume, pitch, etc)
			if (!finishedStory)
			{
				step = story[currentStoryIndex++];
			}

			// Stop current sound effects if we have new ones
			if (!step.theme.empty())
			{
				_result = bgmChannel->stop();

				// Remove the key from the key vector
				std::vector<std::string>::iterator it;
				for (it = keys.begin(); it != keys.end(); it++)
				{
					if (step.theme == *it)
					{
						it = keys.erase(it);
						break;
					}
				}

				playingSounds.erase(step.theme);
			}

			if (!step.effects.empty())
			{
				for (FMOD::Channel* c : effectChannels)
				{
					c->stop();
				}

				// Remove the key from the key vector
				std::vector<std::string>::iterator it;
				for (it = keys.begin(); it != keys.end(); it++)
				{
					for (std::string effect : step.effects)
					{
						if (effect == *it)
						{
							it = keys.erase(it);
							break;
						}
					}
				}

				for (std::string effect : step.effects)
				{
					playingSounds.erase(effect);
				}
			}

			lineHeight -= 60.0f;
			linesToRender.push_back(std::pair<std::string, float>(step.description, lineHeight));
			step.PlayTheme();
			step.PlayEffects();
		}

		std::string selectedSound = keys[keyIndex];
		// Current sound is no longer valid, get the first entry in map
		if (!playingSounds.count(selectedSound))
		{
			if (!keys.empty())
			{
				selectedSound = keys[0];
			}
		}

		for (std::pair<std::string, float> pair : linesToRender)
		{
			RenderText(shader, pair.first, 100.0f, pair.second, 0.8f, glm::vec3(0.5, 0.8f, 0.2f));
		}

		RenderText(shader, "Current Selection: " + selectedSound, 200.0f, 700.0f, 0.8f, glm::vec3(0.5, 0.8f, 0.2f));

		_system->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		ticks++;
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	shutDown();
}



bool init() {

	if (!initGLFW()) {
		return false;
	}
	if (!initGL()) {
		return false;
	}
	if (!initFreeType()) {
		return false;
	}

	if (!initFMOD()) {
		return false;
	}

	return true;
}

bool initGLFW() {

	if (!glfwInit()) {
		fprintf(stderr, "Unable to initalize GLFW");
		return false;
	}

	//set error callback
	glfwSetErrorCallback(glfw_error_callback);

	_window = glfwCreateWindow(_windowWidth, _windowHeight, _appName.c_str(), NULL, NULL);
	if (!_window) {
		fprintf(stderr, "Unable to create GLFW window");
		return false;
	}

	//set keyboard callback
	glfwSetKeyCallback(_window, KeyCallback);
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);


	return true;
}

bool initGL() {

	if (!gladLoadGL(glfwGetProcAddress)) {
		fprintf(stderr, "Unable to initialize glad");
		return false;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Setup text shaders
	shader = new Shader(vertex_shader, fragment_shader);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(_windowWidth), 0.0f, static_cast<float>(_windowHeight)); // set up projection matrix so that we can choose where we render text on the screen
	shader->Use();
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); // Specify the projection value for our shader

	return true;
}

bool initFreeType()
{
	FT_Library ft; // Initialize the freetype library
	if (FT_Init_FreeType(&ft) != 0)
	{
		fprintf(stderr, "Unable to initialize FreeType Library");
		return false;
	}

	FT_Face face;
	std::stringstream ss;
	ss << SOLUTION_DIR << "Extern\\fonts\\arial.ttf";
	if (FT_New_Face(ft, ss.str().c_str(), 0, &face) != 0) // Initialize our font
	{
		fprintf(stderr, "Unable to initialize FreeType Font");
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 32);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
	
	// Map the first 128 ASCII characters to our Character struct
	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
		{
			fprintf(stderr, "Unable to load Glyph");
			return false;
		}

		unsigned int texture;
		glGenTextures(1, &texture); // Generates a texture (first param = # of textures to be generated, second param = where texture is stored)
		glBindTexture(GL_TEXTURE_2D, texture); // Binds a 2d texture
		glTexImage2D( // Specifies a 2D texture image
			GL_TEXTURE_2D,
			0, // Level of detail (0 = base level)
			GL_RED, // The color components in the texture
			face->glyph->bitmap.width, // Width of the image
			face->glyph->bitmap.rows, // Height of the image
			0, // Border (should always be 0)
			GL_RED, // The format of our pixel data
			GL_UNSIGNED_BYTE, //  The type of pixel data from our image
			face->glyph->bitmap.buffer // Our actual image data
		);

		// Texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = { texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x };
		characters.insert(std::pair<char, Character>(c, character)); // Insert the generated character into our map
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// Free up resources after generating text glyphs
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Set up vertex arrays and vertex buffers
	glGenVertexArrays(1, &VAO); // Generate 1 vertex array
	glGenBuffers(1, &VBO); // Generate 1 vertex buffer
	glBindVertexArray(VAO); // Bind our newly generated vertex array
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind our newly generated vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // Create a new location to store our buffer data (since the VBO is updated often we use GL_DYNAMIC_DRAW)
	glEnableVertexAttribArray(0); // Enable vertex array at index 0
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); // Specify vertex array at index 0 to have 4 components of type float that are not normalized with a byte offset of (4 * sizeof(float))
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool initFMOD() {
	_result = FMOD::System_Create(&_system);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to create FMOD system");
		return false;
	}

	_result = _system->init(maxFmodChannels, FMOD_INIT_NORMAL, NULL);
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to initialize FMOD system");
		return false;
	}

    _audioAssetsList = GetFilesFromIndex("audioAssetList.txt");

	for (size_t i = 0; i < _audioAssetsList.size(); i++)
	{
		std::string soundName = _audioAssetsList.at(i);
		CreateSound(_system, soundName, soundName);
	}

	return true;
}

void shutDown() {

	glfwTerminate();

	// Release all of our sounds from memory
	std::map<std::string, FMOD::Sound*>::iterator it = sounds.begin();
	while (it != sounds.end())
	{
		FMOD::Sound* sound = it->second;
		_result = sound->release();
		if (_result != FMOD_OK)
		{
			fprintf(stderr, "Unable to release sound");
		}
	}
	sounds.clear();

	_result = _system->close();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to close system");
	}

	_result = _system->release();
	if (_result != FMOD_OK) {
		fprintf(stderr, "Unable to release system");
	}

	delete shader;

	exit(EXIT_SUCCESS);
}