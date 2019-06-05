#version 450

in vec4 v_Color;
in vec4 v_Temp;

uniform float u_Time;
uniform vec2 u_Points[5];

out vec4 FragColor;

const float PI = 3.1416f;

void Rader()
{
	vec2 new_UV = v_Temp.xy - vec2(0.5f, 0.5f);

	float point_Grey_Scale = 0.0f;

	// 뻗어나가는 레이더 링 그리기
	float circle_Distance = length(new_UV);
	float new_Time = fract(u_Time);
	float ring_Width = 0.1f;

	if (circle_Distance < new_Time + ring_Width && circle_Distance > new_Time)
	{
		float temp = (circle_Distance - new_Time) / ring_Width;
		point_Grey_Scale = temp;

		// 링에 닿은 포인트의 픽셀이라면 더 밝게 그리기
		for (int i = 0; i < 5; ++i)
		{
			vec2 new_Point = u_Points[i];
			vec2 new_Vec = new_Point - new_UV;
			float point_Distance = length(new_Vec);
			if (point_Distance < 0.1f)
			{
				point_Grey_Scale += 0.5f * pow(1 - point_Distance / 0.1f, 2);
			}
		}
	}

	// 완성된 픽셀 색상 적용.
	FragColor = vec4(point_Grey_Scale);

	



	// (sub) 동심원 만들기
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



void Waves()
{
	vec4 new_Color = vec4(0.0f);
	vec2 new_UV = v_Temp.xy - vec2(0.5f, 0.5f);
	for (int i = 0; i < 5; ++i)
	{
		vec2 new_Point = u_Points[i];
		vec2 new_Vec = new_Point - new_UV;
		float point_Distance = length(new_Vec) * 8.0f * PI - u_Time;
		new_Color += vec4(sin(point_Distance));
	}
	FragColor = new_Color;
}



void main()
{
	Rader();
	//Waves();
}