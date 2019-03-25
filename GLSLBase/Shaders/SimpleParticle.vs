#version 450

in vec3 a_Position;
in vec3 a_Velocity;

uniform float u_Time;

void main()
{
	vec3 newPos = a_Position.xyz;

	float newTime = fract(u_Time); // fract()는 소수점 이하를 지운다. 따라서 0~1을 반복하게 된다.

	newPos += a_Velocity * newTime;

	gl_Position = vec4(newPos, 1);
}