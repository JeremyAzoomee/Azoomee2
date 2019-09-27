uniform vec2 u_pixelSize;
uniform float u_radius;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

// Rounded rect distance function
float udRoundRect(vec2 p, vec2 b, float r)
{
	return length(max(abs(p) - b, 0.0)) - r;
}

void main()
{
	vec2 center = u_pixelSize / 2.0;
	vec2 hsize = u_pixelSize / 2.0;
	vec2 fragCoord = v_texCoord * u_pixelSize;
	float a = clamp(udRoundRect(fragCoord - center, hsize - u_radius, u_radius), 0.0, 1.0);

    gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    gl_FragColor.a *= a;
}