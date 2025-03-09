#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <array>

class VulkanInstance {
public:
    VulkanInstance();

    // Factory method to create VulkanInstance
    static std::unique_ptr<VulkanInstance> Create();

    // Validation layer enablement
    #ifdef NDEBUG
        const bool validationLayersEnabled = false;
    #else
        const bool validationLayersEnabled = true;
    #endif

    // Returns the required extensions to create vulkan instance
    std::vector<const char*> GetRequiredExtensions();

    // Returns the validation layers for debugging
    std::array<const char*, 1> GetValidationLayers() const {
        return { "VK_LAYER_KHRONOS_validation" };
    }

    // Instance method to check if validation layers are supported
    bool CheckValidationLayerSupport() const;

    // Getter for VkInstance
    VkInstance GetVkInstance() const { return vkInstance; }

    // Destructor to clean up Vulkan resources
    ~VulkanInstance();

private:
    VkInstance vkInstance;
};

#endif // VULKANINSTANCE_H
