#include <vulkan/vulkan.h>

#include <stdexcept>
#include <cstdlib>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application {
private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

public:
    void run();

private:
    GLFWwindow* window;

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
};
