#version 330
uniform sampler2D texture;
in vec2 uvCoords;
out vec4 fragColor;
void main( )
{
	//vec4 color = texture2D( texture, uvCoords );
	fragColor = vec4(uvCoords, 0.0, 1.0);
} 
