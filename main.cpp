#include <iostream>
#include <fstream>
#include <sstream>
#include <string>  
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Function to read the entire content of a file into a std::string
std::string readShaderSource(const std::string& filepath) {
    std::ifstream vertexFile;
    // ensure ifstream objects can throw exceptions:
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open file
        vertexFile.open(filepath);
        std::stringstream shaderStream;
        // read file's buffer contents into stream
        shaderStream << vertexFile.rdbuf();
        // close file handler
        vertexFile.close();
        // convert stream into string
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filepath << std::endl;
        return ""; // Return empty string on failure
    }
}



void window_resizing (GLFWwindow *window , int width , int hieght){
    glViewport(0 , 0 , width , hieght); 
}

void duplicate(GLint duplicate_uniform_location , GLuint program , int iteration){
    glUseProgram(program);
    glUniform1i(duplicate_uniform_location , 2 * iteration);
}

void window_closing(GLFWwindow *window){
    if (glfwGetKey(window , GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window , true);
    }
}

void setFilter(GLint color_uniform_location , GLuint program , float R , float G , float B){
    glUseProgram(program);
    glUniform4f(color_uniform_location , R , G , B , 1.0);
}


int main()
{
    if (!glfwInit()){
        std:: cout<< "cannot load GLFW library" << "\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow (800 , 600 , "Photo Editor" , NULL , NULL);
    if (!window){
        std:: cout<< "window openning is failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std:: cout << "glad functions cannot be loaded\n";
        glfwTerminate();
        return -1;
    }
    glViewport(0 , 0 , 800 , 600);

    /*--------------reading shaders from file---------------------*/
    std::string vertexCode = readShaderSource("VertexShader.glsl");
    std::string fragmentCode = readShaderSource("FragmentShader.glsl");

    if (vertexCode.empty() || fragmentCode.empty()) {
        glfwTerminate();
        return -1;
    }
    const char* vertex_shader = vertexCode.c_str();
    const char* fragment_shader = fragmentCode.c_str();




float vertices[] = {

1.0f, -1.0f, 0.0f, 1.0f , 0.0f,
1.0f, 1.0f, 0.0f,  1.0f , 1.0f,
-1.0f,1.0f, 0.0f,  0.0f , 1.0f,
-1.0f,1.0f, 0.0f,  0.0f , 1.0f,
-1.0f,-1.0f, 0.0f, 0.0f , 0.0f,
1.0f, -1.0f, 0.0f, 1.0f , 0.0f

};

    /*sending data to the GPU & giving attributes to vertices*/
    GLuint VBO;
    glGenBuffers(1 , &VBO);
    glBindBuffer(GL_ARRAY_BUFFER , VBO);
    glBufferData(GL_ARRAY_BUFFER , sizeof(vertices) , vertices , GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1 , &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs , 1 , &vertex_shader , NULL);
    glCompileShader(vs);
    /*------------vertex shader compileition Errors check--------*/
    int success_vs;
    char infoLog_vs[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success_vs);
    if (!success_vs){
        glGetShaderInfoLog(vs, 512, NULL, infoLog_vs);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
        infoLog_vs << std::endl;
    }
    /*--------------------------------------------------*/
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs , 1 , &fragment_shader , NULL);
    glCompileShader(fs);
    /*------------fragment shader compileition Errors check--------*/
    int success_fs;
    char infoLog_fs[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success_fs);
    if (!success_fs){
        glGetShaderInfoLog(vs, 512, NULL, infoLog_fs);
        std::cout << "ERROR::SHADER::Fragment::COMPILATION_FAILED\n" <<
        infoLog_fs << std::endl;
    }

    /*-----------------------shader program---------------------------*/
   
    GLuint program = glCreateProgram();
    glAttachShader(program , vs);
    glAttachShader(program , fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);


    char file_name[256];
    std:: cout<<"First texture path:"<<std:: endl;
    fgets(file_name, sizeof(file_name), stdin);
    file_name[strcspn(file_name, "\n")] = '\0';

    stbi_set_flip_vertically_on_load(true);
    int width_1, height_1, nrChannels_1;
    unsigned char *data_1 = stbi_load(file_name, &width_1 , &height_1 , &nrChannels_1, 0);
    GLuint texture_1;
    glGenTextures(1 , &texture_1);
    glBindTexture(GL_TEXTURE_2D , texture_1);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    if (data_1){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_1, height_1, 0, GL_RGB,GL_UNSIGNED_BYTE, data_1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std:: cout<<"failed to load texture!";
        glfwTerminate();
        return -1;
    }

    /*-------------------------------second texture----------------------------------*/
    char file_name_2[256];
    std:: cout<<"Second texture path:"<<std:: endl;
    fgets(file_name_2, sizeof(file_name_2), stdin);
    file_name_2[strcspn(file_name_2, "\n")] = '\0';
    int width_2, height_2, nrChannels_2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data_2 = stbi_load(file_name_2, &width_2 , &height_2 , &nrChannels_2, 0);
    GLuint texture_2;
    glGenTextures(1 , &texture_2);
    glBindTexture(GL_TEXTURE_2D , texture_2);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_R , GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST);
    if (data_2){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_2, height_2, 0, GL_RGB,GL_UNSIGNED_BYTE, data_2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std:: cout<<"failed to load second texture!";
        glfwTerminate();
        return -1;
    }
    /*-------------------------Filter color-----------------------------------------*/
   
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program , "ourTexture_1"), 0); 
    glUniform1i(glGetUniformLocation(program , "ourTexture_2"), 1); 
    float opaciyt = 0.1;
    int ouropaciyt_uniform_location = glGetUniformLocation(program , "ouropaciyt");

    GLint duplicate_1_uniform_location = glGetUniformLocation(program , "duplicate_1");
    // glUseProgram(program);
    glUniform1i(duplicate_1_uniform_location , 1);
    GLint duplicate_2_uniform_location = glGetUniformLocation(program , "duplicate_2");
    // glUseProgram(program);
    glUniform1i(duplicate_2_uniform_location , 1);

    GLint ourcolor_1_uniform_location = glGetUniformLocation(program , "ourcolor_1");
    // glUseProgram(program);
    glUniform4f(ourcolor_1_uniform_location , 1.0 , 1.0 , 1.0 , 1.0);
    GLint ourcolor_2_uniform_location = glGetUniformLocation(program , "ourcolor_2");
    // glUseProgram(program);
    glUniform4f(ourcolor_2_uniform_location , 1.0 , 1.0 , 1.0 , 1.0);
    int i = 1;
    int j = 1;
    
    glfwSetFramebufferSizeCallback(window , window_resizing);
    //render while
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0 , 0.0 , 0.0 , 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        // glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, texture_1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D , texture_2);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES , 0 , 6);
        if(glfwGetKey(window , GLFW_KEY_UP) == GLFW_PRESS && opaciyt + 0.1 <= 1.0){

            opaciyt += 0.01;
            glUniform1f(ouropaciyt_uniform_location , opaciyt);
            
        }
        if(glfwGetKey(window , GLFW_KEY_DOWN) == GLFW_PRESS && 0 <= opaciyt){

            opaciyt -= 0.01;
            glUseProgram(program);
            glUniform1f(ouropaciyt_uniform_location , opaciyt);

        }

        if (glfwGetKey(window , GLFW_KEY_D) == GLFW_PRESS){
            duplicate(duplicate_1_uniform_location , program , i);
            i++;
        }
        else if (glfwGetKey(window , GLFW_KEY_S) == GLFW_PRESS){
            duplicate(duplicate_2_uniform_location , program , j);
            j++;
        }
        if (glfwGetKey(window , GLFW_KEY_C) == GLFW_PRESS){
            float filter_color[3];
            std:: cout<< "RGB values: "<<std:: endl;
            for (int i = 0 ; i < 3 ; i++){
            if(i == 0)
            std:: cout<< "R: ";
            else if(i == 1)
            std:: cout<< "G: ";
            else
            std:: cout<< "B: ";
            std:: cin >> filter_color[i];
            }
            setFilter(ourcolor_1_uniform_location , program , filter_color[0] , filter_color[1] , filter_color[2]);
        }


        window_closing(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
   glDeleteProgram(program); 
   stbi_image_free(data_1);
    return 0;
}