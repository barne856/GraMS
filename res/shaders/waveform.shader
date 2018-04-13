#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) uniform mat4 translate;
layout(location = 2) uniform float scale;

void main()
{
    gl_Position = vec4((translate * position).x*scale, (translate * position).yzw);
}

#shader fragment
#version 460 core

out vec4 color;

void main()
{
    color = vec4(0.0, 0.0, 0.0, 1.0);
}