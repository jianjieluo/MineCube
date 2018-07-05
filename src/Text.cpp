#include "Text.hpp"

Text::Text(GLFWwindow* theWindow, Camera* theCamera) {
	//::cout << "init Text" << std::endl;
	window = theWindow;
	camera = theCamera;

	textShader = new Shader("../src/Shader/text.vs", "../src/Shader/text.fs");

	// text control
	lastText = "";
	old = 0;
	textScale = 3.0f;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "../Asset/font/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::push(std::string next, glm::vec3 color, int life) {
	textColor = color;
	textLife = life;
	old = 0;
	lastText = next;

	// cal the string width & height
	GLfloat x = 0.0f, y = 0.0f;
	std::string::const_iterator c;
	for (c = lastText.begin(); c != lastText.end(); c++)
	{
		Character ch = Characters[*c];
		x += (ch.Advance >> 6) * textScale;
	}

	textX = - x / 2;
	textY = y;
}


void Text::render() {
	if (old > textLife) {
		// do nothing
	}
	else {
		old = old + 1;
		int x = textX, y = textY;
		glm::vec4 color(
			textColor.r,
			textColor.g,
			textColor.b,
			1.0 - 1.0 * (static_cast<GLfloat>(old)/ static_cast<GLfloat>(textLife))
		);


		// Set OpenGL options
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), 0.0f, static_cast<GLfloat>(screenHeight));  // here is a bug!
		textShader->setMat4("projection", projection);

		// Activate corresponding render state	
		textShader->use();
		textShader->setVec4("textColor", color);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textVAO);

		// Iterate through all characters
		std::string::const_iterator c;
		for (c = lastText.begin(); c != lastText.end(); c++)
		{
			Character ch = Characters[*c];

			GLfloat xpos = x + ch.Bearing.x * textScale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * textScale;

			GLfloat w = ch.Size.x * textScale;
			GLfloat h = ch.Size.y * textScale;
			// Update VBO for each character
			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			for (int i = 0; i < 6; i++) {
				vertices[i][0] = vertices[i][0] / screenWidth;
				vertices[i][1] = vertices[i][1] / screenHeight;
			}
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			// glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);


			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * textScale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Text::clean() {
	// code
}
