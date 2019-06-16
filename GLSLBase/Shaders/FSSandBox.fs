#version 450

in vec4 v_Color;
in vec4 v_Temp;

uniform float u_Time;
uniform vec2 u_Points[5];

out vec4 FragColor;

const float PI = 3.1416f;





void Rader()
{
    // 1. �߽ɿ��� ���� ������ ������� ���̴�.
    // ���̴��� ���� ���� �࿡ ǥ�� �ȴ�.

	vec2 new_UV = v_Temp.xy - vec2(0.5f, 0.5f);

	float point_Grey_Scale = 0.0f;

	float circle_Distance = length(new_UV);
	float new_Time = fract(u_Time * 0.5f); // 0.0 ~ 1.0 Repeat
	float ring_Width = new_Time * 2.0f; // ���̴� ���� ������. (0.0 ~ 5.0 �ð��� ���� ���� Ŀ��.)
    float point_Width = 0.05f; // ����Ʈ�� ������.

    // �ð��� �帧�� ���� ���̴��� �����̴°��� ǥ���ϱ� ����,
    // ���̴��� �׷����� �� UV ��ġ < 0.0 ~ 3.0
	if (circle_Distance < new_Time + ring_Width)
	{
        // ������� ���̴� �� �׸���.
		point_Grey_Scale = (circle_Distance - new_Time) / ring_Width;

		// ���̴� ���� ����� ���� ��� �׸���.
		for (int i = 0; i < 5; ++i)
		{
			vec2 new_Vec = u_Points[i] - new_UV;
			float point_Distance = length(new_Vec);

            // ����Ʈ�� �� ������ �Ÿ��� ����Ʈ�� ������ ���� ������ �׸���. (����, ����Ʈ�� ������ �׷���)
			if (point_Distance < point_Width) point_Grey_Scale += 0.2f;// * pow(1.0f - point_Distance, 2);
		}
	}

	FragColor = vec4(point_Grey_Scale);
}


void Circle()
{
    // 2. ���ɿ� �����
    // "distance * 4.0f * PI" �� ���شٴ°���, sin ��� 2�ֱ⸦ ǥ���ϰ� �ִٴ� ���̴�.
    // ��, ���ɿ��� 2�� �׷�����.

    vec2 new_UV = v_Temp.xy - vec2(0.5f, 0.5f);
    float distance = length(new_UV);
	float grey_scale = sin(distance * 4.0f * PI);

	FragColor = vec4(grey_scale);
}


void Waves()
{
    // 3. ���ṫ�� �����.

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