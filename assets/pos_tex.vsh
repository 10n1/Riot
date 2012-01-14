#version 330
layout(location = 0) in  vec4   vInPosition;
layout(location = 3) in  vec2   vInTex;

out vec2    vInterpTex;

void main()
{
    gl_Position = vInPosition;
    vInterpTex = vInTex;
}
