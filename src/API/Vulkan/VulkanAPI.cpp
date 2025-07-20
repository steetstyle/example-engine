#include <API/Vulkan/VulkanAPI.h>
#include <stdexcept>
#include <iostream> 
#include <GLFW/glfw3.h>

VulkanAPI::VulkanAPI() {
}

void VulkanAPI::Init(GLFWwindow* window) {
    vulkanInstance = VulkanInstance::Create(window);
    std::cout << "Vulkan API Initialized\n";
    // Get Vulkan debug utils if validation layers are enabled
    if (vulkanInstance->GetDebugUtils()) {
        std::cout << "Debug utils initialized." << std::endl;
    }
}

void VulkanAPI::Update() {
    //std::cout << "Vulkan API Update\n";
}

void VulkanAPI::Cleanup() {
    if(vulkanInstance){
        vulkanInstance.reset();
    }
}

VulkanAPI::~VulkanAPI() {
    Cleanup();        
    std::cout << "Vulkan API Cleanup\n";
}