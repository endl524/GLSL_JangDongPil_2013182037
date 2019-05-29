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
	// 1. 펄럭이는 깃발 ★★
	
	vec3 newPos = a_Position;

	float additional_Value_X = newPos.x + 0.5f; // 0.0f ~ 1.0f
	float additional_Value_Y = newPos.y + 0.5f; // 0.0f ~ 1.0f

	float sin_Period_X = 1.0f + (1.0f - additional_Value_Y) * 0.5f;
	float sin_Period_Y = 1.0f + additional_Value_X * 0.5f;
	
	// x : (1) -0.5f ~ 0.5f -> + 0.5f ----> (2) 0.0f ~ 1.0f -> * 2.0f * PI ----> (3) 0.0f ~ 2.0f
	float value_X = (additional_Value_Y * 2.0f * PI * sin_Period_X) - (u_Time * 10.0f);
	float value_Y = (additional_Value_X * 2.0f * PI * sin_Period_Y) - (u_Time * 10.0f);

	float sin_Value_X = sin(value_X) * 0.08f;
	float sin_Value_Y = sin(value_Y) * 0.2f;

	v_Grey_Scale = sin_Value_Y * additional_Value_X + 0.55f;

	newPos.y = newPos.y * ((1.0f - additional_Value_X) * 0.5f + 0.5f);

	newPos.x = newPos.x - sin_Value_X * additional_Value_X;
	newPos.y = newPos.y + sin_Value_Y * additional_Value_X * v_Grey_Scale;
	
	gl_Position = vec4(newPos.xyz, 1);

	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void Wave()
{
	//======================================
	// 2. 퍼져나가는 동심원 만들기 (.vs 버전) ★
	// => (특정 지점을 이용해 물결치는 텍스쳐 만들기)
	
	float grey = 0.0f;
	float distance = 0.0f;
	
	for(int i = 0; i < 5; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 8.0f * PI * float(i);
		grey += sin(distance - u_Time * 4.0f);
	}

	vec3 new_Pos = a_Position.xyz;
	new_Pos.z += grey;

	v_Grey_Scale = (grey + 1.0f) / 2.0f;

	gl_Position = vec4(new_Pos.xyz, 1);
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void Sphere_Mapping()
{
	//======================================
	// 3. '구' 표면으로 만들기. ★
	// ** mix 함수. => 보간을 해준다. **
	// => (어떤 값이든 보간이 되지만, 시작값과 끝값의 자료형은 같아야 한다.)
	// => (즉, mix(a, b, c); 에서 a와 b는 자료형이 같아야 하며, c는 보간되는 수치 (0.0 ~ 1.0)이다.)
	// "현재 코드는 꿀렁거리는 젤리"

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
	
	gl_Position = vec4(sphere_Pos.xyz, 1.0f);
	v_Grey_Scale = 1.0f;
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void main()
{
	//Flag();
	//Wave();
	Sphere_Mapping();
}