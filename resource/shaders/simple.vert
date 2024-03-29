uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec4 a_position;
attribute vec3 a_normal;

varying vec3 v_pos;
varying vec3 v_normal;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = projection * view * model * a_position;

    v_pos = vec3(gl_Position);
    v_normal = a_normal;
}
