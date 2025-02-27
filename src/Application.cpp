#include "API/Vulkan/VulkanAPI.h"  // Correct header path
#include "Application.h"  // Correct header path
#include <iostream> 
#include <memory> 

void Application::Run() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    api = std::make_unique<VulkanAPI>();
    api->Init();

    Update();
}


void Application::Update() {
    while (!glfwWindowShouldClose(window)) {
        api->Update();
        glfwPollEvents();
    }
}

Application::~Application() {
    api->Cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}