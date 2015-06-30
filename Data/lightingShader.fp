#version 410

// material settings
uniform sampler2D texture;
uniform bool noTexture;
uniform vec4 inputColor;

uniform float materialShininess;
uniform vec3 materialSpecularColor;
uniform vec3 cameraPosition;

uniform mat4 modelView;

uniform struct Light
{
	vec3 position;
	vec3 intensities; //a.k.a the color of the light
	float attenuation;
	float ambientCoefficient;
} light;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec4 fragVert;

out vec4 outputColor;

void main()
{
	vec3 normal = normalize(transpose(inverse(mat3(modelView))) * fragNormal);
	vec3 surfacePos = vec3(modelView * fragVert);
	vec4 surfaceColor;
	
	if(noTexture)
	{
		surfaceColor = inputColor;
	}
	else
	{
		surfaceColor = texture2D(texture, fragTexCoord);
	}

	vec3 surfaceToLight = normalize(light.position - surfacePos);
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);
	
	//ambient
	vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

	//diffuse
	float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
	vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
	
	//specular
	float specularCoefficient = 0.0;
	if(diffuseCoefficient > 0.0)
		specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
	vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;
	
	//attenuation
	float distanceToLight = length(light.position - surfacePos);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

	//linear color (color before gamma correction)
	vec3 linearColor = ambient + attenuation*(diffuse + specular);
	
	//final color (after gamma correction)
	vec3 gamma = vec3(1.0/2.2);
	outputColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}
