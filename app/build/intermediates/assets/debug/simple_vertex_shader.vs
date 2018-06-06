attribute vec4 a_Position;
attribute vec3 a_Color;
attribute vec2 a_TexCoord;

varying vec3 u_Color;
varying vec2 u_TexCoord;

uniform mat4 unitMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
gl_Position =projection * view * model * a_Position;
u_Color = a_Color;
u_TexCoord = a_TexCoord;

}