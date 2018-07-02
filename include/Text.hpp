#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>
#include <string>

#include "Shader.hpp"
#include "Camera.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H


// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};



class Text {
    private:
		GLFWwindow * window;
		Camera* camera;
		Shader* textShader;
		std::map<GLchar, Character> Characters;
		GLuint textVAO, textVBO;

    public:
        Text(GLFWwindow* theWindow, Camera* theCamera);
        void render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		void clean();
};
