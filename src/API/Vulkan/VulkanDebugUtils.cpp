#include <API/Vulkan/VulkanDebugUtils.h>
#include <iostream>

VulkanDebugUtils::VulkanDebugUtils(VkInstance instance)
{
    // Initialize the Vulkan instance
    vkInstance = instance;

    // Attempt to create the debug messenger
    VkResult result = CreateDebugUtilsMessenger(vkInstance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Vulkan debug messenger creation failed.");
    }
}

std::unique_ptr<VulkanDebugUtils> VulkanDebugUtils::Create(VkInstance vulkanInstance)
{
    // Create a VulkanDebugUtils instance and return it as a unique pointer
    return std::make_unique<VulkanDebugUtils>(vulkanInstance);
}

VulkanDebugUtils::~VulkanDebugUtils()
{
    // Clean up the debug messenger when the object is destroyed
    if (debugMessenger != VK_NULL_HANDLE) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(vkInstance, debugMessenger, nullptr);
        }
    }
}

VkResult VulkanDebugUtils::CreateDebugUtilsMessenger(VkInstance instance)
{
    // Prepare the creation info for the debug messenger
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    
    createInfo.pfnUserCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                                    VkDebugUtilsMessageTypeFlagsEXT messageType, 
                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
                                    void* pUserData) {
        std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    };

    // Retrieve the function pointer for creating the debug messenger
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        // Call the Vulkan function to create the debug messenger
        return func(instance, &createInfo, nullptr, &debugMessenger);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}
