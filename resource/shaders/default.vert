uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec4 a_position;
attribute vec2 a_normal;

varying vec3 v_pos;
varying vec2 v_normal;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = projection * view * model * a_position;

    v_pos = vec3(model * a_position);
    v_normal = a_normal;
}
