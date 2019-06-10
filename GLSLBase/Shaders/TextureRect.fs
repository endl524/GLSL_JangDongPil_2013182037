#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
	FragColor = texture(u_Texture, v_Texture_UV);
}