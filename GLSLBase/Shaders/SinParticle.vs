#version 450

in vec3 a_Position;
in vec2 a_Emit_and_Life_Time;

uniform float u_Time;
uniform bool u_Repeat = true;

const float PI = 3.1416;

void main()
{
	vec3 new_Pos = a_Position.xyz;
	float start_Time = a_Emit_and_Life_Time.x;
	float life_Time = a_Emit_and_Life_Time.y;
	//float new_Time = fract(u_Time);

	float new_Time = u_Time - start_Time;

	if (new_Time >= 0.0f)
	{
		float remaining_Life = life_Time - new_Time;

		if (u_Repeat)
		{
			remaining_Life = 1.0f;
			new_Time = mod(new_Time, life_Time);
		}

		if (remaining_Life < 0.0f)
		{
			new_Pos = vec3(20000.0f,20000.0f,20000.0f);
		}
		else
		{
			new_Pos.x += (new_Time * 0.5f);
			new_Pos.y += (sin(new_Time * PI * 2.0f) * 0.5f);
		}
	}
	else
	{
		new_Pos = vec3(20000.0f,20000.0f,20000.0f);
	}

	

	gl_Position = vec4(new_Pos.xyz, 1);
}