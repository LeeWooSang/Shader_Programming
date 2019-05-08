#version 450

out vec4 FragColor;
in vec2 v_UV;
// r = x, g = y, b = 0, a = 1;
in vec4 v_Pos;
const float PI = 3.141592;

uniform float u_Time;
uniform vec2 u_Points[5];

//#define DRAW_CIRCLE
void main()
{
	vec2 newUV = v_UV.xy - vec2(0.5, 0.5);	// -0.5 ~ 0.5 사이의 값
	// 점과 점사이의 거리를 나타내는 함수 length
	float distance = length(newUV);

#ifdef DRAW_CIRCLE
	 // 원그리기
	if(distance < 0.5 && distance > 0.48)
		FragColor = vec4(1, 1, 1, 1);
	else
		FragColor = vec4(0, 0, 0, 0);

#else
	float pointGrey = 0;
	float newTime = fract(u_Time);
	float ringWidth = 0.1;

	// 원 그리기
	if(distance < newTime + ringWidth && distance > newTime)
	{
		// distance는 0.5 - 0.1 = 0.5 / 0.1 = 5;
		float temp = (distance - newTime) / ringWidth;
		pointGrey = temp;

		for(int i = 0; i < 5; ++i)
		{
			vec2 newPoint = u_Points[i];
			vec2 newVec = newPoint - newUV;
			float distance2 = length(newVec);
			if(distance2 < 0.1)
			{
				pointGrey += 0.5 * pow(1 - (distance2 / 0.1f), 5);
			}
		}
	}

	FragColor = vec4(pointGrey);

#endif
}
