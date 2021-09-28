#pragma once
#include <stdlib.h>
#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <rssgl/GLText.h>

#include "SoundUtils.h"

const int maxFmodChannels = 32;

GLuint _windowWidth = 1024;
GLuint _windowHeight = 768;

GLFWwindow* _window = NULL;
std::string _appName = "Project 1";

RSS::GLText* _text;
GLuint _textRowIndex = 2;

bool _isPaused = false;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO: Key callbacks for playing sound and whatnot
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		//_result = _system->playSound(_sound, 0, false, &_channel);
		//if (_result != FMOD_OK) {
		//	fprintf(stderr, "Unable to play sound");
		//}
	}
}

void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "FMOD Error(%d): %s", error, description);
}


bool init();
bool initGLFW();
bool initGL();
bool initFMOD();
void shutDown();

int main(int argc, char* argv) 
{
	if (!init()) {
		fprintf(stderr, "Unable to initialize app");
		exit(EXIT_FAILURE);
	}

	_text->addLine("First text", _textRowIndex++);
	GLuint lastKnownRowIndex = _textRowIndex;

	while (!glfwWindowShouldClose(_window)) 
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lastKnownRowIndex = _textRowIndex;
		_text->addLine("Test line", lastKnownRowIndex++);

		_text->render();

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

	//GLText has a dependency with OpenGL context
	_text = new RSS::GLText(_appName, _windowWidth, _windowHeight);

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

	return true;
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

	// TODO: Load our sound files in
	//for ()
	//{
	//	CreateSound(_system, );
	//}

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

	if (_text) {
		delete _text;
		_text = nullptr;
	}


	exit(EXIT_SUCCESS);
}