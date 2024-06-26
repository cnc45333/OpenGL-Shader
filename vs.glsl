#version 410

layout (location = 0) in vec3 vtxPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;


uniform mat4 proj_mat, view_mat, model_mat;

out vec3 Normal;
out vec3 fpos;

varying vec2 textures;


void main () {
     gl_Position = proj_mat * view_mat * model_mat *vec4(vtxPosition, 1.0);
     fpos = vec3(model_mat * vec4(vtxPosition, 1.0));
     Normal = mat3(transpose(inverse(model_mat))) * normal;
     textures = texture;
}
