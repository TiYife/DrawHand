uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec4 a_position;
attribute vec4 a_normal;
attribute vec2 a_texcoord;

varying vec3 v_pos;
varying vec3 v_normal;
varying vec2 v_texcoord;
varying vec4 v_eyepos;   // Eye position in world space.


//! [0]
void main()
{
    gl_Position = projection * view * model * a_position;

    v_pos = vec3(model * a_position);
//    v_normal = vec3(model * a_normal);
    v_normal = mat3(transpose(inverse(model))) * vec3(a_normal);
    v_texcoord = a_texcoord;
    v_eyepos = view[3];
}
