#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;
uniform float u_Time;

out vec4 FragColor;

const float PI = 3.1416f;

void main()
{
	float new_Time = mod(u_Time, 1.0f);
	vec2 new_Texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);



	// =====================================
	// 1. �����̸� �ݺ��Ǵ� �ؽ��� �׸���
	/*
	new_Texture_UV = fract(new_Texture_UV * 3.0f + vec2(sin(u_Time * PI * 2.0f), cos(u_Time * PI * 2.0f)));

	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================




	// =====================================
	// 2. sin �Լ��� Ȧ�α׷� ó�� �ؽ��� �׸���
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
	// 3. ���� ���� �ſ�� �׸���
	/*
	new_Texture_UV = vec2(0.0f, 0.5f) - abs(v_Texture_UV - vec2(1.0f, 0.5f));
	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 4. Blur Effect �����
	// ���ø��� �������ؼ� ������ ��ѷ��� ��� �� ���� �ش� �����׸�Ʈ�� ������ �д´�.
	/*
	vec2 new_Textures_UV[5];
	new_Textures_UV[0] = vec2(v_Texture_UV.x, v_Texture_UV.y);
	new_Textures_UV[1] = vec2(v_Texture_UV.x - 0.008f, v_Texture_UV.y);
	new_Textures_UV[2] = vec2(v_Texture_UV.x + 0.008f, v_Texture_UV.y);
	new_Textures_UV[3] = vec2(v_Texture_UV.x, v_Texture_UV.y - 0.008f);
	new_Textures_UV[4] = vec2(v_Texture_UV.x, v_Texture_UV.y + 0.008f);
	
	vec4 new_Colors[5];
	vec4 final_Color = vec4(0.0f);
	for (int i = 0; i < 5; ++i)
	{
		new_Colors[i] = texture(u_Texture, new_Textures_UV[i]);
		final_Color += new_Colors[i];
	}
	
	FragColor = final_Color * 0.3f;
	*/
	// =====================================





	// =====================================
	// 5. ��ǥ ���ƺ��� (Simple RGB)
	// x��ǥ�� (0.0f ~ 1.0f) ������ ���� 3�� �ݺ��ȴ�.
	// => (fract ���. 3�� �ݺ��Ǿ� �ϹǷ� 3�� ��������Ѵ�.)
	// y��ǥ�� (0 ~ 1/3), (1/3 ~ 2/3), (2/3 ~ 1) �� ����Ѵ�.
	// => (floor ���. Offset ������ �ʿ��ϴ�. x���� 0~3 ���̷� �������� �̿���, �������� ���ϴ� floor �Լ��� Offset ���� ��´�.)
	/*
	new_Texture_UV.x = fract(v_Texture_UV.x * 3.0f);
	new_Texture_UV.y = floor(v_Texture_UV.x * 3.0f) / 3.0f; // Offset
	new_Texture_UV.y += v_Texture_UV.y / 3.0f;

	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 6. ��ǥ ���ƺ��� 2 (Simple RGB)
	// Offset ���� �����Ѵ�. 2ĭ �������� �о��ָ� �ǹǷ� 2.0f���� ���ش�.
	/*
	new_Texture_UV.x = fract(v_Texture_UV.x * 3.0f);
	new_Texture_UV.y = (2.0f - floor(v_Texture_UV.x * 3.0f)) / 3.0f; // Offset (0, 1, 2, ...)
	new_Texture_UV.y += v_Texture_UV.y / 3.0f;

	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================




	// =====================================
	// 7. ��ǥ ���ƺ��� 3 (Simple RGB)
	/*
	new_Texture_UV.x = v_Texture_UV.x;
	new_Texture_UV.y = (2.0f - floor(v_Texture_UV.y * 3.0f)) / 3.0f; // Offset (2, 1, 0, ...)
	new_Texture_UV.y += fract(v_Texture_UV.y * 3.0f) / 3.0f;
	
	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 8. <����> ��ǥ ���ƺ��� 4 (Simple RGB)
	// ���� ��� �����.

	new_Texture_UV.x = v_Texture_UV.x;
	new_Texture_UV.y = v_Texture_UV.y;

	FragColor = texture(u_Texture, new_Texture_UV);

	// =====================================
}