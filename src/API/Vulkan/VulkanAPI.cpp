#include <API/Vulkan/VulkanAPI.h>
#include <stdexcept>
#include <iostream> 

VulkanAPI::VulkanAPI() {
}

void VulkanAPI::Init() {
    vulkanInstance = VulkanInstance::Create();
    std::cout << "Vulkan API Initialized\n";
}

void VulkanAPI::Update() {
    std::cout << "Vulkan API Update\n";
}

void VulkanAPI::Cleanup() {
    vulkanInstance.reset();
    std::cout << "Vulkan API Cleanup\n";
}

VulkanAPI::~VulkanAPI() {
    Cleanup();
}