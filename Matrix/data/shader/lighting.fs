uniform struct Light {
  vec4 position;
  vec3 color;
  float attenuation;
} light;

in vec3 fragVert;
in vec3 fragNormal;

vec3 diffuseComponent;
vec3 specularComponent;
vec3 surfaceToCamera;
vec3 normal;

void computeLighting() {
  vec3 lightPosition = vec3(1.0, 2.0, 5.0);
  float lightAttenuation = 0.02f;

  mat3 normalMatrix = transpose(inverse(mat3(modMat)));
  normal = normalize(normalMatrix * fragNormal);
  vec3 surfacePosition = vec3(modMat*vec4(fragVert, 1.0));
  vec3 surfaceToLight = normalize(lightPosition - surfacePosition);
  surfaceToCamera = normalize(cameraPosition - surfacePosition);

  float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
  float specularCoefficient = 0.0;
  if(diffuseCoefficient > 0.0)
    specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), mat_shininess);

  float distanceToLight = length(lightPosition - surfacePosition);
  float attenuation = 1.0 / (1.0 + lightAttenuation * pow(distanceToLight, 2));

  if(emissive == 1) {
    diffuseCoefficient = 1.0;
    specularCoefficient = 0.0;
    attenuation = 1.0;
  }

  diffuseComponent  = attenuation * diffuseCoefficient * mat_diffuse.rgb;
  specularComponent = attenuation * specularCoefficient * mat_specular.rgb;

  if(use_reflection == 1) {
    vec3 reflected = reflect(-surfaceToCamera, normal);
    diffuseComponent *= texture(texture_cube, reflected).rgb;
  }
}
