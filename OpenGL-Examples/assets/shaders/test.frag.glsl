#version 330 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

vec4 SampleTexture(int idx, vec2 texCoord)
{
    if (idx == 0) {
        return texture(u_Textures[0], texCoord);
    }
    else {
        return texture(u_Textures[1], texCoord);
    }
};



void main()
{
    int idx = int(v_TexIndex);
    vec4 texColor = SampleTexture(idx, v_TexCoord);
    o_Color = texColor;
}