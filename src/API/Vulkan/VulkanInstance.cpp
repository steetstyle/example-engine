#include "API/Vulkan/VulkanInstance.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <vector>

VulkanInstance::VulkanInstance() : vkInstance(VK_NULL_HANDLE) {
    // Query available extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available extensions:\n";
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    // Get required GLFW extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Check if required GLFW extensions are available
    for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
        const char* requiredExtension = glfwExtensions[i];

        // Check if the required GLFW extension exists in the available extensions
        auto it = std::find_if(extensions.begin(), extensions.end(), [requiredExtension](const VkExtensionProperties& ext) {
            return std::strcmp(ext.extensionName, requiredExtension) == 0;
        });

        if (it == extensions.end()) {
            throw std::runtime_error("Required GLFW extension '" + std::string(requiredExtension) + "' is not supported by Vulkan.");
        }
    }

    // Create vulkan instance
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Create vulkan application
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    createInfo.pApplicationInfo = &appInfo;

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create Vulkan instance!");
    }
}

std::unique_ptr<VulkanInstance> VulkanInstance::Create() {
    return std::make_unique<VulkanInstance>();
}

VulkanInstance::~VulkanInstance() {
    if (vkInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(vkInstance, nullptr);
    }
}
