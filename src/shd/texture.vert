#version 460 core

precision mediump float;

layout (binding = 0) uniform readonly writeonly image3D tex_sheet;
//layout (std140, binding = 0) uniform readonly buffer Texture_Sheets {
//    readonly writeonly image2D sheet[];
//} tex_sheet;

layout (location = 0) in vec2 v_pos;
layout (location = 1) in mat4 mat_pos;
layout (location = 5) in int tex_i;
layout (location = 6) in uvec4 tex_quad;

layout (location = 0) out vec2 tex_coords;
layout (location = 1) out float _tex_i;
out gl_PerVertex {
    vec4 gl_Position;
};

void main(void) {
    gl_Position = mat_pos * vec4(v_pos, 0.0, 1.0);
    tex_coords = v_pos * 0.5 + 0.5;
    vec3 img_size = imageSize(tex_sheet);
    tex_coords = (tex_coords * tex_quad.zw + tex_quad.xy) / img_size.xy;
    _tex_i = tex_i / img_size.z;
}
