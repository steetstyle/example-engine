#ifndef APPLICATION_H
#define APPLICATION_H

#include <API/IGraphicsAPI.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

class Application {
    private:
        const uint32_t WIDTH = 800;
        const uint32_t HEIGHT = 600;
        std::unique_ptr<IGraphicsAPI> api;
        GLFWwindow* window;
    
    public:
        void Run();
        void Update();

    public:
        ~Application();
};

#endif // APPLICATION_H