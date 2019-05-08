#version 450

out vec4 FragColor;

in vec2 UV;
uniform sampler2D u_Texture;
const float PI = 3.141592;

uniform float u_Time;

#define SINMOVE
void main()
{
	vec4 newColor = vec4(0, 0, 0, 0);

#ifdef SINMOVE
	// ���� �׷��� �����̱�
	float x = UV.x * 2 * PI + u_Time;
	float sinResult = (sin(x + u_Time) + 1) * 0.5;

	if(sinResult > UV.y && sinResult - 0.01 < UV.y)
		newColor = vec4(1, 1, 1, 1);
#else
	// ���� �׷��� �ؿ� ��ĥ
	float x = UV.x * 2 * PI;
	float y = (sin(x) + 1) * 0.5;
	if(y > UV.y)
		newColor = vec4(1, 1, 1, 1);
#endif
	//FragColor = texture(u_Texture, UV);
	FragColor = newColor;
}
