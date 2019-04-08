#version 450

in vec4 v_Color;
in vec3 v_Original_Pos;
in float v_Radius;

out vec4 FragColor;

void main()
{
	vec4 new_Color;
	float distance = sqrt(v_Original_Pos.x * v_Original_Pos.x + v_Original_Pos.y * v_Original_Pos.y);

	if (distance < v_Radius)
	{
		new_Color = v_Color;
		new_Color.a = pow(1 - distance / v_Radius, 7);
	}
	else
	{
		new_Color = vec4(0.0f);
	}

	FragColor = new_Color;
}