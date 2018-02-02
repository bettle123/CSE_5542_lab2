#version 330
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
layout(location=0) in vec3 position;
layout(location=3) in vec2 inTexCoords;
out vec2 uvCoords;
void main()
{
    // transform vertex to clip space coordinates
    //gl_Position = projectionMatrix * modelViewMatrix * vec4(position,1);
    gl_Position = vec4( (2*inTexCoords.x-1),(2*inTexCoords.y-1), 0.0, 1.0);
	//baseColor = color;
	uvCoords = inTexCoords;
}
