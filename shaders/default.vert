#version 410
layout(location = 0) in vec2 v_Position;
layout(location = 1) in vec2 v_TextureCoord;

out vec2 UV;

void main(void)
{
    gl_Position = vec4(v_Position, 0.0f, 1.0f);
    UV = v_TextureCoord;
}
