#include <API/Vulkan/VulkanSurface.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <stdexcept>
#include <vulkan/vulkan_win32.h>

VulkanSurface::VulkanSurface(GLFWwindow* window, VkInstance vulkanInstance)
{
    if(vulkanInstance == VK_NULL_HANDLE) {
        throw std::runtime_error("Vulkan instance should be initialized");
    }

    vkInstance = vulkanInstance;

    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(window);
    createInfo.hinstance = GetModuleHandle(nullptr);
    
    if (vkCreateWin32SurfaceKHR(vulkanInstance, &createInfo, nullptr, &vkSurfaceKhr) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

std::unique_ptr<VulkanSurface> VulkanSurface::Create(GLFWwindow* window, VkInstance vulkanInstance)
{
    return std::make_unique<VulkanSurface>(window, vulkanInstance);
}

VulkanSurface::~VulkanSurface()
{
    if (vkSurfaceKhr != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(vkInstance, vkSurfaceKhr, nullptr);
    }
}

VkSurfaceKHR VulkanSurface::GetSurface() const
{
    return vkSurfaceKhr;
}
