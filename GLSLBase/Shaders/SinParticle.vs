#version 450

// attribute ==> vertex shader input
in vec3 a_Position;
in vec3 a_Velocity;
in vec2 a_Start_and_Life_Time;
in vec2 a_Ratio_and_Amplitude;
in float a_Value;
in vec4 a_Color;

// varying ==> fragment shader input
out vec4 v_Color;
out vec3 v_Original_Pos;
out float v_Radius;

uniform float u_Time;

const float PI = 3.1416;
const mat3 c_Rotate_Matrix = 
		mat3(0.0f, -1.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,
			 0.0f, 0.0f, 0.0f);

const vec3 c_Gravity = vec3(0.0f, -5.0f, 0.0f);

void main()
{
	// =========== 위치(이동) 작업 ===========
	vec3 new_Pos = a_Position.xyz;
	vec3 new_Vel = a_Velocity.xyz;

	float start_Time = a_Start_and_Life_Time.x;
	float life_Time = a_Start_and_Life_Time.y;
	float ratio = a_Ratio_and_Amplitude.x;
	float amp = a_Ratio_and_Amplitude.y;

	float new_Time = u_Time - start_Time;

	if (new_Time > 0.0f)
	{
		new_Pos.x += sin(a_Value * 2.0f * PI);
		new_Pos.y += cos(a_Value * 2.0f * PI);

		new_Time = mod(new_Time, life_Time);
		new_Vel += c_Gravity * new_Time;
		new_Pos += a_Velocity * new_Time + (0.5f * c_Gravity * new_Time * new_Time);
		//new_Pos += (0.5f * c_Gravity * new_Time * new_Time);
		//vec3 vSin = a_Velocity * c_Rotate_Matrix;
		//new_Pos += vSin * sin(new_Time * 2.0f * PI * ratio) * amp;
	}

	else
	{
		new_Pos = vec3(20000.0f,20000.0f,20000.0f);
	}
	
	gl_Position = vec4(new_Pos.xyz, 1);





	// =========== 색상 작업 ===========
	float remain_Time = life_Time - new_Time;
	float new_Alpha = pow(remain_Time / life_Time, 2);

	v_Color = a_Color;
	v_Color.w = new_Alpha;





	// =========== 모양 작업 ===========
	v_Original_Pos = a_Position.xyz;
	v_Radius = abs(a_Position.x);


}