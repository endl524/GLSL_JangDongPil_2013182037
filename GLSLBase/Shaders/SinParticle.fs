#version 450

in vec4 v_Color;
in vec2 v_Fragment_Pos;
in float v_Radius;
in vec2 v_Texture_UV;

uniform sampler2D u_Texture;

out vec4 FragColor;

void main()
{
	/*
	vec4 new_Color;
	float distance = length(v_Fragment_Pos);
	
	if (distance < v_Radius)
	{
		new_Color.rgb = v_Color.rgb;
		new_Color.a = pow(1 - distance / v_Radius, 7);
	}
	else new_Color = vec4(0.0f);
	
	FragColor = new_Color;
	*/

	FragColor = texture(u_Texture, v_Texture_UV) * 20.0f; // texture sampling 후, HDR Color로 만든다.
	//FragColor = vec4(5.0f); // HDR Color
	FragColor.a = 1.0f;
}