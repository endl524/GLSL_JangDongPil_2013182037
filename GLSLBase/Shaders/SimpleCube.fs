#version 450

in vec4 v_Color;
in float v_Grey_Scale;
in vec2 v_Texture_UV;
in vec3 v_Normal;
in vec3 v_Pos;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture_Height_Map;
uniform sampler2D u_Texture_Snow;
uniform sampler2D u_Texture_Grass;
uniform sampler2D u_Texture_Height_Map_2;

const vec3 c_Light_1 = vec3(0.0f, 1.0f, 2.0f);
uniform vec3 u_Camera_Position;


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


void Height_Map_Normal()
{
	// 하이트맵에 라이팅 연산 수행.

    vec4 snowColor = texture(u_Texture_Snow, v_Texture_UV * 5.0f) * (v_Grey_Scale);
	vec4 grassColor = texture(u_Texture_Grass, v_Texture_UV * 5.0f) * (1.0f - v_Grey_Scale);
	vec4 newColor = snowColor + grassColor;

	float a = 0.2f;
	float d = 0.2f;
	float s = 1.0f;

	vec3 light_Direction = c_Light_1 - v_Pos;
	
	// ambient 색상 설정.
	vec3 ambient_Color = vec3(0.5f, 0.5f, 0.5f);
	
	// diffuse 계산 및 색상 설정.
	vec3 diffuse_Color = vec3(1.0f, 1.0f, 0.5f);
	float diffuse = clamp(dot(light_Direction, v_Normal), 0.0f, 1.0f);
	
	// specular 계산 및 색상 설정.
	vec3 specular_Color = vec3(1.0f, 1.0f, 1.0f);
	vec3 reflect_Direction = reflect(light_Direction, v_Normal);
	vec3 view_Direction = v_Pos - u_Camera_Position;
	float specular = clamp(dot(view_Direction, reflect_Direction), 0.0f, 1.0f);
	specular = pow(specular, 12.0f);
	
	// 최종 색상 계산.
	vec3 finalColor = ambient_Color * a + diffuse_Color * diffuse * d + specular_Color * specular * s;

	FragColor = vec4((newColor.xyz + finalColor) * 0.5f, 1.0f);
    //FragColor = vec4((v_Normal + finalColor) * 0.5f, 1.0f);
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
	//Height_Map_2();
	Height_Map_Normal();

    //Cube();
}