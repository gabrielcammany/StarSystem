#version 330
 
in vec3 vertexPos;
in vec3 color;

out vec3 colorInt;
uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;
in vec2 uvs;
out vec2 v_uvs;
void main()
{
	v_uvs = uvs;
	
	gl_Position =  u_projection * u_view * u_model * vec4( vertexPos , 1.0 );

	// pass the colour to the fragment shader
	colorInt = color;
}