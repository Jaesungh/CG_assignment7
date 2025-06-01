#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

#include <GL/glew.h>            
#include <GLFW/glfw3.h>         
#include <glm/glm.hpp>          
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


GLuint LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path.c_str(), std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else {
        std::cout << "Error: could not open " << vertex_file_path << std::endl;
        return 0;
    }


    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }
    else {
        std::cout << "Error: could not open " << fragment_file_path << std::endl;
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    std::cout << "Compiling vertex shader: " << vertex_file_path << std::endl;
    const char* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        std::cout << &VertexShaderErrorMessage[0] << std::endl;
    }

    std::cout << "Compiling fragment shader: " << fragment_file_path << std::endl;
    const char* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);


    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        std::cout << &FragmentShaderErrorMessage[0] << std::endl;
    }


    std::cout << "Linking program\n";
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);


    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::cout << &ProgramErrorMessage[0] << std::endl;
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}


void Update_mesh(const GLuint& VAO,
    const std::vector<GLuint>& GLBuffers,
    const std::vector<glm::vec3>& Positions,
    const std::vector<glm::vec3>& Normals,
    const std::vector<GLuint>& Indices)
{

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER,
        Positions.size() * sizeof(Positions[0]),
        Positions.data(),
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                 // location 0
        3,                 // 크기: vec3
        GL_FLOAT,          // 데이터 타입
        GL_FALSE,          // 정규화 여부
        0,                 // stride (연결된 버퍼에 다른 데이터 없음)
        (void*)0);         // 오프셋 0부터 읽기

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER,
        Normals.size() * sizeof(Normals[0]),
        Normals.data(),
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,                 // location 1
        3,                 // 크기: vec3
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0);

  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLBuffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        Indices.size() * sizeof(Indices[0]),
        Indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);

}


void createSphereMesh(std::vector<glm::vec3>& outPositions,
    std::vector<glm::vec3>& outNormals,
    std::vector<GLuint>& outIndices)
{
    const int WIDTH_SEG = 32;
    const int HEIGHT_SEG = 16;


    for (int j = 1; j < HEIGHT_SEG - 1; ++j)
    {
        float theta = float(j) / float(HEIGHT_SEG - 1) * M_PI;
        for (int i = 0; i < WIDTH_SEG; ++i)
        {
            float phi = float(i) / float(WIDTH_SEG - 1) * 2.0f * M_PI;
            float x = std::sinf(theta) * std::cosf(phi);
            float y = std::cosf(theta);
            float z = -std::sinf(theta) * std::sinf(phi);

            outPositions.push_back(glm::vec3(x, y, z));
            outNormals.push_back(glm::normalize(glm::vec3(x, y, z)));
        }
    }
    // top pole
    outPositions.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    outNormals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    // bottom pole
    outPositions.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    outNormals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));


    int rows = HEIGHT_SEG - 2;         
   
    for (int j = 0; j < rows - 1; ++j)
    {
        for (int i = 0; i < WIDTH_SEG - 1; ++i)
        {
            int base = j * WIDTH_SEG;
     
            outIndices.push_back(base + i);
            outIndices.push_back((j + 1) * WIDTH_SEG + (i + 1));
            outIndices.push_back(base + (i + 1));
   
            outIndices.push_back(base + i);
            outIndices.push_back((j + 1) * WIDTH_SEG + i);
            outIndices.push_back((j + 1) * WIDTH_SEG + (i + 1));
        }
    }


    int topIndex = rows * WIDTH_SEG;
    for (int i = 0; i < WIDTH_SEG - 1; ++i)
    {
        outIndices.push_back(topIndex);
        outIndices.push_back(i);
        outIndices.push_back(i + 1);
    }


    int botIndex = rows * WIDTH_SEG + 1;
    int rowStart = (rows - 1) * WIDTH_SEG;
    for (int i = 0; i < WIDTH_SEG - 1; ++i)
    {
        outIndices.push_back(botIndex);
        outIndices.push_back(rowStart + (i + 1));
        outIndices.push_back(rowStart + i);
    }
}


int gWindowWidth = 800;
int gWindowHeight = 800;

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    // 'ESC' 또는 'Q' 누르면 종료
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


// main(): 초기화 → 셰이더 로드 → 메쉬 생성 → VAO/VBO/EBO 업로드 → 렌더 루프 → 정리
int main()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 윈도우 생성
    GLFWwindow* window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Phong Shading", NULL, NULL);
    if (!window) {
        std::cout << "Failed to open GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint shaderProgram = LoadShaders("shaders/Phong.vert", "shaders/Phong.frag");
    if (shaderProgram == 0) {
        std::cout << "Failed to load shaders.\n";
        return -1;
    }


    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<GLuint>   Indices;
    createSphereMesh(Positions, Normals, Indices);


    GLuint VAO;
    glGenVertexArrays(1, &VAO);


    std::vector<GLuint> GLBuffers(3, 0);
    glGenBuffers(3, GLBuffers.data());


    Update_mesh(VAO, GLBuffers, Positions, Normals, Indices);


    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);


        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -7.0f));
        model = glm::scale(model, glm::vec3(2.0f));

        glm::mat4 view = glm::mat4(1.0f);



        float aspect = float(gWindowWidth) / float(gWindowHeight);
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);


        GLint loc_model = glGetUniformLocation(shaderProgram, "model");
        GLint loc_view = glGetUniformLocation(shaderProgram, "view");
        GLint loc_projection = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        GLint loc_normalMatrix = glGetUniformLocation(shaderProgram, "normalMatrix");
        glUniformMatrix3fv(loc_normalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));


        GLint loc_lightPos = glGetUniformLocation(shaderProgram, "lightPos");
        GLint loc_viewPos = glGetUniformLocation(shaderProgram, "viewPos");
        GLint loc_lightAmb = glGetUniformLocation(shaderProgram, "lightAmbient");
        GLint loc_lightDiff = glGetUniformLocation(shaderProgram, "lightDiffuse");
        GLint loc_lightSpec = glGetUniformLocation(shaderProgram, "lightSpecular");
        GLint loc_matAmb = glGetUniformLocation(shaderProgram, "matAmbient");
        GLint loc_matDiff = glGetUniformLocation(shaderProgram, "matDiffuse");
        GLint loc_matSpec = glGetUniformLocation(shaderProgram, "matSpecular");
        GLint loc_shininess = glGetUniformLocation(shaderProgram, "shininess");
        GLint loc_gamma = glGetUniformLocation(shaderProgram, "gamma");

        glUniform3f(loc_lightPos, -4.0f, 4.0f, -3.0f);
        glUniform3f(loc_viewPos, 0.0f, 0.0f, 0.0f);
        glUniform3f(loc_lightAmb, 0.2f, 0.2f, 0.2f);
        glUniform3f(loc_lightDiff, 1.0f, 1.0f, 1.0f);
        glUniform3f(loc_lightSpec, 1.0f, 1.0f, 1.0f);
        glUniform3f(loc_matAmb, 0.0f, 1.0f, 0.0f);
        glUniform3f(loc_matDiff, 0.0f, 0.5f, 0.0f);
        glUniform3f(loc_matSpec, 0.5f, 0.5f, 0.5f);
        glUniform1f(loc_shininess, 32.0f);
        glUniform1f(loc_gamma, 2.2f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,
            static_cast<GLsizei>(Indices.size()),
            GL_UNSIGNED_INT,
            (void*)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    if (GLBuffers[0] != 0) {
        glDeleteBuffers(static_cast<GLsizei>(GLBuffers.size()), GLBuffers.data());
    }
    glDeleteProgram(shaderProgram);


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
