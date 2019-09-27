
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 u_pixelSize;
uniform float u_radius;
uniform float u_corners[4];

float udRoundRect(vec2 p, vec2 b, float r)
{
    return length(max(abs(p) - b, 0.0)) - r;
}

void main()
{
    vec2 center = u_pixelSize * 0.5;
    vec2 hsize = u_pixelSize * 0.5;
    vec2 uv = v_texCoord.xy * u_pixelSize;

    int corner = int(2.0 * floor(uv.y / center.y) + floor(uv.x / center.x));
    
    float a = clamp(udRoundRect(uv - center, hsize - u_radius, u_radius), 0.0, 1.0);
    
    if(a * u_corners[corner] > 0.0)
    {
        discard;
    }
    else
    {
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
        gl_FragColor.a *= 1.0 - (a * u_corners[corner]);
    }
}
