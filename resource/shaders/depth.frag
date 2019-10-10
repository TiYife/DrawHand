uniform vec3 objectColor;

varying vec3 v_pos;

void main()
{
    gl_FragColor = vec4(v_pos.z,v_pos.z, v_pos.z, 1.0);
}
