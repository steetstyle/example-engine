#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <array>
#include <API/Vulkan/VulkanDebugUtils.h>

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
VulkanInstance();
    static std::unique_ptr<VulkanInstance> Create();
    ~VulkanInstance();
    
    VkInstance GetVkInstance() const { return vkInstance; }
    std::vector<const char *> GetRequiredExtensions();
    VulkanDebugUtils *GetDebugUtils() { return debugUtils.get(); }
    std::array<const char *, 1> GetValidationLayers() const
    {
        return {"VK_LAYER_KHRONOS_validation"};
    }

    // Instance method to check if validation layers are supported
    bool CheckValidationLayerSupport() const;

    // Getter for VkInstance
    VkInstance GetVkInstance() const { return vkInstance; }

    // Getter for VulkanDebugUtils to access the debug messenger
    VulkanDebugUtils* GetDebugUtils() { return debugUtils.get(); } 

    // Destructor to clean up Vulkan resources
    ~VulkanInstance();

private:
    VkInstance vkInstance;
    std::unique_ptr<VulkanDebugUtils> debugUtils;
};

#endif // VULKANINSTANCE_H
