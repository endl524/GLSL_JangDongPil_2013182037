#version 450

in float v_Gray_Scale;
in vec2 v_Texture_UV;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
	//======================================
	// 1. �޷��̴� ��� ��
	// �ؽ��� �� �׷��� ������ ����.

	vec2 texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = texture(u_Texture, texture_UV);

	FragColor = vec4(new_Color.xyz * v_Gray_Scale, 1.0f);

	//======================================
}