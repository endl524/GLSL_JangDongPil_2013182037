#version 450 

in vec3 a_Position;
in vec3 a_Normal;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;
uniform mat4 u_ProjView_Matrix;

const float PI = 3.1416;


void main()
{
	gl_Position = u_ProjView_Matrix * vec4(a_Position.xyz, 1.0f);

	v_Color = a_Color;
}