
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 u_pixelSize;
uniform float u_radius;

float udRoundRect(vec2 p, vec2 b, float r)
{
    return length(max(abs(p) - b, 0.0)) - r;
}

void main()
{
    vec2 center = u_pixelSize * 0.5;
    vec2 hsize = u_pixelSize * 0.5;
    vec2 uv = v_texCoord.xy * u_pixelSize;

    float a = clamp(udRoundRect(uv - center, hsize - u_radius, u_radius), 0.0, 1.0);
    
    gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    gl_FragColor.a *= (1.0 - a);
}
