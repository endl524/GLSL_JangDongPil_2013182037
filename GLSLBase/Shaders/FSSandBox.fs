#version 450

in vec4 v_Color;
in vec4 v_Temp;

uniform float u_Time;
uniform vec2 u_Points[5];

out vec4 FragColor;

const float PI = 3.1416f;





void Rader()
{
    // 1. 중심에서 부터 밖으로 뻗어나가는 레이더.
    // 레이더에 닿은 점은 밝에 표시 된다.

	vec2 new_UV = v_Temp.xy - vec2(0.5f, 0.5f);

	float point_Grey_Scale = 0.0f;

	float circle_Distance = length(new_UV);
	float new_Time = fract(u_Time * 0.5f); // 0.0 ~ 1.0 Repeat
	float ring_Width = new_Time * 2.0f; // 레이더 링의 반지름. (0.0 ~ 5.0 시간에 따라 점점 커짐.)
    float point_Width = 0.05f; // 포인트의 반지름.

    // 시간이 흐름에 따라 레이더가 움직이는것을 표현하기 위해,
    // 레이더가 그려져야 할 UV 위치 < 0.0 ~ 3.0
	if (circle_Distance < new_Time + ring_Width)
	{
        // 뻗어나가는 레이더 링 그리기.
		point_Grey_Scale = (circle_Distance - new_Time) / ring_Width;

		// 레이더 링과 가까운 점은 밝게 그리기.
		for (int i = 0; i < 5; ++i)
		{
			vec2 new_Vec = u_Points[i] - new_UV;
			float point_Distance = length(new_Vec);

            // 포인트와 링 사이의 거리가 포인트의 반지름 보다 작으면 그린다. (따라서, 포인트는 원으로 그려짐)
			if (point_Distance < point_Width) point_Grey_Scale += 0.2f;// * pow(1.0f - point_Distance, 2);
		}
	}

	FragColor = vec4(point_Grey_Scale);
}


void Circle()
{
    // 2. 동심원 만들기
    // "distance * 4.0f * PI" 를 해준다는것은, sin 곡선의 2주기를 표현하고 있다는 것이다.
    // 즉, 동심원이 2개 그려진다.

    vec2 new_UV = v_Temp.xy - vec2(0.5f, 0.5f);
    float distance = length(new_UV);
	float grey_scale = sin(distance * 4.0f * PI);

	FragColor = vec4(grey_scale);
}


void Waves()
{
    // 3. 물결무늬 만들기.

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
    //Circle();
	//Waves();
}