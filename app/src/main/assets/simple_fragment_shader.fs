precision mediump float;

varying vec3 u_Color;
varying vec2 u_TexCoord;

uniform sampler2D ourTexture;

void main()
{
//gl_FragColor = vec4(u_Color,1.0);
gl_FragColor = texture2D(ourTexture, u_TexCoord) * vec4(u_Color,1.0);
}