#version 330 core

flat in int rand;
flat in int face;

out vec4 Color;

uniform vec4 colors[7];
uniform bool scrambled;

void main() {
    if(scrambled) {
        if(face < 6) Color = colors[rand % 6];
        else Color = colors[face]; //Default and Other
    }
    else Color = colors[face];
}