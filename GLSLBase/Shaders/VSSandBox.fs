#version 450

in float v_Grey_Scale;
in vec2 v_Texture_UV;

out vec4 FragColor;

uniform sampler2D u_Texture;

void Flag()
{
	vec2 texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = texture(u_Texture, texture_UV);

	FragColor = vec4(new_Color.xyz * v_Grey_Scale, 1.0f);
}

void Wave()
{
    vec2 texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = vec4(1.0f);

	FragColor = vec4(new_Color.xyz * v_Grey_Scale, 1.0f);
}

void Sphere_Mapping()
{
    vec2 texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = vec4(1.0f);

	FragColor = vec4(new_Color.xyz * v_Grey_Scale, 1.0f);
}



void main()
{
	//Flag();
	//Wave();
	Sphere_Mapping();
}