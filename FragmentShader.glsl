#version 330 core
out vec4 FragColor;
uniform vec4 ourcolor_1;
uniform vec4 ourcolor_2;
uniform float ouropaciyt;
in vec2 ourtexturecoords;
uniform int duplicate_1;
uniform int duplicate_2;
uniform sampler2D ourTexture_1;
uniform sampler2D ourTexture_2;
void main()
{
FragColor = mix(texture(ourTexture_1 , duplicate_1 * ourtexturecoords) * ourcolor_1 , texture(ourTexture_2 , duplicate_2 * ourtexturecoords) * ourcolor_2, ouropaciyt);
}
