uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec4 a_position;
attribute vec2 a_texcoord;

varying vec3 v_pos;
varying vec2 v_texcoord;

//! [0]
void main()
{
    gl_Position = projection * view * model * a_position;

    v_pos = vec3(gl_Position);
    v_texcoord = a_texcoord;
}
