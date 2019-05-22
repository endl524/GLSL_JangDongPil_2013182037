#version 450 

in vec3 a_Position;

out float v_Gray_Scale;
out vec2 v_Texture_UV;

uniform float u_Time;

const float PI = 3.1416;

void main()
{
	//======================================
	// 1. ÆÞ·°ÀÌ´Â ±ê¹ß ¡Ú

	vec3 newPos = a_Position;

	float additional_Value_X = newPos.x + 0.5f; // 0.0f ~ 1.0f
	float additional_Value_Y = newPos.y + 0.5f; // 0.0f ~ 1.0f

	float sin_Period_X = 1.0f + (1.0f - additional_Value_Y) * 0.5f;
	float sin_Period_Y = 1.0f + additional_Value_X * 0.5f;
	
	// x : (1) -0.5f ~ 0.5f -> + 0.5f ----> (2) 0.0f ~ 1.0f -> * 2.0f * PI ----> (3) 0.0f ~ 2.0f
	float value_X = (additional_Value_Y * 2.0f * PI * sin_Period_X) - (u_Time * 10.0f);
	float value_Y = (additional_Value_X * 2.0f * PI * sin_Period_Y) - (u_Time * 10.0f);

	float sin_Value_X = sin(value_X) * 0.08f;
	float sin_Value_Y = sin(value_Y) * 0.2f;

	v_Gray_Scale = sin_Value_Y * additional_Value_X + 0.55f;

	newPos.y = newPos.y * ((1.0f - additional_Value_X) * 0.5f + 0.5f);

	newPos.x = newPos.x - sin_Value_X * additional_Value_X;
	newPos.y = newPos.y + sin_Value_Y * additional_Value_X * v_Gray_Scale;
	
	gl_Position = vec4(newPos.xyz, 1);

	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;

	//======================================
}