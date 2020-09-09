#version 450 core
uniform sampler2D Main_Tex;

out vec4 FragColor;
in vec2 TexCoord;
void main()
{
	vec3 color = vec3(219.0/255.0, 188.0/255.0, 33.0/255.0);

	FragColor = vec4(0.2, 0.2, 0.2, 0.9);

}