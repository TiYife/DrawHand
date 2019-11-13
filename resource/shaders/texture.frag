uniform sampler2D texture;

varying vec4 v_pos;
varying vec4 v_normal;
varying vec2 v_texcoord;
varying vec4 v_eyepos;   // Eye position in world space.

vec4 light_color = vec4(0.3f, 0.3f, 0.3f, 1.0f);
vec4 light_pos = vec4(-1000.f, 0, 0, 1.f);

vec4 material_emissive = vec4(0);
vec4 material_diffuse = vec4(1);
vec4 material_specular = vec4(1);
float material_shininess = 50.f;

vec4 Ambient = vec4(0.7f, 0.7f, 0.7f, 1.f);


void main()
{
    // Compute the emissive term.
    vec4 Emissive = material_emissive;

    // Compute the diffuse term.
    vec4 N = normalize( v_normal );
    vec4 L = normalize( light_pos - v_pos );
//    vec4 L = normalize( light_pos);
    float NdotL = max( dot( N, L ), 0.1 );
    vec4 Diffuse =  NdotL * light_color * material_diffuse;

    // Compute the specular term.
//    vec4 V = normalize( v_eyepos - v_pos );
//    vec4 H = normalize( L + V );
//    vec4 R = reflect( -L, N );
//    float RdotV = max( dot( R, V ), 0 );
//    float NdotH = max( dot( N, H ), 0 );
//    vec4 Specular = pow( NdotH, material_shininess ) * light_color * material_specular;

//    gl_FragColor = ( Emissive + Ambient + Diffuse + Specular ) * texture2D( texture, v_texcoord );
    gl_FragColor = ( Ambient + Diffuse ) * texture2D( texture, v_texcoord );
//        gl_FragColor =texture2D( texture, v_texcoord );

}


//void main()
//{
//    // Set fragment color from texture
//    gl_FragColor = texture2D(texture, v_texcoord);
//}
