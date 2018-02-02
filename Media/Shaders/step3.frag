#version 330
uniform sampler2D texture;
uniform sampler2D normaltexture;
in vec4 baseColor;
in vec2 uvCoords;
out vec4 fragColor;
void main( )
{
	vec4 color = texture2D( texture, uvCoords );
	fragColor = color *baseColor;
	//fragColor = color;
	//fragColor = baseColor;
} 
