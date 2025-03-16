#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <array>
#include <API/Vulkan/VulkanDebugUtils.h>
#include <API/Vulkan/VulkanPhysicalDevice.h>

class VulkanInstance
{
public:
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

    bool CheckValidationLayerSupport() const;

    void PickPhysicalDevice();

    std::vector<std::shared_ptr<VulkanPhysicalDevice>> GetPhysicalDevices() const { return phsyicalDevices; }

private:
    VkInstance vkInstance;
    std::unique_ptr<VulkanDebugUtils> debugUtils;
    std::vector<std::shared_ptr<VulkanPhysicalDevice>> phsyicalDevices;
};

#endif // VULKANINSTANCE_H
