#version 450

in vec4 v_Color;
in vec2 v_Original_Pos;
in float v_Radius;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
	/*
	vec4 new_Color;
	float distance_of_Curr_Fragment_from_Original_Pos = sqrt(v_Original_Pos.x * v_Original_Pos.x + v_Original_Pos.y * v_Original_Pos.y);
	
	if (distance_of_Curr_Fragment_from_Original_Pos < v_Radius)
	{
		new_Color = v_Color;
		new_Color.a = pow(1 - distance_of_Curr_Fragment_from_Original_Pos / v_Radius, 7);
	}
	else
	{
		new_Color = vec4(0.0f);
	}
	
	FragColor = new_Color;
	*/

	vec2 new_Pos;
	new_Pos.x = 0.5f + v_Original_Pos.x;
	new_Pos.y = 0.5f - v_Original_Pos.y;
	FragColor = texture(u_Texture, new_Pos / v_Radius * 0.5f);
}