#shader vertex
#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 velocity;
layout(location = 2) in float stress;
layout(location = 3) in float radius;

layout(location = 4) uniform samplerBuffer allRadii;
layout(location = 5) uniform samplerBuffer allPositions;
layout(location = 6) uniform samplerBuffer allVelocities;

layout(xfb_buffer = 0) out vec2 newPosition;
layout(xfb_buffer = 1) out vec2 newVelocity;
layout(xfb_buffer = 2) out float newStress;
layout(location = 9) uniform float angle;
vec2 newForce = vec2(0.0, -50.0);

layout(location = 10) uniform float boxScale = 1.0;

layout(location = 7) uniform int count;
layout(location = 8) uniform vec2 mouse;

float dt = 0.001;

float alpha = 0.5;
float beta = 3.0/2.0;
float ks = 1000000.0;
float viscosity = 0.1;
float kd = viscosity*(2.0*sqrt(ks));
float kt = 0.00;

void wallCollisions()
{
    // right wall
    vec2 separation = vec2(boxScale, position.y) - position;
    float overlap = radius - length(separation);
    if(overlap > 0)
    {
        vec2 n = normalize(separation);
        vec2 v = velocity;
        float overlapRate = dot(v, n);
        vec2 vt = v - overlapRate*n;
        float fn = -kd*pow(overlap, alpha)*overlapRate - ks*pow(overlap, beta);
        vec2 Fn = fn*n;
        vec2 Ft = -kt*vt;
        newForce = newForce + Fn + Ft;
        newStress = newStress + abs(length(Fn) + length(Ft));
    }

    // left wall
    separation = vec2(-boxScale, position.y) - position;
    overlap = radius - length(separation);
    if(overlap > 0)
    {
        vec2 n = normalize(separation);
        vec2 v = velocity;
        float overlapRate = dot(v, n);
        vec2 vt = v - overlapRate*n;
        float fn = -kd*pow(overlap, alpha)*overlapRate - ks*pow(overlap, beta);
        vec2 Fn = fn*n;
        vec2 Ft = -kt*vt;
        newForce = newForce + Fn + Ft;
        newStress = newStress + abs(length(Fn) + length(Ft));
    }

    // bottom wall
    separation = vec2(position.x, -boxScale) - position;
    overlap = radius - length(separation);
    if(overlap > 0)
    {
        vec2 n = normalize(separation);
        vec2 v = velocity;
        float overlapRate = dot(v, n);
        vec2 vt = v - overlapRate*n;
        float fn = -kd*pow(overlap, alpha)*overlapRate - ks*pow(overlap, beta);
        vec2 Fn = fn*n;
        vec2 Ft = -kt*vt;
        newForce = newForce + Fn + Ft;
        newStress = newStress + abs(length(Fn) + length(Ft));
    }

    // top wall
    separation = vec2(position.x, boxScale) - position;
    overlap = radius - length(separation);
    if(overlap > 0)
    {
        vec2 n = normalize(separation);
        vec2 v = velocity;
        float overlapRate = dot(v, n);
        vec2 vt = v - overlapRate*n;
        float fn = -kd*pow(overlap, alpha)*overlapRate - ks*pow(overlap, beta);
        vec2 Fn = fn*n;
        vec2 Ft = -kt*vt;
        newForce = newForce + Fn + Ft;
        newStress = newStress + abs(length(Fn) + length(Ft));
    }
}

void particleCollisions()
{
    vec2 force = {0.0, 0.0};
    for(int i = 0; i < count; i++)
    {
        if(i != gl_VertexID)
        {
            vec2 separation = texelFetch(allPositions, i).xy - position;
            vec2 v = texelFetch(allVelocities, i).xy - velocity;
            float r = texelFetch(allRadii, i).x;
            vec2 n = normalize(separation);
            float overlap = (radius + r) - length(separation);
            if(overlap > 0)
            {
                vec2 n = normalize(separation);
                vec2 v = velocity;
                float overlapRate = dot(v, n);
                vec2 vt = v - overlapRate*n;
                float fn = -kd*pow(overlap, alpha)*overlapRate - ks*pow(overlap, beta);
                vec2 Fn = fn*n;
                vec2 Ft = -kt*vt;
                newForce = newForce + Fn + Ft;
                newStress = newStress + abs(length(Fn) + length(Ft));
            }
        }
    }
    newForce = newForce + force;
}

void boundsCheck()
{
    float m;
    float xsq = pow(newPosition.x, 2.0);
    float ysq = pow(newPosition.y, 2.0);
    if (xsq >= ysq)
    {
        m = boxScale*sqrt(1 + ysq);
    }
    else
    {
        m = boxScale*sqrt(xsq + 1);
    }
    if(length(newPosition) > (m - 0.01*radius))
    {
        newPosition = normalize(newPosition)*(m - 0.5*radius);
        newVelocity = newVelocity*0.0;
    }
}

void mouseForce()
{
    vec2 separation = mouse - position;
    vec2 v = -velocity;
    vec2 n = normalize(separation);
    float overlap = 0.2 - length(separation);
    if(overlap > 0)
    {
        vec2 n = normalize(separation);
        vec2 v = velocity;
        float overlapRate = dot(v, n);
        vec2 vt = v - overlapRate*n;
        float fn = -10.0*kd*pow(overlap, alpha)*overlapRate - ks*pow(overlap, beta);
        vec2 Fn = fn*n;
        vec2 Ft = -kt*vt;
        newForce = newForce + (Fn + Ft)*0.1;
        newStress = newStress + abs(length(Fn) + length(Ft));
    }
}

void main()
{
    newStress = 0.0;
    particleCollisions();
    wallCollisions();
    mouseForce();
    newVelocity = velocity + newForce*dt;
    newPosition = position + newVelocity*dt;
    boundsCheck();
}

#shader fragment
#version 460 core

out vec4 color;

void main()
{  
    color = vec4(0.0, 0.0, 0.0, 1.0);
}