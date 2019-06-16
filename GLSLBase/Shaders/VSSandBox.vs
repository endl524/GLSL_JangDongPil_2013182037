#version 450 

in vec3 a_Position;

out float v_Grey_Scale;
out vec2 v_Texture_UV;

uniform float u_Time;
uniform vec2 u_Points[5];

const float PI = 3.1416;



// Functions ===============================


void Flag()
{
	//======================================
	// 1. �޷��̴� ��� �ڡ�
	
	vec3 newPos = a_Position;


	// ����ġ.
	float additional_Value_X = newPos.x + 0.5f; // 0.0f ~ 1.0f
	float additional_Value_Y = newPos.y + 0.5f; // 0.0f ~ 1.0f


	// �� sin �ֱ⸦ ���Ѵ�. (�� �ֱ�� ����)
	float sin_Period_X = 1.0f + (1.0f - additional_Value_Y) * 0.5f;
	float sin_Period_Y = 1.0f + additional_Value_X * 0.5f;
	

	// sin�� ���� �غ�.
	// x : (1) -0.5f ~ 0.5f -> + 0.5f ----> (2) 0.0f ~ 1.0f -> * 2.0f * PI ----> (3) 0.0f ~ 2.0f
	float value_X = (additional_Value_Y * 2.0f * PI * sin_Period_X) - (u_Time * 10.0f);
	float value_Y = (additional_Value_X * 2.0f * PI * sin_Period_Y) - (u_Time * 10.0f);


	// sin���� ���Ѵ�.
	float sin_Value_X = sin(value_X) * 0.08f;
	float sin_Value_Y = sin(value_Y) * 0.2f;


	// sin���� ����ġ�� ������ ���̿� ���� ����� �ش�.
	v_Grey_Scale = sin_Value_Y * additional_Value_X + 0.55f;


	// ������ ������ 'y ����'�� �پ��� �����.
	newPos.y = newPos.y * ((1.0f - additional_Value_X) * 0.5f + 0.5f);


	// �� sin���� ����ġ�� ���� �޷��̴� ����� ������ش�.
	newPos.x += -sin_Value_X * additional_Value_X;
	newPos.y += sin_Value_Y * additional_Value_X * v_Grey_Scale;
	

	// ��� ����.
	gl_Position = vec4(newPos.xyz, 1);
	v_Texture_UV = a_Position.xy + vec2(0.5f, 0.5f);
}



void Wave()
{
	//======================================
	// 2. ����ġ���� ����� (.vs ����) ��
	// => (Ư�� ������ �̿��� ����ġ�� �ؽ��� �����)
	
	float grey = 0.0f;
	float distance = 0.0f;
	
	for(int i = 0; i < 5; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 8.0f * PI * float(i);
		grey += sin(distance - u_Time * 4.0f);
	}

	vec3 new_Pos = a_Position.xyz;
	new_Pos.z += grey;
	new_Pos.xy *= 2.0f;

	v_Grey_Scale = (grey + 1.0f) / 2.0f;

	gl_Position = vec4(new_Pos.xyz, 1);
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void Sphere_Mapping()
{
	//======================================
	// 3. '��' ǥ������ �����. ��
	// ** mix �Լ�. => ������ ���ش�. (���⼭�� Morphing) **
	// => (� ���̵� ������ ������, ���۰��� ������ �ڷ����� ���ƾ� �Ѵ�.)
	// => (��, mix(a, b, c); ���� a�� b�� �ڷ����� ���ƾ� �ϸ�, c�� �����Ǵ� ��ġ (0.0 ~ 1.0)�̴�.)
	// "���� �ڵ�� �ܷ��Ÿ��� ����"


	float grey = 0.0f;
	float distance = 0.0f;

	// (���� �𸣰ڴ�..)
	for(int i = 0; i < 2; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 8.0f * PI;
		grey += sin(distance - u_Time);
	}


	// ���� ���� ��ǥ�� ���� �� ǥ�� ������ ���Ѵ�.
	float r = 0.5f + abs(grey) * 0.1f;
	float beta = (a_Position.x + 0.5f) * 2.0f * PI;
	float theta = (a_Position.y + 0.5f) * PI;
	

	// �� ǥ���� ���Ŀ� ���� ���ο� ���� ��ǥ.
	vec3 sphere_Pos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);


	// �ð��� ������ ���� '�簢���'�� '��'�� ���ϴ� ��ǥ�� �����ش�.
	vec3 new_Pos = mix(a_Position.xyz, sphere_Pos, fract(u_Time));
	
	
	gl_Position = vec4(new_Pos.xyz, 1.0f);
	v_Grey_Scale = 1.0f;
	v_Texture_UV = a_Position.xy + vec2(0.5f, 0.5f);
}



void main()
{
	//Flag();
	//Wave();
	Sphere_Mapping();
}