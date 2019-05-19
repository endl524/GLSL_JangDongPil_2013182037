#version 450

// attribute ==> vertex shader input
in vec3 a_Position;
in vec2 a_Texture_UV;

// varying ==> fragment shader input
out vec2 v_Texture_UV;

uniform float u_Time;

const float PI = 3.1416f;

void main()
{
	// =========== ��ġ(�̵�) �۾� ===========
	
	// ** "Sin �ֱ�"�� "�ð� ��"�� ���� �¿�� �̵��ϵ��� �Ѵ�. **
	vec4 new_Position = vec4(0.0f);
	new_Position.x = a_Position.x * 0.5f + sin(fract(u_Time * 0.25f) * PI * 2.0f) ;
	new_Position.y = a_Position.y * 0.5f;
	gl_Position = vec4(new_Position.xyz, 1);
	

	// ** Default Location. **
	//gl_Position = vec4(a_Position.xyz, 1);




	// =========== ��� �۾� ===========
	v_Texture_UV = a_Texture_UV.xy;
}