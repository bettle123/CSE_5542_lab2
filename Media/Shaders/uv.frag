#version 330
uniform sampler2D texture;
in vec2 uvCoords;
in vec4 baseColor;
out vec4 fragColor;
void main( )
{
	//vec4 color = texture2D( texture, uvCoords );
	fragColor = baseColor;
	//fragColor = vec4(0.5, 0.6, 0.1, 1);
} 
