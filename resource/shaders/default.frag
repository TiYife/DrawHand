uniform vec3 objectColor;

varying vec3 v_normal;
//varying vec3 v_pos;

void main()
{
    vec3 norm = normalize(v_normal);
    vec3 lightDir = vec3(0,0,-1);
    float diff = max(dot(norm, lightDir), 0.3);

//    gl_FragColor = vec4(diff * objectColor, 1.0);
    gl_FragColor = vec4(1.0,0.0,0.0, 1.0);
}
