#version 450

in vec3 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 1);
}







// Shader Language 집합체 타입 연습문제

// mat4 m = mat4(2.0);
// => 2.0으로 각 0x0, 1x1, 2x2, ...의 원소들이 초기화된 4x4 행렬.
// 2.0f 0.0f 0.0f 0.0f
// 0.0f 2.0f 0.0f 0.0f
// 0.0f 0.0f 2.0f 0.0f
// 0.0f 0.0f 0.0f 2.0f

// => 단, 벡터의 경우 vec3(n)은 전체 원소를 n으로 초기화 한다.
// => vec3 white = vec3(1.0f);
// => white == {1.0f, 1.0f, 1.0f}

// vec4 zVec = m[2];
// => m[2]는 3열. {0.0, 0.0f, 2.0f, 0.0f}.

// float yScale = m[1][1];
// => m[1][1]은 2행 2열. yScale = 2.0f.

// ** float이 기본형이고, double로 구성하고 싶다면 d를 접두사로 붙힌다.
// ==> vec2, vec3, mat3, mat4, .... 는 float형을 사용.
// ==> dvec2, dvec3, dmat3, dmat4, .... 는 double형을 사용.
// ** int형은 마찬가지로 'i'를, unsigned int는 'u'를, bool형은 'b'를 접두사로 한다. **
