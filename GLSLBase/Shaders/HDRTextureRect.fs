#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;
uniform float u_Blur_Size;
uniform vec2 u_Texel_Size;

out vec4 FragColor;

void Bloom()
{
	// Average Blur
	float size = u_Blur_Size / 2.0f;
	vec2 x_Dir = vec2(1.0f, 0.0f);
	vec2 y_Dir = vec2(0.0f, 1.0f);
	vec3 new_Color = vec3(0.0f, 0.0f, 0.0f);
	float count = 0.0f;
	float max_Distance = length(size * x_Dir * u_Texel_Size);

	for (float x = -size; x < size; x += 1.0f)
	{
		for (float y = -size; y < size; y += 1.0f)
		{
			vec2 new_Tex = v_Texture_UV + x * x_Dir * u_Texel_Size + y * y_Dir * u_Texel_Size;

			// Bloom ȿ���� �ձ۰� ������ش�.
			float distance = length(new_Tex - v_Texture_UV);
			float add = clamp(1.0f - (distance / max_Distance), 0.0f, 1.0f);
			add = pow(add, 5); // ������ �� Ű���ش�.

			// (1) clamp�� ����ؼ� ������ 1.0f ���� ���� �ȼ��� Bloom ó���� ���� ���� �� �ִ�.
			// �ٽø��ؼ�, ��ƼŬ ó�� Ư���� ��츦 ���� �ϰ�� Bloom ȿ���� �����Ű�� �ʵ��� ���� �� �ִ�.
			vec4 sample_color = texture(u_Texture, new_Tex);
			sample_color = clamp(sample_color - vec4(1.0f), 0.0f, 100.0f);
			new_Color += sample_color.rgb * add;

			// (2) �Ʒ� ó�� Clamp�� ���� ������, ��� FrameBuffer Texture�� ���빰�� Bloom ��Ų��.
			//new_Color += texture(u_Texture, new_Tex).rgb;

			count += 0.1f;
		}
	}

	FragColor = vec4((new_Color.xyz / count) + texture(u_Texture, v_Texture_UV).rgb, 1.0f);
}


void main()
{
	Bloom();
}