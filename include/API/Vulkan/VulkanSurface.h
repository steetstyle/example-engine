#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <GLFW/glfw3.h>

class VulkanSurface {
public:
    VulkanSurface(GLFWwindow* window, VkInstance vulkanInstance);
    static std::unique_ptr<VulkanSurface> Create(GLFWwindow* window, VkInstance vulkanInstance);
    ~VulkanSurface();

    VkSurfaceKHR GetSurface() const;

private:
    VkSurfaceKHR vkSurfaceKhr = VK_NULL_HANDLE;
    VkInstance vkInstance = VK_NULL_HANDLE;
};

#endif // VULKANSURFACE_