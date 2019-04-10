#version 450

in vec4 v_Color;
in vec4 v_Temp;

uniform vec2 u_Points[5];

out vec4 FragColor;

const float PI = 3.1416;
const vec2 c_Points[2] = {vec2(0.0f, 0.0f), vec2(0.1f, 0.1f)};


void main()
{
	vec2 new_UV = v_Temp.xy - vec2(0.5f, 0.5f);

	float point_Grey_Scale = 0.0f;
	vec2 new_Point;
	vec2 new_Vec;
	float distance;

	for (int i = 0; i < 2; ++i)
	{
		new_Point = c_Points[i];
		new_Vec = new_Point - new_UV;
		distance = sqrt(new_Vec.x * new_Vec.x + new_Vec.y * new_Vec.y);
		if (distance < 0.1f)
		{
			point_Grey_Scale += 0.5f * pow(1 - distance / 0.1f, 5);
		}
	}

	FragColor = vec4(point_Grey_Scale);

	// 동심원 만들기
	/*
	distance = sqrt(new_UV.x * new_UV.x + new_UV.y * new_UV.y);
	float grey_scale = sin(distance * 4.0f * PI * 10.0f);

	FragColor = vec4(grey_scale);

	if (distance < 0.5f && distance > 0.48f)
	{
		FragColor = vec4(1.0f);
	}
	else
	{
		FragColor = vec4(0.0f);
	}
	*/
}