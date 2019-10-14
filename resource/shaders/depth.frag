uniform vec3 objectColor;

varying vec3 v_pos;

void main()
{
    gl_FragColor = vec4(0.1 * (v_pos.z + 0.0),0.1 * (v_pos.z + 0.0), 0.1 * (v_pos.z + 0.0), 1.0);
}
