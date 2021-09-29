#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <rssgl/GLText.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "SoundUtils.h"
#include "TextUtils.h"
#include "StoryScript.h"

const int maxFmodChannels = 32;

GLuint _windowWidth = 1024;
GLuint _windowHeight = 768;

GLFWwindow* _window = NULL;
std::string _appName = "Project 1";

bool _isPaused = false;

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
bool initFreeType();
bool initFMOD();
void shutDown();
void PlayEffectForStep(int i) {

}
int main(int argc, char* argv) 
{
	if (!init()) {
		fprintf(stderr, "Unable to initialize app");
		exit(EXIT_FAILURE);
	}

	while (!glfwWindowShouldClose(_window)) 
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Playing the story
		StoryScript story = StoryScript();
		bool theEnd = false;
		const float lineSpace = 50.0f;
		while(!theEnd)
		{
			StoryStep step = story.GetNextLine();
			theEnd = step.isLast;
			//TODO play step.theme case exists otherwise keep the last playing
			//Show Story description on window on a different line
			RenderText(shader, step.description, 300.0f , 300.0f + lineSpace, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
			//TODO play the effects list in order step.effects
			std::this_thread::sleep_for(std::chrono::seconds(step.waitTime));
			glfwSwapBuffers(_window);
			glfwPollEvents();
		}

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

	delete shader;

	exit(EXIT_SUCCESS);
}