#version 330
// Fragment Shader - file "brush.frag"
out vec4 out_Color;
in vec4 brushColor;
in vec2 texCoord;
uniform sampler2D brushTexture;
void main( )
{
	fragColor = vec4(0.0, 0.3, 0.2, 1.0);
	const float alpha = 0.6f;
	vec4 intensity = texture2D(brushTexture, texCoord.st);
	out_Color = brushColor * intensity;
	out_Color.a *= alpha;
	//out_Color = vec4(0.5, 0.1, 0.3, 1.0);
} 