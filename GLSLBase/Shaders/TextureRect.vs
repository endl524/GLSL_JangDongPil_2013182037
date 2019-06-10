#version 450

in vec3 a_Position;
in vec2 a_Texture_UV;

out vec2 v_Texture_UV;

uniform vec2 u_Position;
uniform vec2 u_Size;

void main()
{
	gl_Position = vec4(a_Position.xy * u_Size.xy + u_Position.xy, 0.0f, 1.0f);
	v_Texture_UV = a_Texture_UV.xy;
}