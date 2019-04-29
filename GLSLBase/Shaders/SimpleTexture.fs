#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;
uniform float u_Time;

out vec4 FragColor;

const float PI = 3.1416f;

void main()
{
	float new_Time = mod(u_Time, 1.0f);
	vec2 new_Texture_UV = vec2(v_Texture_UV.x, 1 - v_Texture_UV.y);



	// =====================================
	// 1. 움직이며 반복되는 텍스쳐 그리기
	/*
	new_Texture_UV = fract(new_Texture_UV * 3.0f + vec2(sin(u_Time * PI * 2.0f), cos(u_Time * PI * 2.0f)));

	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================




	// =====================================
	// 2. sin 함수로 홀로그램 처럼 텍스쳐 그리기
	/*
	vec4 new_Color = vec4(0.0f);
	float new_X_Value = v_Texture_UV.x;
	//new_X_Value += new_Time;

	float sin_value_Y = 0.5f + sin(new_Time * 2.0f * PI) * 0.5f;

	if (v_Texture_UV.y < sin_value_Y + 0.005f && v_Texture_UV.y > sin_value_Y - 0.005f)
	{
		//new_Color = vec4(v_Texture_UV, 0.0f, 1.0f);
		new_Color = texture(u_Texture, new_Texture_UV);
	}

	FragColor = new_Color;
	*/
	// =====================================




	// =====================================
	// 3. 상하 반전 거울로 그리기
	/*
	if (v_Texture_UV.y < 0.5f)
	{
		new_Texture_UV.y = v_Texture_UV.y;
	}
	*/
	new_Texture_UV = abs(v_Texture_UV - vec2(0.0f, 0.5f)) + vec2(0.0f, 0.5f);
	FragColor = texture(u_Texture, new_Texture_UV);

	// =====================================
}