#version 450

// attribute ==> vertex shader input
in vec3 a_Position;
in vec2 a_Texture_UV;

// varying ==> fragment shader input
out vec2 v_Texture_UV;

uniform vec2 u_Position;
uniform vec2 u_Size;

void main()
{
	vec3 new_Pos = vec3 ((a_Position.x + u_Position.x) * u_Size.x, (a_Position.y + u_Position.y) * u_Size.y, a_Position.z);
	gl_Position = vec4(new_Pos, 1);
	v_Texture_UV = a_Texture_UV.xy;
}