#version 330
// Vertex Shader - file "brush.vert"
layout(location=0) in vec3 position;
out vec4 color;
void main()
{
	gl_Position = vec4(position, 1.0);	
	color = vec4(0.95f,0.56f,0.23f,0.1f);
}
