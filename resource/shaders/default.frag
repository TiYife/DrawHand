uniform vec3 objectColor;

varying vec2 v_normal;
varying vec3 v_pos;

void main()
{
    vec3 norm = normalize(v_normal);
    vec3 lightDir = vec3(0,0,-1);
    float diff = max(dot(norm, lightDir), 0.3);

//    gl_FragColor = diff * objectColor;
    gl_FragColor = vec4(255,0,0, 1.0);
}
