#version 460 core

precision mediump float;


layout (binding = 0) uniform sampler3D tex_sheet;

layout (location = 0) in vec2 tex_coords;
layout (location = 1) in float tex_i;

layout (location = 0) out vec4 fragment;


void main(void) {
    fragment = texture(tex_sheet, vec3(tex_coords, tex_i));
}
