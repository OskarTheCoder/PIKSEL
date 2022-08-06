#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <Windows.h>
#include <vector>


void on_resize(GLFWwindow* window, int width, int height)
{
    //std::cout << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow* window, float &offset, float &offsetY)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        offset -= 0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        offset += 0.001;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        offsetY += 0.001;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        offsetY -= 0.001;
    }

}

void clearColorRGB(int r, int g, int b, int a)
{
    glClearColor((float)r / 255.0, (float)g / 255.0, (float)b / 255.0, (float)a / 255.0);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float size;\n"
"uniform float offset;\n"
"uniform float offsetY;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x * size + offset, aPos.y * size + offsetY, aPos.z * size, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

class GeometryObject {
public:
    int x;
    int y;
    int w;
    int h;
    std::vector<std::vector<int>> points;
    GeometryObject(int _x, int _y, int _w, int _h) {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        points = { {_x,_y}, {_x + _w,_y}, {_x,_y + _h}, {_x + _w,_y + _h} };
    }
};

void drawObject(unsigned int &shaderProgram) {
    glUseProgram(shaderProgram);


    glUseProgram(0);
}

//vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
int main()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Open Graphics Library", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window " << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, on_resize);

    GeometryObject obj = GeometryObject(-1.0f, -1.0f, 1.0f, 1.0f);

    
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3,  // second Triangle
    };

   // std::vector

    /*
    float vertices[] = {
        obj.x, obj.y, 0.0f,
        obj.x + obj.w, obj.y, 0.0f,
        obj.x + obj.w, obj.y + obj.h, 0.0f
    };
    

    unsigned int indices[] = {
        0, 1, 2
    };*/

    std::vector<float> vec = {
        0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int VertexBufferObject, VertexArrayObject, EBO;

    glGenVertexArrays(1, &VertexArrayObject);
    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vec.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    unsigned int vertexShader{};
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    
    glBindBuffer(GL_ARRAY_BUFFER, VertexArrayObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool draw = true;
    float size = 1.0f;
    float offset = 0.0f;
    float offsetY = 0.0f;
    float color[4] = { 1.0f, 0.5f, 0.3f, 1.0f };
    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
    glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

    while (!glfwWindowShouldClose(window)) {

        // input
        handleInput(window, offset, offsetY);

        // tegn opp ting her
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        clearColorRGB(65, 80, 80, 255);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        

        // Element Buffer Object
        glUseProgram(shaderProgram);
        glBindVertexArray(VertexArrayObject);

        if (draw)
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        //glBindVertexArray(0);

        ImGui::Begin("Renderer");
        //ImGui::Text("OpenGL is quite nice!");
        ImGui::Checkbox("Render", &draw);
        ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
        ImGui::ColorEdit4("Color", color);
        ImGui::End();

        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
        glUniform1f(glGetUniformLocation(shaderProgram, "offset"), offset);
        glUniform1f(glGetUniformLocation(shaderProgram, "offsetY"), offsetY);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        glfwSwapBuffers(window);


        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteBuffers(1, &VertexBufferObject);
    glDeleteVertexArrays(1, &VertexArrayObject);

    glfwTerminate();


    return 0;
}