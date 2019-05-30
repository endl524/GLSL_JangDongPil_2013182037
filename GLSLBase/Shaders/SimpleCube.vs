#version 450 

in vec3 a_Position;
in vec3 a_Normal;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;
uniform mat4 u_ProjView_Matrix;

const float PI = 3.1416;


//VSSandBox ¿¹Á¦¿ë========
out float v_Grey_Scale;
out vec2 v_Texture_UV;
uniform vec2 u_Points[5];
//=======================

void Flag()
{
	vec3 newPos = a_Position;

	float additional_Value_X = newPos.x + 0.5f; // 0.0f ~ 1.0f
	float additional_Value_Y = newPos.y + 0.5f; // 0.0f ~ 1.0f

	float sin_Period_X = 1.0f + (1.0f - additional_Value_Y) * 0.5f;
	float sin_Period_Y = 1.0f + additional_Value_X * 0.5f;
	
	float value_X = (additional_Value_Y * 2.0f * PI * sin_Period_X) - (u_Time * 10.0f);
	float value_Y = (additional_Value_X * 2.0f * PI * sin_Period_Y) - (u_Time * 10.0f);

	float sin_Value_X = sin(value_X) * 0.08f;
	float sin_Value_Y = sin(value_Y) * 0.2f;

	v_Grey_Scale = sin_Value_Y * additional_Value_X + 0.55f;

	newPos.y = newPos.y * ((1.0f - additional_Value_X) * 0.5f + 0.5f);

	newPos.x = newPos.x - sin_Value_X * additional_Value_X;
	newPos.y = newPos.y + sin_Value_Y * additional_Value_X * v_Grey_Scale;
	newPos.z = v_Grey_Scale - 0.5f;

	gl_Position = u_ProjView_Matrix * vec4(newPos.xyz, 1);

	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}


void Wave()
{
	float grey = 0.0f;
	float distance = 0.0f;
	
	for(int i = 0; i < 5; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 8.0f * PI * float(i);
		grey += sin(distance - u_Time * 4.0f);
	}

	vec3 new_Pos = a_Position.xyz;
	new_Pos.z = grey / 100.0f;

	v_Grey_Scale = (grey + 1.0f) / 2.0f;

	gl_Position = u_ProjView_Matrix * vec4(new_Pos.xyz, 1);
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void Sphere_Mapping()
{
	float grey = 0.0f;
	float distance = 0.0f;

	for(int i = 0; i < 5; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 2.0f * PI * float(i);
		grey += sin(distance - u_Time * 4.0f);
	}

	float r = 0.5f + abs(grey) * 0.1f;
	float beta = (a_Position.x + 0.5f) * 4.0f * PI;
	float theta = (a_Position.y + 0.5f) * 2.0f * PI;
	
	vec3 sphere_Pos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);

	//vec3 original_Pos = a_Position.xyz;
	//vec3 new_Pos = mix(original_Pos, sphere_Pos, fract(u_Time));
	
	gl_Position = u_ProjView_Matrix * vec4(sphere_Pos.xyz, 1.0f);
	v_Grey_Scale = 1.0f;
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void Cube()
{
	gl_Position = u_ProjView_Matrix * vec4(a_Position, 1.0f);
	
	v_Color = a_Color;
}



void main()
{
	//Flag();
	//Wave();
	//Sphere_Mapping();

	Cube();
}