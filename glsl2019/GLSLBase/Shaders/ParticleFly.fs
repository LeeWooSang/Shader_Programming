#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_Orixy;
in float v_Radius;

uniform sampler2D u_Texture;

void main()
{
	/*
	vec4 newColor;
	float distance = sqrt(v_Orixy.x * v_Orixy.x + v_Orixy.y * v_Orixy.y);
	if(distance < v_Radius)
	{
		newColor = v_Color;
		newColor.a = pow(1 - distance / v_Radius, 2);
	}
	else
	{
		newColor = vec4(0, 0, 0, 0);
		//newColor = v_Color;
	}

	FragColor = newColor;
	*/
	//FragColor = texture(u_Texture, v_Orixy);
	FragColor = vec4((v_Orixy / v_Radius), 0, 1);
}
