#version 450 // GLSL v4.5�� ����Ѵٰ� ����!

// attribute�� ���� �������� ������ �ִ�.
// uniform�� �б����� �ܺκ���.
// 

in vec4 a_Position; 
in vec4 a_Color;

uniform float u_Time;
uniform float u_Radian;

void main()
{
	float new_x, new_y;
	new_x = sin(u_Time * 3.1416f * 2.0f) + a_Position.x;
	new_y = cos(u_Time * 3.1416f * 2.0f) + a_Position.y;

	gl_Position = vec4(new_x, new_y, a_Position.z, 1);
}