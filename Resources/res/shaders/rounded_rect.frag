// this shader will discard any pixels that fall outside the given corner radius, creating rounded rectangle effect.
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 u_pixelSize;
uniform vec2 u_texSize;
uniform vec4 u_normTexRect;
uniform float u_radius;
uniform float u_corners[4];

float udRoundRect(vec2 p, vec2 b, float r)
{
    return length(max(abs(p) - b, 0.0)) - r;
}

void main()
{
    vec2 coordScale = u_texSize / u_texSize;
    
    vec2 normalisedTexCoords = (v_texCoord - u_normTexRect.xy) * u_normTexRect.zw;
    
    vec2 center = u_pixelSize * 0.5;
    vec2 hsize = u_pixelSize * 0.5;
    vec2 uv = normalisedTexCoords * u_pixelSize;
    //get quadrent number for current pixel (0 = top left, 1 = top right, 2 = bottom left, 3 = bottom right)
    int corner = int(2.0 * floor(uv.y / center.y) + floor(uv.x / center.x));
    //calc if current pixel fals outside given corner radius
    float a = clamp(udRoundRect(uv - center, hsize - u_radius, u_radius), 0.0, 1.0);
    //discard pixel if rounding current corner quadrent and pixel is outside target radius
    if(a * u_corners[corner] > 0.0)
    {
        discard;
    }
    else
    {
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    }
    //gl_FragColor = vec4(normalisedTexCoords.x, normalisedTexCoords.y, 0.0, 1.0);
}
