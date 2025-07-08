#include "shaders.h"

char* get_shader_content(const char* fileName){
    FILE *fp;
    long size = 0;
    char* shaderContent;
    
    /* Read File to get size */
    fp = fopen(fileName, "rb");
    if(fp == NULL) {
        return "";
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp)+1;
    fclose(fp);

    /* Read File for Content */
    fp = fopen(fileName, "r");
    shaderContent = memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size-1, fp);
    fclose(fp);

    return shaderContent;
}

void setup_fvc_shader(unsigned int* shader, const char** shaderSource, const char* type){
    if (strcmp(type, "fragment") == 0){
        *shader = glCreateShader(GL_FRAGMENT_SHADER);
    }else if (strcmp(type, "vertex") == 0){
        *shader = glCreateShader(GL_VERTEX_SHADER);
    }else if (strcmp(type, "compute") == 0){
        *shader = glCreateShader(GL_COMPUTE_SHADER);
    }else {
        printf("wrong type given - setup_fvc_shader: given type was: %s \n", type);
    }
    
    glShaderSource(*shader, 1, shaderSource, NULL);
    glCompileShader(*shader);


}

void setup_fvc_shader_prog(unsigned int* program, unsigned int* fragmentShader, unsigned int* vertexShader, unsigned int* computeShader, const char* type){
    *program = glCreateProgram();
    if (strcmp(type, "compute") == 0){
        glAttachShader(*program, *computeShader);
    }else if (strcmp(type, "main_pipeline") == 0){
        glAttachShader(*program, *fragmentShader);
        glAttachShader(*program, *vertexShader);
    }else {
        printf("wrong type given - setup_fvc_shader_prog: given type was: %s \n", type);
    }
    
    glLinkProgram(*program);

    if (strcmp(type, "compute") == 0){
        glDeleteShader(*computeShader);
    }else if (strcmp(type, "main_pipeline") == 0){
        glDeleteShader(*fragmentShader);
        glDeleteShader(*vertexShader);
    }else {
        printf("wrong type given - setup_fvc_shader_prog: given type was: %s \n", type);
    }
}


void fvc_shader_comp_error(unsigned int* shader, const char* type){
    GLint status;

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if(status == GL_TRUE){

    }
    else {
        printf("Compile error: %s shader\n", type);
        char infoLog[256];
        glGetShaderInfoLog(*shader, sizeof(infoLog), NULL, infoLog);
        printf("\n");
        printf(infoLog);
        printf("\n");
    }

}

void fvc_shader_prog_link_error(unsigned int* program, const char* type){
    GLint status;
   
    glGetProgramiv(*program, GL_LINK_STATUS, &status);
    if(status == GL_TRUE){
        //glUseProgram(program); // IDK if use here or in while loop
    }
    else {
        printf("Linking error: %s program\n", type);
        printf("\n");
    }
}
