#version 330 core
layout (location = 0) in vec3 position;
//layout (location = 2) in vec2 texCoord;

//out vec2 TexCoord;

uniform mat4 model; //coordenadas locais para coordenadas de camera
uniform mat4 view; // coordenas normalizadas para coordenadas de janela
uniform mat4 projection; //coordenas de camera para coordenadas normalizadas

//uniform mat4 transform;

void main()
{
    //gl_Position = transform * vec4(position, 1.0f);
	gl_Position = projection * view * model * vec4(position, 1.0f);

    //TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}