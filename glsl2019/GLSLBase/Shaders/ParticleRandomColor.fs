#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_Orixy;
in float v_Radius;

void main()
{
	
	vec4 newColor;
	float distance = length(v_Orixy);

	if(distance < v_Radius)
	{
		newColor = v_Color;
		newColor.a = pow(1 - distance / v_Radius, 2);
	}
	else
	{
		newColor = vec4(0, 0, 0, 0);
	}

	FragColor = newColor;
}
