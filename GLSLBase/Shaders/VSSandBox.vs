#version 450 

in vec3 a_Position;

uniform float u_Time;

const float PI = 3.1416;

void main()
{
	vec3 newPos = a_Position;
	
	// x : -0.5f ~ 0.5f ---> + 0.5f ----> 0.0f ~ 1.0f ---> * 2.0f * PI ---> 0.0f ~ 2.0f
	float value = (newPos.x + 0.5f) * 2.0f * PI + u_Time * 2.0f;
	float sinValue = sin(value) * 0.5f;
	
	// y : 
	newPos.y = newPos.y + sinValue;

	gl_Position = vec4(newPos.xyz, 1);
}