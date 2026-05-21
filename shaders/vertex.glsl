#version 330 core
layout (location = 0) in vec3 vertexpos;
layout (location = 1) in vec3 instancePos;
layout (location = 2) in vec3 rot;
layout (location = 3) in vec3 scale;

layout (location = 4) in int randomnumber;
layout (location = 5) in int faceidx;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

flat out int rand;
flat out int face;

vec3 rotate(vec3 v, float radians, vec3 axis) {
    return (v*cos(radians) + cross(axis, v)*sin(radians) + axis*dot(axis, v)*(1.0-cos(radians)));
}

void main() {
    vec3 scaled = vertexpos * scale;

    vec3 rotation = rotate(scaled, radians(rot.x), vec3(1.0, 0.0, 0.0));
    rotation = rotate(rotation, radians(rot.y), vec3(0.0, 1.0, 0.0));
    rotation = rotate(rotation, radians(rot.z), vec3(0.0, 0.0, 1.0));
                

    vec3 worldPos = rotation + instancePos;
    gl_Position = proj * view * model * vec4(worldPos, 1.0);
    rand = randomnumber;
    face = faceidx;
}