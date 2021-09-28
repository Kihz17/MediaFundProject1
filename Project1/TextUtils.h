#pragma once
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Shader.h"


// Represents a character mapped to a free type glyph
struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

extern std::map<char, Character> characters;
extern unsigned int VAO;
extern unsigned int VBO;

// Render text to the screen
void RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color);