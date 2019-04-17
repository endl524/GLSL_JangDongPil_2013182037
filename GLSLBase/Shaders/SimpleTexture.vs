#version 450

// attribute ==> vertex shader input
in vec3 a_Position;
in vec2 a_Texture_UV;

// varying ==> fragment shader input
out vec2 v_Texture_UV;

void main()
{
	// =========== ��ġ(�̵�) �۾� ===========
	gl_Position = vec4(a_Position.xyz, 1);


	// =========== ��� �۾� ===========
	v_Texture_UV = a_Texture_UV.xy;
}