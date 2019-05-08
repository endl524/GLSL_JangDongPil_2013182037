#version 450

in vec2 v_Texture_UV;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture_2;
uniform sampler2D u_Texture_3;
uniform sampler2D u_Texture_4;
uniform float u_Time;

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

	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================




	// =====================================
	// 2. sin 함수로 홀로그램 처럼 텍스쳐 그리기
	/*
	vec4 new_Color = vec4(0.0f);
	float new_X_Value = v_Texture_UV.x;
	//new_X_Value += new_Time;

	float sin_value_Y = 0.5f + sin(new_Time * 2.0f * PI) * 0.5f;

	if (v_Texture_UV.y < sin_value_Y + 0.005f && v_Texture_UV.y > sin_value_Y - 0.005f)
	{
		//new_Color = vec4(v_Texture_UV, 0.0f, 1.0f);
		new_Color = texture(u_Texture, new_Texture_UV);
	}

	FragColor = new_Color;
	*/
	// =====================================




	// =====================================
	// 3. 상하 반전 거울로 그리기
	/*
	new_Texture_UV = vec2(0.0f, 0.5f) - abs(v_Texture_UV - vec2(1.0f, 0.5f));
	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 4. Blur Effect 만들기
	// 샘플링을 여러번해서 주위로 흩뿌려준 결과 중 현재 해당 프래그먼트의 색상을 읽는다.
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
	// 10. 좌표 꼬아보기 번외 1 (Simple RGB)
	// 벽돌 모양 만들어서 흘러가게 하기. (가로)

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
		new_Color = texture(u_Texture, vec2(new_Texture_UV.x * 2.0f, new_Texture_UV.y));
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

	new_Color = vec4(0.0f);
	new_Texture_UV = vec2(v_Texture_UV.x, 1.0f - v_Texture_UV.y);
	
	if (new_Texture_UV.x < 0.5f && new_Texture_UV.y < 0.5f) 
	{
		new_Color = texture(u_Texture, vec2(new_Texture_UV.x * 2.0f, new_Texture_UV.y * 2.0f));
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
    
	// =====================================
}