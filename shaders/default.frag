#version 410
uniform float u_Time;
uniform sampler2D u_Texture;
in vec2 UV;

void main(void)
{
    gl_FragColor = vec4(UV, cos(u_Time), 1.0f);
}
