uniform vec3 objectColor;

varying vec3 v_pos;

void main()
{
    gl_FragColor = vec4(0.001 * (v_pos.z + 3.0),0.001 * (v_pos.z + 3.0), 0.001 * (v_pos.z + 3.0), 1.0);
}
