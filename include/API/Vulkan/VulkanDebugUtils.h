#ifndef VULKANDEBUGUTILS_H
#define VULKANDEBUGUTILS_H

#include <memory>
#include <vulkan/vulkan.h>

class VulkanDebugUtils {
    public:
        VulkanDebugUtils(VkInstance instance);
    
    static std::unique_ptr<VulkanDebugUtils> Create(VkInstance vulkanInstace);

    ~VulkanDebugUtils();

    VkDebugUtilsMessengerEXT GetDebugMessenger() const { return debugMessenger; }

    private:
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        VkResult CreateDebugUtilsMessenger(VkInstance instance);
        VkInstance vkInstance;
};


#endif VULKANDEBUGUTILS_H