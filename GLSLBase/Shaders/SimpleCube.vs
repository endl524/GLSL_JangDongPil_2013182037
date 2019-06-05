#version 450 

in vec3 a_Position;
in vec3 a_Normal;
in vec4 a_Color;

out vec4 v_Color;
out float v_Grey_Scale;
out vec2 v_Texture_UV;
out vec3 v_Normal;
out vec3 v_Pos;

uniform float u_Time;
uniform mat4 u_ViewProj_Matrix;
uniform vec2 u_Points[5];
uniform sampler2D u_Texture_Height_Map;
uniform sampler2D u_Texture_Height_Map_2;

const float PI = 3.1416f;

void Flag()
{
	vec3 newPos = a_Position;

	float additional_Value_X = newPos.x + 0.5f; // 0.0f ~ 1.0f
	float additional_Value_Y = newPos.y + 0.5f; // 0.0f ~ 1.0f

	float sin_Period_X = 1.0f + (1.0f - additional_Value_Y) * 0.5f;
	float sin_Period_Y = 1.0f + additional_Value_X * 0.5f;
	
	float value_X = (additional_Value_Y * 2.0f * PI * sin_Period_X) - (u_Time * 10.0f);
	float value_Y = (additional_Value_X * 2.0f * PI * sin_Period_Y) - (u_Time * 10.0f);

	float sin_Value_X = sin(value_X) * 0.08f;
	float sin_Value_Y = sin(value_Y) * 0.2f;

	v_Grey_Scale = sin_Value_Y * additional_Value_X + 0.55f;

	newPos.y = newPos.y * ((1.0f - additional_Value_X) * 0.5f + 0.5f);

	newPos.x = newPos.x - sin_Value_X * additional_Value_X;
	newPos.y = newPos.y + sin_Value_Y * additional_Value_X * v_Grey_Scale;
	newPos.z = v_Grey_Scale - 0.5f;

	gl_Position = u_ViewProj_Matrix * vec4(newPos.xyz, 1);

	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}


void Wave()
{
	float grey = 0.0f;
	float distance = 0.0f;
	
	for(int i = 0; i < 5; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 4.0f * PI * float(i);
		grey += sin(distance - u_Time * 4.0f);
	}

	vec3 new_Pos = a_Position.xyz;
	new_Pos.z = grey / 100.0f;

	v_Grey_Scale = (grey + 1.0f) / 2.0f;

	gl_Position = u_ViewProj_Matrix * vec4(new_Pos.xyz, 1);
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void Sphere_Mapping()
{
	float grey = 0.0f;
	float distance = 0.0f;

	for(int i = 0; i < 5; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 2.0f * PI * float(i);
		grey += sin(distance - u_Time * 4.0f);
	}

	float r = 0.5f + abs(grey) * 0.1f;
	float beta = (a_Position.x + 0.5f) * 4.0f * PI;
	float theta = (a_Position.y + 0.5f) * 2.0f * PI;
	
	vec3 sphere_Pos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);

	//vec3 original_Pos = a_Position.xyz;
	//vec3 new_Pos = mix(original_Pos, sphere_Pos, fract(u_Time));
	
	gl_Position = u_ViewProj_Matrix * vec4(sphere_Pos.xyz, 1.0f);
	v_Grey_Scale = 1.0f;
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}


void Height_Map()
{
	// �ؽ��� ���ø� ��ǥ�� �����̴� �� ������ �ٴ��� �ĵ��� �����̴°� ���� ȿ���� �� �� �ִ�.
	// ù���� vec2 new_UV = a_Position.xy + vec2(0.5f, 0.5f); ����
	// vec2(0.5f + u_Time * 0.1f, 0.5f); ó�� �ð� ���� �����ָ� �ȴ�.

	vec2 new_UV = a_Position.xy + vec2(0.5f, 0.5f);
	float height = texture(u_Texture_Height_Map, new_UV).r; // �ؽ��� ��ǥ�� ���� �ϳ��� ���ø�. (������ Grey scale �̹Ƿ�..)
	vec3 new_Pos = vec3(a_Position.xy, a_Position.z + height * 0.2f);

	gl_Position = u_ViewProj_Matrix * vec4(new_Pos.xyz, 1.0f);
	v_Grey_Scale = height;
	v_Texture_UV = new_UV;
}


void Height_Map_Normal()
{
	// ����Ʈ�ʿ��� Normal �� ���ϱ�.

	// gap == ���� ��ǥ Offset ��
	float gap = 1.0f / 100.0f;

	// 1. ������ UV ��ǥ�� ������ �� UV ��ǥ(���� ��ǥ�� ������ ������ �� ����..)�� �����ش�.
	vec2 new_UV = a_Position.xy + vec2(0.5f, 0.5f);
	vec2 new_UV_Right = a_Position.xy + vec2(0.5f + gap, 0.5f);
	vec2 new_UV_Up = a_Position.xy + vec2(0.5f, 0.5f + gap);

	// 2. �� UV ��ǥ�� ���� �ؽ��� ���ø��� �����Ͽ� ���̰��� ��´�.
	float height = texture(u_Texture_Height_Map, new_UV).r;
	float height_Right = texture(u_Texture_Height_Map, new_UV_Right).r;
	float height_Up = texture(u_Texture_Height_Map, new_UV_Up).r;

	// 3. ���� ���̰��� ���� ��ǥ�� �����Ų��.
	vec3 new_Pos = a_Position + vec3(0.0f, 0.0f, height * 0.2f);
	vec3 new_Pos_Right = a_Position + vec3(gap, 0.0f, height_Right * 0.2f);
	vec3 new_Pos_Up = a_Position + vec3(0.0f, gap, height_Up * 0.2f);

	// 4. �� ���� ��ǥ�� ���� �븻���� ���Ѵ�.
	vec3 diff1 = new_Pos_Right - new_Pos;
	vec3 diff2 = new_Pos_Up - new_Pos;
	vec3 normal = normalize(cross(diff1, diff2));
	
	gl_Position = u_ViewProj_Matrix * vec4(new_Pos.xyz, 1.0f);
	v_Grey_Scale = height;
	v_Texture_UV = new_UV;
	v_Normal = normal;
	v_Pos = new_Pos;
}


void Cube()
{
	gl_Position = u_ViewProj_Matrix * vec4(a_Position, 1.0f);
	
	v_Color = a_Color;
}



void main()
{
	//Flag();
	//Wave();
	//Sphere_Mapping();
	//Height_Map();
	Height_Map_Normal();

	//Cube();
}