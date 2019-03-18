#version 450

in vec3 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 1);
}







// Shader Language ����ü Ÿ�� ��������

// mat4 m = mat4(2.0);
// => 2.0���� �� 0x0, 1x1, 2x2, ...�� ���ҵ��� �ʱ�ȭ�� 4x4 ���.
// 2.0f 0.0f 0.0f 0.0f
// 0.0f 2.0f 0.0f 0.0f
// 0.0f 0.0f 2.0f 0.0f
// 0.0f 0.0f 0.0f 2.0f

// => ��, ������ ��� vec3(n)�� ��ü ���Ҹ� n���� �ʱ�ȭ �Ѵ�.
// => vec3 white = vec3(1.0f);
// => white == {1.0f, 1.0f, 1.0f}

// vec4 zVec = m[2];
// => m[2]�� 3��. {0.0, 0.0f, 2.0f, 0.0f}.

// float yScale = m[1][1];
// => m[1][1]�� 2�� 2��. yScale = 2.0f.

// ** float�� �⺻���̰�, double�� �����ϰ� �ʹٸ� d�� ���λ�� ������.
// ==> vec2, vec3, mat3, mat4, .... �� float���� ���.
// ==> dvec2, dvec3, dmat3, dmat4, .... �� double���� ���.
// ** int���� ���������� 'i'��, unsigned int�� 'u'��, bool���� 'b'�� ���λ�� �Ѵ�. **
