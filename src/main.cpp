#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <iostream>
#include <cmath>
#include <vector>

#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

                                                         // glfw window creation
                                                         // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // 创造着色器程序
    Shader my_shader = Shader(".\\Shader\\shader.vs", ".\\Shader\\shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // 背面
        -0.2f, -0.2f, -0.2f,  0.8f, 0.2f, 0.2f,
        0.2f, -0.2f, -0.2f,   0.8f, 0.2f, 0.2f,
        0.2f,  0.2f, -0.2f,   0.8f, 0.2f, 0.2f,
        0.2f,  0.2f, -0.2f,   0.8f, 0.2f, 0.2f,
        -0.2f,  0.2f, -0.2f,  0.8f, 0.2f, 0.2f,
        -0.2f, -0.2f, -0.2f,  0.8f, 0.2f, 0.2f,
        // 左
        -0.2f,  0.2f,  0.2f,  0.2f, 0.8f, 0.2f,
        -0.2f,  0.2f, -0.2f,  0.2f, 0.8f, 0.2f,
        -0.2f, -0.2f, -0.2f,  0.2f, 0.8f, 0.2f,
        -0.2f, -0.2f, -0.2f,  0.2f, 0.8f, 0.2f,
        -0.2f, -0.2f,  0.2f,  0.2f, 0.8f, 0.2f,
        -0.2f,  0.2f,  0.2f,  0.2f, 0.8f, 0.2f,
        // 正面
        -0.2f, -0.2f,  0.2f,  0.2f, 0.2f, 0.8f,
        0.2f, -0.2f,  0.2f,   0.2f, 0.2f, 0.8f,
        0.2f,  0.2f,  0.2f,   0.2f, 0.2f, 0.8f,
        0.2f,  0.2f,  0.2f,   0.2f, 0.2f, 0.8f,
        -0.2f,  0.2f,  0.2f,  0.2f, 0.2f, 0.8f,
        -0.2f, -0.2f,  0.2f,  0.2f, 0.2f, 0.8f,
        // 右
        0.2f,  0.2f,  0.2f,   0.2f, 0.8f, 0.8f,
        0.2f,  0.2f, -0.2f,   0.2f, 0.8f, 0.8f,
        0.2f, -0.2f, -0.2f,   0.2f, 0.8f, 0.8f,
        0.2f, -0.2f, -0.2f,   0.2f, 0.8f, 0.8f,
        0.2f, -0.2f,  0.2f,   0.2f, 0.8f, 0.8f,
        0.2f,  0.2f,  0.2f,   0.2f, 0.8f, 0.8f,
        // 下
        -0.2f, -0.2f, -0.2f,  0.8f, 0.2f, 0.8f,
        0.2f, -0.2f, -0.2f,   0.8f, 0.2f, 0.8f,
        0.2f, -0.2f,  0.2f,   0.8f, 0.2f, 0.8f,
        0.2f, -0.2f,  0.2f,   0.8f, 0.2f, 0.8f,
        -0.2f, -0.2f,  0.2f,  0.8f, 0.2f, 0.8f,
        -0.2f, -0.2f, -0.2f,  0.8f, 0.2f, 0.8f,
        // 上
        -0.2f,  0.2f, -0.2f,  0.8f, 0.8f, 0.2f,
        0.2f,  0.2f, -0.2f,   0.8f, 0.8f, 0.2f,
        0.2f,  0.2f,  0.2f,   0.8f, 0.8f, 0.2f,
        0.2f,  0.2f,  0.2f,   0.8f, 0.8f, 0.2f,
        -0.2f,  0.2f,  0.2f,  0.8f, 0.8f, 0.2f,
        -0.2f,  0.2f, -0.2f,  0.8f, 0.8f, 0.2f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Bonus 画立方体星球
    float car_vertices[] = {
        // 背面
        -0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.2f,
        0.05f, -0.05f, -0.05f,   0.8f, 0.2f, 0.2f,
        0.05f,  0.05f, -0.05f,   0.8f, 0.2f, 0.2f,
        0.05f,  0.05f, -0.05f,   0.8f, 0.2f, 0.2f,
        -0.05f,  0.05f, -0.05f,  0.8f, 0.2f, 0.2f,
        -0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.2f,
        // 左
        -0.05f,  0.05f,  0.05f,  0.2f, 0.8f, 0.2f,
        -0.05f,  0.05f, -0.05f,  0.2f, 0.8f, 0.2f,
        -0.05f, -0.05f, -0.05f,  0.2f, 0.8f, 0.2f,
        -0.05f, -0.05f, -0.05f,  0.2f, 0.8f, 0.2f,
        -0.05f, -0.05f,  0.05f,  0.2f, 0.8f, 0.2f,
        -0.05f,  0.05f,  0.05f,  0.2f, 0.8f, 0.2f,
        // 正面
        -0.05f, -0.05f,  0.05f,  0.2f, 0.2f, 0.8f,
        0.05f, -0.05f,  0.05f,   0.2f, 0.2f, 0.8f,
        0.05f,  0.05f,  0.05f,   0.2f, 0.2f, 0.8f,
        0.05f,  0.05f,  0.05f,   0.2f, 0.2f, 0.8f,
        -0.05f,  0.05f,  0.05f,  0.2f, 0.2f, 0.8f,
        -0.05f, -0.05f,  0.05f,  0.2f, 0.2f, 0.8f,
        // 右
        0.05f,  0.05f,  0.05f,   0.2f, 0.8f, 0.8f,
        0.05f,  0.05f, -0.05f,   0.2f, 0.8f, 0.8f,
        0.05f, -0.05f, -0.05f,   0.2f, 0.8f, 0.8f,
        0.05f, -0.05f, -0.05f,   0.2f, 0.8f, 0.8f,
        0.05f, -0.05f,  0.05f,   0.2f, 0.8f, 0.8f,
        0.05f,  0.05f,  0.05f,   0.2f, 0.8f, 0.8f,
        // 下
        -0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.8f,
        0.05f, -0.05f, -0.05f,   0.8f, 0.2f, 0.8f,
        0.05f, -0.05f,  0.05f,   0.8f, 0.2f, 0.8f,
        0.05f, -0.05f,  0.05f,   0.8f, 0.2f, 0.8f,
        -0.05f, -0.05f,  0.05f,  0.8f, 0.2f, 0.8f,
        -0.05f, -0.05f, -0.05f,  0.8f, 0.2f, 0.8f,
        // 上
        -0.05f,  0.05f, -0.05f,  0.8f, 0.8f, 0.2f,
        0.05f,  0.05f, -0.05f,   0.8f, 0.8f, 0.2f,
        0.05f,  0.05f,  0.05f,   0.8f, 0.8f, 0.2f,
        0.05f,  0.05f,  0.05f,   0.8f, 0.8f, 0.2f,
        -0.05f,  0.05f,  0.05f,  0.8f, 0.8f, 0.2f,
        -0.05f,  0.05f, -0.05f,  0.8f, 0.8f, 0.2f
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // 构建星球
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(car_vertices), car_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Imgui 的设置
    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // render loop 控制变量
    bool show_demo_window = false;

    int mode = 1;

    int carNum = 3;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // set up imgui 
        ImGui_ImplGlfwGL3_NewFrame();

        // 1. Show a simple window.
        // This is the menu of gui
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {
            ImGui::Begin("Menu");

            ImGui::Text("I want HIGH SCORE!! Thank you TAs~");
            ImGui::Text("Choose the following mode you want.");

            // 模式选择框
            if (ImGui::Button("Static"))
                mode = 0;
            ImGui::SameLine();
            if (ImGui::Button("Translation"))
                mode = 1;
            ImGui::SameLine();
            if (ImGui::Button("Rotation"))
                mode = 2;
            ImGui::SameLine();
            if (ImGui::Button("Scaling"))
                mode = 3;
            if (ImGui::Button("Bonus"))
                mode = 4;

            switch (mode)
            {
            case 0:
                ImGui::Text("Cube is static");
                break;
            case 1:
                ImGui::Text("Cube translates");
                break;
            case 2:
                ImGui::Text("Cube rotates");
                break;
            case 3:
                ImGui::Text("Cube scales");
                break;
            case 4:
                ImGui::Text("Bonus: Simulate satellite rotating.");
                break;
            default:
                break;
            }
            ImGui::End();
        }

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        std::vector<glm::mat4> carModel;
        carModel.assign(carNum, glm::mat4(1));

        view = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        my_shader.use();
        my_shader.setMat4("view", glm::value_ptr(view));
        my_shader.setMat4("projection", glm::value_ptr(projection));
        switch (mode)
        {
        case 0:
            my_shader.setMat4("model", glm::value_ptr(model));
            // render box
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            break;
        case 1:
            model = glm::translate(model, (float)sin(glfwGetTime()) * glm::vec3(0.5f, 0.0f, 0.0f));
            my_shader.setMat4("model", glm::value_ptr(model));
            // render box
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            break;
        case 2:
            model = glm::rotate(model, (float)glfwGetTime() * 80.0f, glm::vec3(0.0f, 1.0f, 1.0f));
            //model = glm::translate(model, glm::vec3(0.0, 0.3, 0.0));
            my_shader.setMat4("model", glm::value_ptr(model));
            // render box
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            break;
        case 3:
            model = glm::scale(model, (float)abs(sin(glfwGetTime())) * glm::vec3(2.0f, 2.0f, 2.0f));
            my_shader.setMat4("model", glm::value_ptr(model));
            // render box
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            break;
        case 4:
            glBindVertexArray(vao);
            
            // 画太阳
            carModel[0] = glm::scale(carModel[0], glm::vec3(2.0, 2.0, 2.0));
            carModel[0] = glm::rotate(carModel[0], (float)glfwGetTime() * 30.0f, glm::vec3(0.0f, 0.0f, 1.0f));

            // 画地球
            carModel[1] = glm::rotate(carModel[1], (float)glfwGetTime() * 40.0f, glm::vec3(0.0, 0.0, 1.0));
            carModel[1] = glm::translate(carModel[1], glm::vec3(0.55, 0.55, 0.0));

            // 画月球
            carModel[2] = glm::scale(carModel[2], glm::vec3(0.7, 0.7, 0.7));
            carModel[2] = carModel[1] * carModel[2];
            carModel[2] = glm::rotate(carModel[2], (float)glfwGetTime() * 50.0f, glm::vec3(0.0, 0.0, 1.0));
            carModel[2] = glm::translate(carModel[2], glm::vec3(0.3, 0.3, 0.0));

            view = glm::lookAt(glm::vec3(0.0f, -2.0f, 3.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f));
            my_shader.setMat4("view", glm::value_ptr(view));

            my_shader.setMat4("model", glm::value_ptr(carModel[0]));
            glDrawArrays(GL_TRIANGLES, 0, 36);
            my_shader.setMat4("model", glm::value_ptr(carModel[1]));
            glDrawArrays(GL_TRIANGLES, 0, 36);
            my_shader.setMat4("model", glm::value_ptr(carModel[2]));
            glDrawArrays(GL_TRIANGLES, 0, 36);
            break;
        default:
            break;
        }

        // Imgui render
        if (show_demo_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}