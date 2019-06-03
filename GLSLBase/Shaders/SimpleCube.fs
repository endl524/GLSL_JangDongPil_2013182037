#version 450

in vec4 v_Color;

out vec4 FragColor;



//VSSandBox 예제용========
in float v_Grey_Scale;
in vec2 v_Texture_UV;
uniform sampler2D u_Texture;
uniform sampler2D u_Texture_Height_Map;
uniform sampler2D u_Texture_Snow;
uniform sampler2D u_Texture_Grass;
//=======================

void Flag()
{
	vec2 texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = texture(u_Texture, texture_UV);

	FragColor = vec4(new_Color.xyz * v_Grey_Scale, 1.0f);
}

void Wave()
{
    vec2 texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = vec4(1.0f);

	FragColor = vec4(new_Color.xyz * v_Grey_Scale, 1.0f);
}

void Sphere_Mapping()
{
    vec2 texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = vec4(1.0f);

	FragColor = vec4(new_Color.xyz * v_Grey_Scale, 1.0f);
}

void Height_Map()
{
	FragColor = vec4(v_Grey_Scale);
}


void Height_Map_2()
{
	// 하이트맵의 고도에 따른 텍스쳐 샘플링 변경.
	// 샘플링을 10번 반복하고 있다. (v_Texture_UV * 5.0f)
	// => 고도의 변화에 따라 늘어나는 텍스쳐를 좀 더 조밀하게 그리기 위함.

	vec4 snowColor = texture(u_Texture_Snow, v_Texture_UV * 5.0f) * (v_Grey_Scale);
	vec4 grassColor = texture(u_Texture_Grass, v_Texture_UV * 5.0f) * (1.0f - v_Grey_Scale);
	vec4 finalColor = snowColor + grassColor;

	FragColor = finalColor;
}

void Cube()
{
    FragColor = v_Color;
}

void main()
{
    //Flag();
    //Wave();
    //Sphere_Mapping();
    //Height_Map();
	Height_Map_2();

	//Cube();
}