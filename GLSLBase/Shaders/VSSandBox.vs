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


	// 가중치.
	float additional_Value_X = newPos.x + 0.5f; // 0.0f ~ 1.0f
	float additional_Value_Y = newPos.y + 0.5f; // 0.0f ~ 1.0f


	// 각 sin 주기를 정한다. (몇 주기로 할지)
	float sin_Period_X = 1.0f + (1.0f - additional_Value_Y) * 0.5f;
	float sin_Period_Y = 1.0f + additional_Value_X * 0.5f;
	

	// sin값 만들 준비.
	// x : (1) -0.5f ~ 0.5f -> + 0.5f ----> (2) 0.0f ~ 1.0f -> * 2.0f * PI ----> (3) 0.0f ~ 2.0f
	float value_X = (additional_Value_Y * 2.0f * PI * sin_Period_X) - (u_Time * 10.0f);
	float value_Y = (additional_Value_X * 2.0f * PI * sin_Period_Y) - (u_Time * 10.0f);


	// sin값을 구한다.
	float sin_Value_X = sin(value_X) * 0.08f;
	float sin_Value_Y = sin(value_Y) * 0.2f;


	// sin값과 가중치를 적용한 높이에 따라 명암을 준다.
	v_Grey_Scale = sin_Value_Y * additional_Value_X + 0.55f;


	// 끝으로 갈수록 'y 길이'가 줄어들게 만든다.
	newPos.y = newPos.y * ((1.0f - additional_Value_X) * 0.5f + 0.5f);


	// 각 sin값과 가중치에 따라 펄럭이는 모션을 만들어준다.
	newPos.x += -sin_Value_X * additional_Value_X;
	newPos.y += sin_Value_Y * additional_Value_X * v_Grey_Scale;
	

	// 결과 전송.
	gl_Position = vec4(newPos.xyz, 1);
	v_Texture_UV = a_Position.xy + vec2(0.5f, 0.5f);
}



void Wave()
{
	//======================================
	// 2. 물결치도록 만들기 (.vs 버전) ★
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
	new_Pos.xy *= 2.0f;

	v_Grey_Scale = (grey + 1.0f) / 2.0f;

	gl_Position = vec4(new_Pos.xyz, 1);
	v_Texture_UV = vec2(0.5f, 0.5f) + a_Position.xy;
}



void Sphere_Mapping()
{
	//======================================
	// 3. '구' 표면으로 만들기. ★
	// ** mix 함수. => 보간을 해준다. (여기서는 Morphing) **
	// => (어떤 값이든 보간이 되지만, 시작값과 끝값의 자료형은 같아야 한다.)
	// => (즉, mix(a, b, c); 에서 a와 b는 자료형이 같아야 하며, c는 보간되는 수치 (0.0 ~ 1.0)이다.)
	// "현재 코드는 꿀렁거리는 젤리"


	float grey = 0.0f;
	float distance = 0.0f;

	// (뭔질 모르겠다..)
	for(int i = 0; i < 2; ++i)
	{
		distance = length(a_Position.xy - u_Points[i]) * 8.0f * PI;
		grey += sin(distance - u_Time);
	}


	// 현재 정점 좌표에 대한 구 표면 각도를 구한다.
	float r = 0.5f + abs(grey) * 0.1f;
	float beta = (a_Position.x + 0.5f) * 2.0f * PI;
	float theta = (a_Position.y + 0.5f) * PI;
	

	// 구 표면의 공식에 따른 새로운 정점 좌표.
	vec3 sphere_Pos = vec3(
		r * sin(theta) * cos(beta),
		r * sin(theta) * sin(beta),
		r * cos(theta)
	);


	// 시간이 지남에 따라 '사각평면'이 '구'로 변하는 좌표를 구해준다.
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