#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture_1;
uniform sampler2D u_Texture_2;
uniform sampler2D u_Texture_3;
uniform sampler2D u_Texture_4;
uniform sampler2D u_Number_Texture;
uniform sampler2D u_Number_Texture_2;
uniform sampler2D u_Texture_1_Reversed;

uniform sampler2D u_Sans_Sprite;
uniform sampler2D u_Runner_Sprite;
uniform sampler2D u_Gunner_Sprite;
uniform sampler2D u_Flame_Sprite;

uniform sampler2D u_Runner_Sprite_Reversed;
uniform float u_X_Index;
uniform float u_Resolution_X;
uniform float u_Resolution_Y;

uniform float u_Time;

uniform int u_Number;
uniform int u_Numbers[3];

out vec4 FragColor;

const float PI = 3.1416f;

void main()
{
	float new_Time = mod(u_Time, 1.0f);
	vec2 new_Texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	vec4 new_Color = vec4(0.0f);


	// =====================================
	// 1. 움직이며 반복되는 텍스쳐 그리기
	/*
	new_Texture_UV = fract(new_Texture_UV * 3.0f + vec2(sin(u_Time * PI * 2.0f), cos(u_Time * PI * 2.0f)));

	FragColor = texture(u_Texture_1, new_Texture_UV);
	*/
	// =====================================




	// =====================================
	// 2. sin 함수로 홀로그램 처럼 텍스쳐 그리기
	/*
	new_Color = vec4(0.0f);
	float new_X_Value = v_Texture_UV.x;
	//new_X_Value += new_Time;

	float sin_value_Y = 0.5f + sin(new_Time * 2.0f * PI) * 0.5f;

	if (v_Texture_UV.y < sin_value_Y + 0.005f && v_Texture_UV.y > sin_value_Y - 0.005f)
	{
		//new_Color = vec4(v_Texture_UV, 0.0f, 1.0f);
		new_Color = texture(u_Texture_1, new_Texture_UV);
	}

	FragColor = new_Color;
	*/
	// =====================================





	// =====================================
	// 3. 상하 반전 거울로 그리기 ★
    // (1) abs(v_Texture_UV - vec2(0.0f, 0.5f))는 V 좌표값이 "0.5f -> 0.0f -> 0.5f" 순서로 찍힌다.
    // (2-1) -abs(v_Texture_UV - vec2(0.0f, 0.5f))는 V 좌표값이 "-0.5f -> 0.0f -> -0.5f" 순서로 찍힌다.
    // (2-2) "-0.5f -> 0.0f -> -0.5f" 는 사실상 "0.5f -> 1.0f -> 0.5f"와 같다.
    // (2-3) 0.5f + "(2-2)의 값" 은 결국 "0.0f -> 0.5f -> 0.0f"와 같다.
    //
    //
    /*
    // (1) 애초에 뒤집힌 텍스쳐 (GLSL에서의 편의성을 위해 텍스쳐를 미리 뒤집어놓은..)
	//new_Texture_UV = vec2(0.0f, 0.5f) + abs(v_Texture_UV - vec2(0.0f, 0.5f));
    //FragColor = texture(u_Texture_1_Reversed, new_Texture_UV);

    // (2) 정상적인 텍스쳐
    new_Texture_UV = vec2(0.0f, 0.5f) - abs(v_Texture_UV - vec2(1.0f, 0.5f));
	FragColor = texture(u_Texture_1, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 4. Blur Effect 만들기 ★★
	// 샘플링을 여러번해서, 현재 fragment의 주위의 색상들을 "모두 더한 결과값"을 최종 색상으로 결정한다.
	/*
    float blurring_Size = 0.007f;
	vec2 new_Textures_UV[5];
	new_Textures_UV[0] = vec2(v_Texture_UV.x, v_Texture_UV.y);
	new_Textures_UV[1] = vec2(v_Texture_UV.x - blurring_Size, v_Texture_UV.y);
	new_Textures_UV[2] = vec2(v_Texture_UV.x + blurring_Size, v_Texture_UV.y);
	new_Textures_UV[3] = vec2(v_Texture_UV.x, v_Texture_UV.y - blurring_Size);
	new_Textures_UV[4] = vec2(v_Texture_UV.x, v_Texture_UV.y + blurring_Size);
	
	vec4 new_Colors[5];
	vec4 final_Color = vec4(0.0f);
	for (int i = 0; i < 5; ++i)
	{
		new_Colors[i] = texture(u_Texture, new_Textures_UV[i]);
		final_Color += new_Colors[i];
	}
	
	FragColor = final_Color * 0.2f;
	*/
	// =====================================





	// =====================================
	// 5. 좌표 꼬아보기 (Simple RGB)
	// <문제> RGB로 3분할 된 텍스쳐를 가로로 3등분 하여,
    //        왼쪽 부터 오른쪽으로 재구성하여 붙인다.
    //        단, 텍스쳐에 써있는 글자 (R, G, B)가 회전되지 않아야 한다.

    // <전략> U 좌표는 모두 "0.0f ~ 1.0f 까지"를 "3번" 사용해서 그려주어야 한다.
    //        V 좌표는 3분할 한 뒤, 원래보다 3배로 늘려서 그려져야 한다.

    // <1st> U 좌표는 fract(원래 u좌표 * 3.0f)로 구해준다. (그릴 범위를 확실히 지정해주는 것이다!)
    //          => 이유는 처음부터 끝까지 (0.0f ~ 1.0f)를 총 3번 그려주어야 하기 때문.
    //          => 그냥 '원래 u좌표'를 대입해버리면 v좌표 뿐만 아니라 u좌표도 0.0f ~ 0.333f, 0.333f ~ 0.666f, 0.666f ~ 1.0f를 따라가게 된다.
    //          => 즉, G는 글자가 보이지만 R, B는 보이지 않게 된다는 것.
    //              => fract(원래 u좌표 * 3.0f)에서 3.0f를 곱해주는 이유는 '원래의 크기'를 "더 좁게", "축약해서" 3번 그려야 하기 때문이다. (텍스쳐 사진을 축소해서 볼 때 처럼)
    //              => 반대로 3.0f를 나눠준다면, "더 넓게", "세밀하게" 그려질것이다. (텍스쳐 사진을 확대해서 볼 때 처럼)

    // <2nd> V 좌표는 U 좌표 처럼 처음부터 끝까지를 반복해서 그리는것이 아니다.
    //          => 단, U 좌표가 0.0f ~ 1.0f 까지 모두 그려졌다면 다음 줄을 그리도록 V좌표를 바꿔줘야한다.
    //              => 즉, floor(v_Texture_UV.x * 3.0f)를 취하여 정수 값만 뽑아내면,
    //              => U 좌표가 0.0f 부터 1.0f 까지 모두 그려질 때마다 그 반환 값이 증가할 것이다.
    //              => (다시말해, 반환값이 0 or 1 or 2 or 3이 된다.)

    // <3rd> <2nd>의 반환값을 원래 V 좌표에 더해주면 적절히 보정된 V 좌표를 얻을 수 있다.
    //          => 단, 한줄에 한가지 색만 보여줘야 하므로 V 좌표를 3배 늘려서 그려줘야한다.

    // <4th> <1st>의 맨 밑줄을 참조하여 <3rd>의 결과 값에 " / 3.0f"를 취해주면, 적절히 늘어난 V 좌표를 얻을 수 있다.
    /*
	new_Texture_UV.x = fract(v_Texture_UV.x * 3.0f);
	new_Texture_UV.y = (v_Texture_UV.y + floor(v_Texture_UV.x * 3.0f)) / 3.0f;

	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 6. 좌표 꼬아보기 2 (Simple RGB)
    // 좌표 꼬아보기 1의 배치 순서를 반대로 하기.
	// Offset 값을 수정한다. 2칸 왼쪽으로 밀어주면 되므로 '2'에서 '해당 줄 수'를 빼준다.
	/*
	new_Texture_UV.x = fract(v_Texture_UV.x * 3.0f);
    new_Texture_UV.y = v_Texture_UV.y / 3.0f;
	new_Texture_UV.y += (2 - floor(v_Texture_UV.x * 3.0f)) / 3.0f; // Offset (0, 1, 2, ...)
	
	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 7. 좌표 꼬아보기 3 (Simple RGB)
    // RGB 순서 바꾸기.
	/*
	new_Texture_UV.x = v_Texture_UV.x;
    new_Texture_UV.y = fract(v_Texture_UV.y * 3.0f) / 3.0f;	
    new_Texture_UV.y += (2 - floor(v_Texture_UV.y * 3.0f)) / 3.0f; // Offset (2, 1, 0, ...)
		
	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================






	// ★★★★★★ 기말 시험 문제 ★★★★★★
	// (그대로 나오진 않음. 변형해서 생각할것.)
	// =====================================
	// 8. 좌표 꼬아보기 4 (Simple RGB)
	// <숙제> 벽돌 모양 만들기 1. (가로)

    // <1st> U 좌표와 V 좌표를 각각 2번씩 그려주도록 한다.
    //          => fract(원래 좌표 * 2.0f)을 취해주면 된다.

    // <2nd> 윗줄의 U 좌표는 U 방향으로 0.5f 만큼 밀어줘야한다.
    //          => 단, 아랫줄은 밀려선 안된다는걸 명심해야한다.
    //          => 때문에, U 좌표에 단순히 "- 0.5f"를 취하는 것이 아닌,
    //          => "- floor(v_Texture_UV.y * 2.0f) * 0.5f" 를 취해주도록 한다.
    //              => 이유는, "V 좌표를 2분할" 해줬었다는 점을 이용할 수 있기 때문이다.
    //              => "2분할된 V 좌표"가 다음줄이 된다면 floor 값은 "0 -> 1"이 될것이다.
    //              => 이전에는 "- 0 * 0.5f"를 취해주고 있었다면, 다음줄이 되면서 "- 1 * 0.5f"를 취하게 된다.
    //              => 따라서 윗줄만 0.5f 만큼 밀리게 된다.
	/*
	new_Texture_UV.x = fract(v_Texture_UV.x * 2.0f) - floor(v_Texture_UV.y * 2.0f) * 0.5f;
	new_Texture_UV.y = fract(v_Texture_UV.y * 2.0f);

	FragColor = texture(u_Texture, new_Texture_UV);
    */
	// =====================================





	// =====================================
	// 9. 좌표 꼬아보기 5 (Simple RGB)
	// 벽돌 모양 만들기 2. (세로)
    /*
	new_Texture_UV.x = fract(v_Texture_UV.x * 2.0f);
	new_Texture_UV.y = fract(v_Texture_UV.y * 2.0f) - floor(v_Texture_UV.x * 2.0f) * 0.5f;

	FragColor = texture(u_Texture, new_Texture_UV);
    */
	// =====================================






    // =====================================
	// 10. 좌표 꼬아보기 6 (Simple RGB)
	// (번외) 벽돌 모양 만들어서 흘러가게 하기. (가로)

    // <문제> 밑에서 부터 위로 세로 줄을 세볼때 (첫번째 줄은 '0' = 짝수)
    //        '홀수번 째'인 줄은 좌측으로, '짝수번 째'인 줄은 우측으로 흘러가도록 한다.

    // <1st> floor(v_Texture_UV.y * 4.0f)로 세로 줄이 몇번째 줄인지 알아낸다. (정수 값)

    // <2nd> mod(<1st>의 결과 값, 2)로 짝수인지 홀수인지 알아낸다. (0 or 1)

    // <3rd> 2.0f * (0.5f - <2nd>의 결과 값)을 하면 방향을 구할 수 있다. (1.0f or -1.0f)

    // <4th> 최종적으로 방향 * 시간 값 (fract되어 0~1사이를 반복하는..) 을 해서 샘플링할 텍스쳐의 u좌표에 더해준다.
    /*
	new_Texture_UV.x = fract(v_Texture_UV.x * 5.0f) - ( (0.5f - mod(floor(v_Texture_UV.y * 5.0f), 2)) * 2.0f * fract(u_Time) );
	new_Texture_UV.y = fract(v_Texture_UV.y * 5.0f);

	FragColor = texture(u_Texture, new_Texture_UV);
    */
	// =====================================







	// =====================================
	// 11. 멀티텍스쳐 다루기.
	// 텍스쳐 두 장을 잘리지 않은 채로 (늘리고/줄여서) 한 면에 다 그리기.
	// 왼쪽에는 1번째 텍스쳐를, 오른쪽에는 2번째 텍스쳐를..
	/*
	new_Color = vec4(0.0f);
	new_Texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	
	if (new_Texture_UV.x < 0.5f) 
	{
		new_Color = texture(u_Texture_1, vec2(new_Texture_UV.x * 2.0f, new_Texture_UV.y));
	}
	else
	{
		new_Color = texture(u_Texture_2, vec2(fract(new_Texture_UV.x * 2.0f), new_Texture_UV.y));
	}
	FragColor = new_Color;
    */
	// =====================================







	// =====================================
	// 12. 멀티텍스쳐 다루기 2.
	// 텍스쳐 네 장을 잘리지 않은 채로 (늘리고/줄여서) 한 면에 다 그리기.
	// 왼쪽 위 부터 0, 1, 2, 3번째 텍스쳐를 그리기.
	// **이 방식은 "캐시미스" 발생.**
	// => ** Texture Switch가 발생하여 효율성 하락. (Cache Miss) **
	/*
	new_Color = vec4(0.0f);
	new_Texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	
	if (new_Texture_UV.x < 0.5f && new_Texture_UV.y < 0.5f) 
	{
		new_Color = texture(u_Texture_1, vec2(new_Texture_UV.x * 2.0f, new_Texture_UV.y * 2.0f));
	}
	else if (new_Texture_UV.x >= 0.5f && new_Texture_UV.y < 0.5f)
	{
		new_Color = texture(u_Texture_2, vec2(fract(new_Texture_UV.x * 2.0f), new_Texture_UV.y * 2.0f));
	}
	else if (new_Texture_UV.x < 0.5f && new_Texture_UV.y >= 0.5f)
	{
		new_Color = texture(u_Texture_3, vec2(new_Texture_UV.x * 2.0f, fract(new_Texture_UV.y * 2.0f)));
	}
	else
	{
		new_Color = texture(u_Texture_4, vec2(fract(new_Texture_UV.x * 2.0f), fract(new_Texture_UV.y * 2.0f)));
	}
	FragColor = new_Color;
    */
	// =====================================








	// =====================================
	// 13. 멀티텍스쳐 다루기 3. (Atlas Texture)
	// 여러개의 텍스쳐를 하나의 텍스쳐에 합쳐놓고 그 텍스쳐의 UV좌표만 가지고 구분하기(다루기).
	// 시간에 따라 다음 텍스쳐를 그려주도록.. (0~9 반복)
	// ** Texture Switch가 발생하지 않아 Cache Miss가 발생하지 않는다! **
	/*
	new_Color = vec4(0.0f);
	new_Texture_UV = vec2(v_Texture_UV.x, (9 - u_Number + v_Texture_UV.y) * 0.1f);
	
	new_Color = texture(u_Number_Texture_2, new_Texture_UV);

	FragColor = new_Color;
	*/
	// =====================================






	// =====================================
	// 14. 멀티텍스쳐 다루기 4.
	// 아틀라스 텍스쳐를 가지고 세개의 텍스쳐를 한번에 그려보기.
	// *** 임의로 시계 처럼 만듦 ***
	// ** Texture Switch가 발생하지 않아 Cache Miss가 발생하지 않는다! **
    /*
	new_Color = vec4(0.0f);
	int index = int(floor(v_Texture_UV.x * 3.0f));
	new_Texture_UV = vec2(fract(v_Texture_UV.x * 3.0f), ( (9 - u_Numbers[index] + v_Texture_UV.y) ) * 0.1f);
	
	new_Color = texture(u_Number_Texture_2, new_Texture_UV);

	FragColor = new_Color;
    */
	// =====================================




	

	// =====================================
	// 15. 멀티텍스쳐 다루기 5.
	// <숙제> Atlas Texture를 이용하여 Sprite Animation 구현하기.
	// => Sprite 텍스쳐를 하나 구해서 애니메이션 구현.
    //
    // (( 현재 "Runner_Sprite" 사용 중 ))
    //
    // => x 기본 좌표 : v_Texture_UV.x / 8.0f;
    // ==> "Runner_Sprite"는 가로로 나열된 "8 frame" 애니메이션이다.
    // ==> (1) 때문에 현재의 U 좌표를 "8"로 나누어 준다.
    //
    //
    // => y 기본 좌표 : (1.0f - v_Texture_UV.y) * 0.5f;
    // ==> "Runner_Sprite"는 세로로 나열된 다른 형태의 애니메이션들도 가지고 있다. (총 2가지)
    // ==> (1) 때문에, 우선 OpenGL 좌표계에 맞게 V 좌표를 뒤집어 준다.
    // ==> (2) 뒤집어준 V 좌표를 "2"로 나누어준다.
    //
    // => 여기서의 기본 좌표들은 굳이 소수점만을 취해줄(fract) 필요가 없다. --> 어차피 0~1 사이의 값이므로..
    // => 만약 "문제 14번" 처럼 U 좌표를 "3분할" 시켜서 사용해야 한다면 fract를 사용해야겠다.
    //
    //
    // => x Offset : mod(floor(u_Time  * animate_Speed), 8) / 8.0f;
    // ==> (1) 8 frame인것을 생각하여 "0~7의 정수"를 뽑아내야 한다.
    // ==> (1-1) 시간에 따라 움직여야 하므로 시간값에 floor를 취하되,
    // ==>      애니메이션 진행 속도를 바꿔줄 수 있도록 속도값도 곱해준다.
    // ==> (1-2) (1-1)의 결과 값을 8로 나눈 나머지값을 구하면 0~7의 정수를 얻을 수 있다.
    // ==> (2) 뽑아낸 0~7의 정수값을 "다시 8로 나눠주면 x의 Offset값"을 얻을 수 있다.
    //
    //
    // => y Offset : mod(floor((u_Time + 1.0f) * 0.5f), 2) * 0.5f;
    // ==> V좌표의 Offset의 최종값은 오른쪽으로 가는 모션은 0.0f, 왼쪽으로 가는 모션은 0.5f 이어야 한다.  
    // ==> (1) 우선 "0 or 1"의 값이 반복되도록 해야 한다.
    // ==> (1-1) 우선 시간에 따라 변하게 되므로, 시간값을 받아온다.
    // ==> (1-2) 해당 애니메이션은 화면 중앙에서 시작해서 오른쪽을 먼저 이동 후 다시 왼쪽으로 이동하기를 반복한다.
    // ==> (1-3) 그런데 중앙에서 시작한다는 것은, 절반만 이동한 후 방향이 뒤집힌다는 말이므로
    // ==>      ** 시간값에 "0.5f"를 더해줘야 한다. **
    // ==> (1-4) ** .vs에서 시간은 "4배 느리게" 흐르므로, 여기서는 시간값을 "2로 나눠줘야" 한다. **
    // ==> (1-5) ** (1-3)과 (1-4)의 식을 합쳐서 (u_Time + 1.0f) * 0.5f 라는 식이 나온다. **
    // ==> (1-6) (1-5)의 식에서 "정수값"만 취한 뒤, "2로 나눈 나머지"를 구하면, 시간에 따라 0과 1을 반복하게되는 값을 얻는다.
    // ==> (2) 시간에 따라 0과 1을 반복하게되는 값을 얻었으므로, 이제 0.5f를 곱해주면된다.
    //
    //
    /*
    float animate_Speed = 15.0f;
    new_Color = vec4(0.0f);

	new_Texture_UV.x = ( v_Texture_UV.x + mod(floor(u_Time * animate_Speed), 8.0f) ) / 8.0f;
    new_Texture_UV.y = ( (1.0f - v_Texture_UV.y) + mod(floor((u_Time + 1.0f) * 0.5f), 2.0f) ) / 2.0f;

    new_Color = texture(u_Runner_Sprite, new_Texture_UV);
	FragColor = new_Color;
    */
	// =====================================




	

    // =====================================
	// 16. 멀티텍스쳐 다루기 6.
    // (번외) Sprite 텍스쳐로 애니메이션 구현 2.
    // => 텍스쳐 뒤집기.
    //
    // ==> (1) 방향 구하기.
    // ==> (1-1) 방향은 좌or우 이므로, 0과 1의 값만 나오는 인덱스를 구해주도록 한다.
    // ==> (1-2) mod(floor((u_Time + 1.0f) * 0.5f), 2)는 "문제 15"의 시간에 따라 0 또는 1의 값을 추출해준다. (반환값이 float 이므로 int로 형변환 해줘야 한다.)
    // ==> (1-3) 미리 정의해둔 "방향 부호 배열"에 구한 인덱스를 넣어주어 방향에 대한 부호를 구한다. (-1 or 1)
    // ==> (1-4) (dir_index - dir * v_Texture_UV.x + 6.0f) / 9.0f 는 "Gunner_Sprite"에서 이동 모션의 시작부를 가리킨다.
    // ==>      오른쪽으로 이동하는 모션이 Default 이므로, (0 - (-1 * U 좌표) + 6.0f) / 9.0f 가 시작부.
    // ==>      왼쪽으로 이동하는 모션은 위의 좌표를 뒤집어 줘야 한다.
    // ==>      때문에 단순히 (-U 좌표 + 6.0f) / 9.0f 가 아니라 (1.0f - U 좌표 + 6.0f) / 9.0f 가 시작부가 된다.
    //
    //
    // ==> (2) 오프셋 구하기.
    // ==> (2-1) "Gunner_Sprite"의 이동 모션은 "3 frame" 이므로 mod(floor(u_Time * animate_Speed), 3) / 9.0f를 더해준다.
    //
    //
    /*
    float animate_Speed = 11.0f;
    new_Color = vec4(0.0f);
    
    int dir_index = int(mod(floor((u_Time + 1.0f) * 0.5f), 2));
    int dir_container[2] = {-1, 1};
    int dir = dir_container[dir_index];
	new_Texture_UV = vec2( ( (dir_index - dir * v_Texture_UV.x + 6.0f) + mod(floor(u_Time * animate_Speed), 3) ) / 9.0f , 
                            (1.0f - v_Texture_UV.y) / 6.0f);

    new_Color = texture(u_Gunner_Sprite, new_Texture_UV);
	FragColor = new_Color;
    */
    // =====================================
	



	

    // =====================================
	// 17. 멀티텍스쳐 다루기 7.
	// => Sprite 텍스쳐로 애니메이션 구현 3. (실습)
	
    new_Color = vec4(0.0f);

	float x_index = floor( fract( u_X_Index / u_Resolution_X ) * u_Resolution_X );
	float y_index = floor( floor( u_X_Index / u_Resolution_X ) );

	new_Texture_UV.x = ( v_Texture_UV.x + x_index ) / u_Resolution_X;
    new_Texture_UV.y = ( v_Texture_UV.y + y_index ) / u_Resolution_Y;

    new_Color = texture(u_Flame_Sprite, new_Texture_UV);
	FragColor = new_Color;
	
	// =====================================
}