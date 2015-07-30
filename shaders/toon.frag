#version 300 es
precision mediump float;

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;


// Ouput data
out vec3 color;


// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;


void main(){
	// Material properties
	vec3 MaterialDiffuseColor = 0.2 * MaterialDiffuse + texture( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = MaterialAmbient + vec3(0.5,0.5,0.5) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = MaterialSpecular;


	vec3 N = normalize(Position_worldspace);

    vec3 L = normalize(LightPosition_worldspace);

    vec3 E = vec3(0, 0, 1);

    vec3 H = normalize(L + E);

	float Shininess = 0.8;
     
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, Shininess);
	color = MaterialAmbientColor + df * MaterialDiffuseColor + sf * MaterialSpecularColor;
}
