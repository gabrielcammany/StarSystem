#version 330

out vec4 fragColor;
in vec2 v_uvs;
uniform sampler2D u_texture;


void main(void)
{
	vec4 texture_color = texture2D(u_texture, v_uvs);

	fragColor =  vec4(texture_color.xyz, 1.0);
}
