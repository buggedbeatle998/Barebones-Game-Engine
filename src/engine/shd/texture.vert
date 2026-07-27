#version 460 core

precision mediump float;


layout (binding = 0) uniform readonly writeonly image3D tex_sheet;

layout (location = 0) in vec2 v_pos;
layout (location = 1) in mat4 mat_pos;
layout (location = 5) in int tex_i;
layout (location = 6) in uvec4 tex_quad;
layout (location = 7) in vec4 blend;

layout (location = 0) out vec2 tex_coords;
layout (location = 1) out float _tex_i;
layout (location = 2) out vec4 _blend;
out gl_PerVertex {
    vec4 gl_Position;
};


const vec4 consts = vec4(0.0, 1.0, 2.0, -1.0);


void main(void) {
    gl_Position = fma(mat_pos * vec4(v_pos * tex_quad.zw, 0.0, 1.0), consts.zzxy, consts.wwxx);
    tex_coords = v_pos + consts.xy;
    vec3 img_size = imageSize(tex_sheet);
    tex_coords = fma(tex_coords, tex_quad.zw, tex_quad.xy) / img_size.xy;
    _tex_i = tex_i / img_size.z;
    _blend = blend;
}
