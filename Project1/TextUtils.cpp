#pragma once
#include "TextUtils.h"

std::map<char, Character> characters;
unsigned int VAO;
unsigned int VBO;

void RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color)
{
	s->Use(); // Tell open GL to use this shader
	glUniform3f(glGetUniformLocation(s->ID, "textColor"), color.x, color.y, color.z); // Set our text color to the specified vector
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) // Iterate through all characters in the string
	{
		Character characterObject = characters[*c]; // Get our mapped character

		float xPos = x + characterObject.Bearing.x * scale;
		float yPos = y + (characterObject.Size.y - characterObject.Bearing.y) * scale;
		float width = characterObject.Size.x * scale;
		float height = characterObject.Size.y * scale;

		float vertices[6][4] = {
			{ xPos, yPos + height, 0.0f, 0.0f },
			{ xPos, yPos, 0.0f, 1.0f },
			{ xPos + width, yPos, 1.0f, 1.0f },

			{ xPos, yPos + height, 0.0f, 0.0f },
			{ xPos + width, yPos, 1.0f, 1.0f },
			{ xPos + width, yPos + height, 1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, characterObject.TextureID); // Bind the character's texture
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Update the VBO
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Redefine that data in the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6); // Render
		x += (characterObject.Advance >> 6) * scale; // Move to the right to prepare for the next character (bitshift by 6 to get pixel value)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}