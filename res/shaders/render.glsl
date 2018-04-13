#shader vertex
#version 460 core

layout(location = 0) in vec2 model;
layout(location = 1) in vec2 position;
layout(location = 2) in float radius;
layout(location = 3) in float stress;

out float c;

void main()
{
    gl_Position = vec4(radius*model + position, 0.0, 1.0);
    float colorScale = 0.0001;
    c = clamp(colorScale*stress, 0.0, 1.0);
}

#shader fragment
#version 460 core

in float c;

out vec4 color;

void main()
{
    vec4 green = vec4(0.0, 0.5, 0.0, 1.0);
    vec4 yellow = vec4(1.0, 0.8, 0.0, 1.0);
    vec4 red = vec4(0.7, 0.2, 0.0, 1.0);
    vec4 stressColor;
    if(c < 0.5)
    {
        stressColor = mix(green, yellow, 2.0*c);
    }
    if( c >= 0.5)
    {
        stressColor = mix(yellow, red, 2.0*(c - 0.5));
    }
    //float r = clamp(-pow(2.0*c - 2.0, 2.0) + 1.0, 0.0, 1.0);
    //float g = clamp(-pow(2.0*c - 1.0, 2.0) + 1.0, 0.0, 1.0);
    //float b = clamp(-pow(2.0*c, 2.0) + 1.0, 0.0, 1.0);
    //color = vec4(b, g, b+g, 1.0);
    color = stressColor;
}