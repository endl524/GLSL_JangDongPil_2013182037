#version 450

in vec3 a_Position; 
in vec4 a_Color;
in vec2 a_UV;

uniform float u_Time;

out vec4 v_Color;
out vec4 v_Temp;

void main()
{
	gl_Position = vec4(a_Position.xyz, 1.0f);
	v_Color = a_Color;
	v_Temp = vec4(a_UV.xy, 0.0f, 1.0f);
}