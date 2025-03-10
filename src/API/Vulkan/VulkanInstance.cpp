#include "API/Vulkan/VulkanInstance.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <vector>

VulkanInstance::VulkanInstance() : vkInstance(VK_NULL_HANDLE) {
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

    // Enable validation layer for vulkan instance
    if (validationLayersEnabled) {
       const char* layer_name = "VK_LAYER_KHRONOS_validation";

        auto validationLayers = GetValidationLayers();
         
        createInfo.enabledLayerCount = static_cast<uint32_t>(std::size(validationLayers));
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    auto extensions = GetRequiredExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create Vulkan instance!");
    }

    if(validationLayersEnabled) {
        debugUtils = VulkanDebugUtils::Create(vkInstance);
    }
}

bool VulkanInstance::CheckValidationLayerSupport() const {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // Get the required validation layer
    auto validationLayers = GetValidationLayers();

    for (int i = 0; i < std::size(validationLayers); i++) {
       auto it = std::find_if(
        availableLayers.begin(), 
        availableLayers.end(), 
        [validationLayers, i](const VkLayerProperties& layerProps) {
            return strcmp(validationLayers[i], layerProps.layerName) == 0;
        });

       if(it == availableLayers.end()){
            return false;
       }
    }

    return true;
}

std::unique_ptr<VulkanInstance> VulkanInstance::Create() {
    return std::make_unique<VulkanInstance>();
}

std::vector<const char *> VulkanInstance::GetRequiredExtensions()
{
    if (validationLayersEnabled && !CheckValidationLayerSupport()) {
        throw new std::runtime_error("validation layers are requested, but not available!");
    }

    // Query available extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available extensions:\n";
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    // Get required GLFW extensionsextensions.data()
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

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

    // Tp set up a callback in the program to handle messages and the associated details,
    // We have to set up a debug messenger with a callback using the VK_EXT_DEBUG_UTILS extenion
    if (validationLayersEnabled) {
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return requiredExtensions; 
}

VulkanInstance::~VulkanInstance() {
    if(debugUtils) {
        debugUtils.reset();
    }

    if (vkInstance != VK_NULL_HANDLE) {
        vkDestroyInstance(vkInstance, nullptr);
    }
}
