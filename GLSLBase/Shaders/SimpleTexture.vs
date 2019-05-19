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
	// =========== 위치(이동) 작업 ===========
	
	// ** "Sin 주기"와 "시간 값"에 따라 좌우로 이동하도록 한다. **
	vec4 new_Position = vec4(0.0f);
	new_Position.x = a_Position.x * 0.5f + sin(fract(u_Time * 0.25f) * PI * 2.0f) ;
	new_Position.y = a_Position.y * 0.5f;
	gl_Position = vec4(new_Position.xyz, 1);
	

	// ** Default Location. **
	//gl_Position = vec4(a_Position.xyz, 1);




	// =========== 모양 작업 ===========
	v_Texture_UV = a_Texture_UV.xy;
}