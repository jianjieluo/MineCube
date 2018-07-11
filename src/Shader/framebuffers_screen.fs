#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

const float offset = 1.0 / 300.0; 

uniform sampler2D screenTexture;
uniform int specialEffect;

void main()
{
	if (specialEffect == 1) {
		FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	}
	else if (specialEffect == 2) {
		FragColor = texture(screenTexture, TexCoords);
		float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
		FragColor = vec4(average, average, average, 1.0);
	} 
	else if (specialEffect == 3 || specialEffect == 4) {
		vec2 offsets[9] = vec2[](
				vec2(-offset,  offset), // 左上
				vec2( 0.0f,    offset), // 正上
				vec2( offset,  offset), // 右上
				vec2(-offset,  0.0f),   // 左
				vec2( 0.0f,    0.0f),   // 中
				vec2( offset,  0.0f),   // 右
				vec2(-offset, -offset), // 左下
				vec2( 0.0f,   -offset), // 正下
				vec2( offset, -offset)  // 右下
		);

		float kernel1[9] = float[](
        			-1, -1, -1,
        			-1,  9, -1,
        			-1, -1, -1
   		 );

		float kernel2[9] = float[](
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16  
		);

		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++) {
			if (specialEffect == 3)  {
				col += sampleTex[i] * kernel1[i];
			} else {
				col += sampleTex[i] * kernel2[i];
			}
		}

		FragColor = vec4(col, 1.0);
	}
	else {
		FragColor = vec4(texture(screenTexture, TexCoords).rgb, 1.0);
	}
	// Cause skybox here too so we apply gamma Correction in phongshader.fs
} 