#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
	vec2 new_Texture_UV = v_Texture_UV + vec2(0.5f, 0.5f);
	FragColor = texture(u_Texture, v_Texture_UV);
}