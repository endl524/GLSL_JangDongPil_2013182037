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
	// 1. �����̸� �ݺ��Ǵ� �ؽ��� �׸���
	/*
	new_Texture_UV = fract(new_Texture_UV * 3.0f + vec2(sin(u_Time * PI * 2.0f), cos(u_Time * PI * 2.0f)));

	FragColor = texture(u_Texture_1, new_Texture_UV);
	*/
	// =====================================




	// =====================================
	// 2. sin �Լ��� Ȧ�α׷� ó�� �ؽ��� �׸���
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
	// 3. ���� ���� �ſ�� �׸��� ��
    // (1) abs(v_Texture_UV - vec2(0.0f, 0.5f))�� V ��ǥ���� "0.5f -> 0.0f -> 0.5f" ������ ������.
    // (2-1) -abs(v_Texture_UV - vec2(0.0f, 0.5f))�� V ��ǥ���� "-0.5f -> 0.0f -> -0.5f" ������ ������.
    // (2-2) "-0.5f -> 0.0f -> -0.5f" �� ��ǻ� "0.5f -> 1.0f -> 0.5f"�� ����.
    // (2-3) 0.5f + "(2-2)�� ��" �� �ᱹ "0.0f -> 0.5f -> 0.0f"�� ����.
    //
    //
    /*
    // (1) ���ʿ� ������ �ؽ��� (GLSL������ ���Ǽ��� ���� �ؽ��ĸ� �̸� ���������..)
	//new_Texture_UV = vec2(0.0f, 0.5f) + abs(v_Texture_UV - vec2(0.0f, 0.5f));
    //FragColor = texture(u_Texture_1_Reversed, new_Texture_UV);

    // (2) �������� �ؽ���
    new_Texture_UV = vec2(0.0f, 0.5f) - abs(v_Texture_UV - vec2(1.0f, 0.5f));
	FragColor = texture(u_Texture_1, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 4. Blur Effect ����� �ڡ�
	// ���ø��� �������ؼ�, ���� fragment�� ������ ������� "��� ���� �����"�� ���� �������� �����Ѵ�.
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
	// 5. ��ǥ ���ƺ��� (Simple RGB)
	// <����> RGB�� 3���� �� �ؽ��ĸ� ���η� 3��� �Ͽ�,
    //        ���� ���� ���������� �籸���Ͽ� ���δ�.
    //        ��, �ؽ��Ŀ� ���ִ� ���� (R, G, B)�� ȸ������ �ʾƾ� �Ѵ�.

    // <����> U ��ǥ�� ��� "0.0f ~ 1.0f ����"�� "3��" ����ؼ� �׷��־�� �Ѵ�.
    //        V ��ǥ�� 3���� �� ��, �������� 3��� �÷��� �׷����� �Ѵ�.

    // <1st> U ��ǥ�� fract(���� u��ǥ * 3.0f)�� �����ش�. (�׸� ������ Ȯ���� �������ִ� ���̴�!)
    //          => ������ ó������ ������ (0.0f ~ 1.0f)�� �� 3�� �׷��־�� �ϱ� ����.
    //          => �׳� '���� u��ǥ'�� �����ع����� v��ǥ �Ӹ� �ƴ϶� u��ǥ�� 0.0f ~ 0.333f, 0.333f ~ 0.666f, 0.666f ~ 1.0f�� ���󰡰� �ȴ�.
    //          => ��, G�� ���ڰ� �������� R, B�� ������ �ʰ� �ȴٴ� ��.
    //              => fract(���� u��ǥ * 3.0f)���� 3.0f�� �����ִ� ������ '������ ũ��'�� "�� ����", "����ؼ�" 3�� �׷��� �ϱ� �����̴�. (�ؽ��� ������ ����ؼ� �� �� ó��)
    //              => �ݴ�� 3.0f�� �����شٸ�, "�� �а�", "�����ϰ�" �׷������̴�. (�ؽ��� ������ Ȯ���ؼ� �� �� ó��)

    // <2nd> V ��ǥ�� U ��ǥ ó�� ó������ �������� �ݺ��ؼ� �׸��°��� �ƴϴ�.
    //          => ��, U ��ǥ�� 0.0f ~ 1.0f ���� ��� �׷����ٸ� ���� ���� �׸����� V��ǥ�� �ٲ�����Ѵ�.
    //              => ��, floor(v_Texture_UV.x * 3.0f)�� ���Ͽ� ���� ���� �̾Ƴ���,
    //              => U ��ǥ�� 0.0f ���� 1.0f ���� ��� �׷��� ������ �� ��ȯ ���� ������ ���̴�.
    //              => (�ٽø���, ��ȯ���� 0 or 1 or 2 or 3�� �ȴ�.)

    // <3rd> <2nd>�� ��ȯ���� ���� V ��ǥ�� �����ָ� ������ ������ V ��ǥ�� ���� �� �ִ�.
    //          => ��, ���ٿ� �Ѱ��� ���� ������� �ϹǷ� V ��ǥ�� 3�� �÷��� �׷�����Ѵ�.

    // <4th> <1st>�� �� ������ �����Ͽ� <3rd>�� ��� ���� " / 3.0f"�� �����ָ�, ������ �þ V ��ǥ�� ���� �� �ִ�.
    /*
	new_Texture_UV.x = fract(v_Texture_UV.x * 3.0f);
	new_Texture_UV.y = (v_Texture_UV.y + floor(v_Texture_UV.x * 3.0f)) / 3.0f;

	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 6. ��ǥ ���ƺ��� 2 (Simple RGB)
    // ��ǥ ���ƺ��� 1�� ��ġ ������ �ݴ�� �ϱ�.
	// Offset ���� �����Ѵ�. 2ĭ �������� �о��ָ� �ǹǷ� '2'���� '�ش� �� ��'�� ���ش�.
	/*
	new_Texture_UV.x = fract(v_Texture_UV.x * 3.0f);
    new_Texture_UV.y = v_Texture_UV.y / 3.0f;
	new_Texture_UV.y += (2 - floor(v_Texture_UV.x * 3.0f)) / 3.0f; // Offset (0, 1, 2, ...)
	
	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================





	// =====================================
	// 7. ��ǥ ���ƺ��� 3 (Simple RGB)
    // RGB ���� �ٲٱ�.
	/*
	new_Texture_UV.x = v_Texture_UV.x;
    new_Texture_UV.y = fract(v_Texture_UV.y * 3.0f) / 3.0f;	
    new_Texture_UV.y += (2 - floor(v_Texture_UV.y * 3.0f)) / 3.0f; // Offset (2, 1, 0, ...)
		
	FragColor = texture(u_Texture, new_Texture_UV);
	*/
	// =====================================






	// �ڡڡڡڡڡ� �⸻ ���� ���� �ڡڡڡڡڡ�
	// (�״�� ������ ����. �����ؼ� �����Ұ�.)
	// =====================================
	// 8. ��ǥ ���ƺ��� 4 (Simple RGB)
	// <����> ���� ��� ����� 1. (����)

    // <1st> U ��ǥ�� V ��ǥ�� ���� 2���� �׷��ֵ��� �Ѵ�.
    //          => fract(���� ��ǥ * 2.0f)�� �����ָ� �ȴ�.

    // <2nd> ������ U ��ǥ�� U �������� 0.5f ��ŭ �о�����Ѵ�.
    //          => ��, �Ʒ����� �з��� �ȵȴٴ°� ����ؾ��Ѵ�.
    //          => ������, U ��ǥ�� �ܼ��� "- 0.5f"�� ���ϴ� ���� �ƴ�,
    //          => "- floor(v_Texture_UV.y * 2.0f) * 0.5f" �� �����ֵ��� �Ѵ�.
    //              => ������, "V ��ǥ�� 2����" ������ٴ� ���� �̿��� �� �ֱ� �����̴�.
    //              => "2���ҵ� V ��ǥ"�� �������� �ȴٸ� floor ���� "0 -> 1"�� �ɰ��̴�.
    //              => �������� "- 0 * 0.5f"�� �����ְ� �־��ٸ�, �������� �Ǹ鼭 "- 1 * 0.5f"�� ���ϰ� �ȴ�.
    //              => ���� ���ٸ� 0.5f ��ŭ �и��� �ȴ�.
	/*
	new_Texture_UV.x = fract(v_Texture_UV.x * 2.0f) - floor(v_Texture_UV.y * 2.0f) * 0.5f;
	new_Texture_UV.y = fract(v_Texture_UV.y * 2.0f);

	FragColor = texture(u_Texture, new_Texture_UV);
    */
	// =====================================





	// =====================================
	// 9. ��ǥ ���ƺ��� 5 (Simple RGB)
	// ���� ��� ����� 2. (����)
    /*
	new_Texture_UV.x = fract(v_Texture_UV.x * 2.0f);
	new_Texture_UV.y = fract(v_Texture_UV.y * 2.0f) - floor(v_Texture_UV.x * 2.0f) * 0.5f;

	FragColor = texture(u_Texture, new_Texture_UV);
    */
	// =====================================






    // =====================================
	// 10. ��ǥ ���ƺ��� 6 (Simple RGB)
	// (����) ���� ��� ���� �귯���� �ϱ�. (����)

    // <����> �ؿ��� ���� ���� ���� ���� ������ (ù��° ���� '0' = ¦��)
    //        'Ȧ���� °'�� ���� ��������, '¦���� °'�� ���� �������� �귯������ �Ѵ�.

    // <1st> floor(v_Texture_UV.y * 4.0f)�� ���� ���� ���° ������ �˾Ƴ���. (���� ��)

    // <2nd> mod(<1st>�� ��� ��, 2)�� ¦������ Ȧ������ �˾Ƴ���. (0 or 1)

    // <3rd> 2.0f * (0.5f - <2nd>�� ��� ��)�� �ϸ� ������ ���� �� �ִ�. (1.0f or -1.0f)

    // <4th> ���������� ���� * �ð� �� (fract�Ǿ� 0~1���̸� �ݺ��ϴ�..) �� �ؼ� ���ø��� �ؽ����� u��ǥ�� �����ش�.
    /*
	new_Texture_UV.x = fract(v_Texture_UV.x * 5.0f) - ( (0.5f - mod(floor(v_Texture_UV.y * 5.0f), 2)) * 2.0f * fract(u_Time) );
	new_Texture_UV.y = fract(v_Texture_UV.y * 5.0f);

	FragColor = texture(u_Texture, new_Texture_UV);
    */
	// =====================================







	// =====================================
	// 11. ��Ƽ�ؽ��� �ٷ��.
	// �ؽ��� �� ���� �߸��� ���� ä�� (�ø���/�ٿ���) �� �鿡 �� �׸���.
	// ���ʿ��� 1��° �ؽ��ĸ�, �����ʿ��� 2��° �ؽ��ĸ�..
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
	// 12. ��Ƽ�ؽ��� �ٷ�� 2.
	// �ؽ��� �� ���� �߸��� ���� ä�� (�ø���/�ٿ���) �� �鿡 �� �׸���.
	// ���� �� ���� 0, 1, 2, 3��° �ؽ��ĸ� �׸���.
	// **�� ����� "ĳ�ù̽�" �߻�.**
	// => ** Texture Switch�� �߻��Ͽ� ȿ���� �϶�. (Cache Miss) **
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
	// 13. ��Ƽ�ؽ��� �ٷ�� 3. (Atlas Texture)
	// �������� �ؽ��ĸ� �ϳ��� �ؽ��Ŀ� ���ĳ��� �� �ؽ����� UV��ǥ�� ������ �����ϱ�(�ٷ��).
	// �ð��� ���� ���� �ؽ��ĸ� �׷��ֵ���.. (0~9 �ݺ�)
	// ** Texture Switch�� �߻����� �ʾ� Cache Miss�� �߻����� �ʴ´�! **
	/*
	new_Color = vec4(0.0f);
	new_Texture_UV = vec2(v_Texture_UV.x, (9 - u_Number + v_Texture_UV.y) * 0.1f);
	
	new_Color = texture(u_Number_Texture_2, new_Texture_UV);

	FragColor = new_Color;
	*/
	// =====================================






	// =====================================
	// 14. ��Ƽ�ؽ��� �ٷ�� 4.
	// ��Ʋ�� �ؽ��ĸ� ������ ������ �ؽ��ĸ� �ѹ��� �׷�����.
	// *** ���Ƿ� �ð� ó�� ���� ***
	// ** Texture Switch�� �߻����� �ʾ� Cache Miss�� �߻����� �ʴ´�! **
    /*
	new_Color = vec4(0.0f);
	int index = int(floor(v_Texture_UV.x * 3.0f));
	new_Texture_UV = vec2(fract(v_Texture_UV.x * 3.0f), ( (9 - u_Numbers[index] + v_Texture_UV.y) ) * 0.1f);
	
	new_Color = texture(u_Number_Texture_2, new_Texture_UV);

	FragColor = new_Color;
    */
	// =====================================




	

	// =====================================
	// 15. ��Ƽ�ؽ��� �ٷ�� 5.
	// <����> Atlas Texture�� �̿��Ͽ� Sprite Animation �����ϱ�.
	// => Sprite �ؽ��ĸ� �ϳ� ���ؼ� �ִϸ��̼� ����.
    //
    // (( ���� "Runner_Sprite" ��� �� ))
    //
    // => x �⺻ ��ǥ : v_Texture_UV.x / 8.0f;
    // ==> "Runner_Sprite"�� ���η� ������ "8 frame" �ִϸ��̼��̴�.
    // ==> (1) ������ ������ U ��ǥ�� "8"�� ������ �ش�.
    //
    //
    // => y �⺻ ��ǥ : (1.0f - v_Texture_UV.y) * 0.5f;
    // ==> "Runner_Sprite"�� ���η� ������ �ٸ� ������ �ִϸ��̼ǵ鵵 ������ �ִ�. (�� 2����)
    // ==> (1) ������, �켱 OpenGL ��ǥ�迡 �°� V ��ǥ�� ������ �ش�.
    // ==> (2) �������� V ��ǥ�� "2"�� �������ش�.
    //
    // => ���⼭�� �⺻ ��ǥ���� ���� �Ҽ������� ������(fract) �ʿ䰡 ����. --> ������ 0~1 ������ ���̹Ƿ�..
    // => ���� "���� 14��" ó�� U ��ǥ�� "3����" ���Ѽ� ����ؾ� �Ѵٸ� fract�� ����ؾ߰ڴ�.
    //
    //
    // => x Offset : mod(floor(u_Time  * animate_Speed), 8) / 8.0f;
    // ==> (1) 8 frame�ΰ��� �����Ͽ� "0~7�� ����"�� �̾Ƴ��� �Ѵ�.
    // ==> (1-1) �ð��� ���� �������� �ϹǷ� �ð����� floor�� ���ϵ�,
    // ==>      �ִϸ��̼� ���� �ӵ��� �ٲ��� �� �ֵ��� �ӵ����� �����ش�.
    // ==> (1-2) (1-1)�� ��� ���� 8�� ���� ���������� ���ϸ� 0~7�� ������ ���� �� �ִ�.
    // ==> (2) �̾Ƴ� 0~7�� �������� "�ٽ� 8�� �����ָ� x�� Offset��"�� ���� �� �ִ�.
    //
    //
    // => y Offset : mod(floor((u_Time + 1.0f) * 0.5f), 2) * 0.5f;
    // ==> V��ǥ�� Offset�� �������� ���������� ���� ����� 0.0f, �������� ���� ����� 0.5f �̾�� �Ѵ�.  
    // ==> (1) �켱 "0 or 1"�� ���� �ݺ��ǵ��� �ؾ� �Ѵ�.
    // ==> (1-1) �켱 �ð��� ���� ���ϰ� �ǹǷ�, �ð����� �޾ƿ´�.
    // ==> (1-2) �ش� �ִϸ��̼��� ȭ�� �߾ӿ��� �����ؼ� �������� ���� �̵� �� �ٽ� �������� �̵��ϱ⸦ �ݺ��Ѵ�.
    // ==> (1-3) �׷��� �߾ӿ��� �����Ѵٴ� ����, ���ݸ� �̵��� �� ������ �������ٴ� ���̹Ƿ�
    // ==>      ** �ð����� "0.5f"�� ������� �Ѵ�. **
    // ==> (1-4) ** .vs���� �ð��� "4�� ������" �帣�Ƿ�, ���⼭�� �ð����� "2�� �������" �Ѵ�. **
    // ==> (1-5) ** (1-3)�� (1-4)�� ���� ���ļ� (u_Time + 1.0f) * 0.5f ��� ���� ���´�. **
    // ==> (1-6) (1-5)�� �Ŀ��� "������"�� ���� ��, "2�� ���� ������"�� ���ϸ�, �ð��� ���� 0�� 1�� �ݺ��ϰԵǴ� ���� ��´�.
    // ==> (2) �ð��� ���� 0�� 1�� �ݺ��ϰԵǴ� ���� ������Ƿ�, ���� 0.5f�� �����ָ�ȴ�.
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
	// 16. ��Ƽ�ؽ��� �ٷ�� 6.
    // (����) Sprite �ؽ��ķ� �ִϸ��̼� ���� 2.
    // => �ؽ��� ������.
    //
    // ==> (1) ���� ���ϱ�.
    // ==> (1-1) ������ ��or�� �̹Ƿ�, 0�� 1�� ���� ������ �ε����� �����ֵ��� �Ѵ�.
    // ==> (1-2) mod(floor((u_Time + 1.0f) * 0.5f), 2)�� "���� 15"�� �ð��� ���� 0 �Ǵ� 1�� ���� �������ش�. (��ȯ���� float �̹Ƿ� int�� ����ȯ ����� �Ѵ�.)
    // ==> (1-3) �̸� �����ص� "���� ��ȣ �迭"�� ���� �ε����� �־��־� ���⿡ ���� ��ȣ�� ���Ѵ�. (-1 or 1)
    // ==> (1-4) (dir_index - dir * v_Texture_UV.x + 6.0f) / 9.0f �� "Gunner_Sprite"���� �̵� ����� ���ۺθ� ����Ų��.
    // ==>      ���������� �̵��ϴ� ����� Default �̹Ƿ�, (0 - (-1 * U ��ǥ) + 6.0f) / 9.0f �� ���ۺ�.
    // ==>      �������� �̵��ϴ� ����� ���� ��ǥ�� ������ ��� �Ѵ�.
    // ==>      ������ �ܼ��� (-U ��ǥ + 6.0f) / 9.0f �� �ƴ϶� (1.0f - U ��ǥ + 6.0f) / 9.0f �� ���ۺΰ� �ȴ�.
    //
    //
    // ==> (2) ������ ���ϱ�.
    // ==> (2-1) "Gunner_Sprite"�� �̵� ����� "3 frame" �̹Ƿ� mod(floor(u_Time * animate_Speed), 3) / 9.0f�� �����ش�.
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
	// 17. ��Ƽ�ؽ��� �ٷ�� 7.
	// => Sprite �ؽ��ķ� �ִϸ��̼� ���� 3. (�ǽ�)
	
    new_Color = vec4(0.0f);

	float x_index = floor( fract( u_X_Index / u_Resolution_X ) * u_Resolution_X );
	float y_index = floor( floor( u_X_Index / u_Resolution_X ) );

	new_Texture_UV.x = ( v_Texture_UV.x + x_index ) / u_Resolution_X;
    new_Texture_UV.y = ( v_Texture_UV.y + y_index ) / u_Resolution_Y;

    new_Color = texture(u_Flame_Sprite, new_Texture_UV);
	FragColor = new_Color;
	
	// =====================================
}