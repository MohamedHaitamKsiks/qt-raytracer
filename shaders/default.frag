#version 460

uniform sampler2D u_Texture;
uniform int u_FrameCounter;
in vec2 UV;
out vec4 fragColor;

void main(void)
{
    fragColor = vec4(texture(u_Texture, UV).rgb, 1.0f);
}
