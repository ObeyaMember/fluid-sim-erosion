#include "data.h"


void setup_texture_from_jpg(unsigned int* texture, const char* file_name, unsigned int* program, const char* tex_unit_name, GLint v0){
    
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    
    
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load and generate the texture
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    
    if (data){
    
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    
    }else {
        printf("Failed to load texture \n");
    }

    stbi_image_free(data);
}

void setup_texture_from_png(unsigned int* texture, const char* file_name, unsigned int* program, const char* tex_unit_name, GLint v0){
    
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    
    
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load and generate the texture
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file_name, &width, &height, &nrChannels, 0);
    
    if (data){
    
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    
    }else {
        printf("Failed to load texture \n");
    }

    stbi_image_free(data);
}

void print_vertex_array(float* a, int n_vertices){
    printf("[\n");
    for (int i = 0; i < 3 * n_vertices; i += 3){
        printf(" (%f, %f, %f)\n", a[i], a[i+1], a[i+2]);
    }
    printf(" ] \n");
}
