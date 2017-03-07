#version 330

out vec4 fragColor;
uniform sampler2D u_texture;
in vec2 v_uvs;
uniform vec3 u_cam_pos;
uniform vec3 u_ambient;
uniform vec3 u_light_dir;
uniform float u_shininess;



void main(void)
{
	vec3 texture_color = texture2D(u_texture, v_uvs).xyz;

	vec3 N = normalize(v_normal);
	
	vec3 L = normalize(u_light_dir);

	float NdotL = max( dot(N,L) , 0.0 );

	vec3 ambient_color = texture_color * u_ambient;

	vec3 diffuse_color = texture_color * NdotL;

	vec3 E = normalize(u_cam_pos - v_world_vertex_pos);

	float RdotE = pow( max(dot(R, -E), 0.0), u_shininess);

	vec3 specular_color = u_light_color * RdotE;
	

	fragColor = vec4(final_color, u_transparency);
}