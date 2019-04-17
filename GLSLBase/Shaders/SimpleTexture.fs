#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;
uniform float u_Time;

out vec4 FragColor;

const float PI = 3.1416;

void main()
{
	vec4 new_Color = vec4(0.0f);

	float new_Time = mod(u_Time, 1.0f);
	float new_X_Value = v_Texture_UV.x;
	new_X_Value += new_Time;

	float sin_value_Y = 0.5f + sin(new_X_Value * 2.0f * PI) * 0.5f;

	if (v_Texture_UV.y < sin_value_Y + 0.005f && v_Texture_UV.y > sin_value_Y - 0.005f)
	{
		new_Color = vec4(v_Texture_UV, 0.0f, 1.0f);
	}

	FragColor = new_Color;
}