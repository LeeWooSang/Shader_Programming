#version 450

//#define DRAW_CHECKERBOARD
//#define DRAW_TILING
//#define DRAW_REVERSE
//#define DRAW_BLUR
//#define DRAW_1
//#define DRAW_2
#define DRAW_3

out vec4 FragColor;
in vec2 UV;
uniform sampler2D u_Texture;
uniform float u_Time;

void main()
{

#ifdef DRAW_CHECKERBOARD
	// 체크보드 그리기
	vec4 newColor = texture(u_Texture, UV);

#elif defined(DRAW_TILING)
	// 타일링
	//vec2 newUV = fract(UV * 2 + vec2(u_Time, 0));
	vec2 newUV = fract(UV * 3 + vec2(cos(u_Time), sin(u_Time)));
	vec4 newColor = vec4(newUV, 0, 1);
	newColor = texture(u_Texture, newUV);

#elif defined(DRAW_REVERSE)
	// 텍스쳐 반전 시키기
	vec2 newUV = abs(UV - vec2(0, 0.5)) + vec2(0, 0.5);
	vec4 newColor = vec4(newUV, 0, 1);
	newColor = texture(u_Texture, newUV);

#elif defined(DRAW_BLUR)
	
	vec2 newUV[5];

	newUV[0] = vec2(UV.x, UV.y);
	newUV[1] = vec2(UV.x -1 / 4, UV.y);
	newUV[2] = vec2(UV.x + 1 / 4, UV.y);
	newUV[3] = vec2(UV.x, UV.y - 1 / 4);
	newUV[4] = vec2(UV.x, UV.y + 1 / 4);

	vec4 BlurColor[5];

	BlurColor[0] = texture(u_Texture, newUV[0]);
	BlurColor[1] = texture(u_Texture, newUV[1]);
	BlurColor[2] = texture(u_Texture, newUV[2]);
	BlurColor[3] = texture(u_Texture, newUV[3]);
	BlurColor[4] = texture(u_Texture, newUV[4]);

	vec4 newColor = BlurColor[0] + BlurColor[1] + BlurColor[2] + BlurColor[3] + BlurColor[4];
	newColor /= 5;

#elif defined(DRAW_1)

vec2 newUV;

// x좌표는 0에서 1을 반복하게 하고, 정수 자리를 날린다.
newUV.x = fract(UV.x * 3.0f);	
newUV.y = UV.y / 3.0f;
// y좌표는 0에서 1, 1에서 2, 2에서 3으로 만들면 된다. (0 ~ 1) / 3
// offset + 0 ~ 1 / 3
newUV.y += floor(UV.x * 3.0f) / 3.0f;
vec4 newColor = texture(u_Texture, newUV);

#elif defined(DRAW_2)

vec2 newUV;

newUV.x = fract(UV.x * 3.0f);	
newUV.y = UV.y / 3.0f;
newUV.y += (2 - floor(UV.x * 3.0f)) / 3.0f;

vec4 newColor = texture(u_Texture, newUV);

#elif defined(DRAW_3)

vec2 newUV;

newUV.x = UV.x;
// 2, 1, 0 offset + 0 ~ 1
// 2/3, 1/3, 0
newUV.y = (2 - floor(UV.y * 3)) / 3; 
newUV.y += fract(UV.y * 3) / 3;

vec4 newColor = texture(u_Texture, newUV);

#endif
	FragColor = newColor;
}
