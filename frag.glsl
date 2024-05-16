#version 410


// Define INPUTS from fragment shader
//uniform mat4 view_mat;
in vec3 Normal;
in vec3 fpos;

// These come from the VAO for texture coordinates.
varying vec2 textures;

// And from the uniform outputs for the textures setup in main.cpp.
uniform sampler2D texture00;
uniform sampler2D texture01;

//get light info
uniform float lightX;
uniform float lightY;
uniform float lightZ;
uniform float specEx;
uniform int diffFlag;
uniform int specFlag;
uniform int texFlag;

out vec4 fragment_color; //RGBA color

vec3 lightPos = vec3(lightX,lightY,lightZ);
const vec3 specColor = vec3(1.0,1.0,1.0);


void main () {
     
    vec3 diffColor = texture2D(texture00 ,textures).rgb;

    if(texFlag == 0) {
       diffColor = vec3(1.0, 0.5, 0.0);
    }
    vec3 normal = normalize(Normal);
  
    vec3 lightDir = normalize(lightPos - fpos);
    float amb = max(dot(lightDir ,normal), 0.0);
    
    float spec = 0.0;
    if (amb > 0.0) {
       vec3 refDir = reflect(-lightDir, normal);
       vec3 viewDir = normalize(-fpos);

       float specAngle = max(dot(refDir, viewDir), 0.0);
       spec = pow(specAngle, specEx);
     }

     if (diffFlag == 0) {
        amb = 1.0;
     }

     if (specFlag == 0) {
     	spec = 0.0;
     }

     fragment_color = vec4(amb * diffColor + spec * specColor, 1.0);
  }
