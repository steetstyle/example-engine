#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <array>
#include <API/Vulkan/VulkanDebugUtils.h>
#include <API/Vulkan/VulkanPhysicalDevice.h>
#include <API/Vulkan/VulkanSurface.h>
#include <GLFW/glfw3.h>

class VulkanInstance
{
public:
#ifdef NDEBUG
    const bool validationLayersEnabled = false;
#else
    const bool validationLayersEnabled = true;
#endif
    VulkanInstance(GLFWwindow* window);

    static std::unique_ptr<VulkanInstance> Create(GLFWwindow* window);

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

    std::vector<std::shared_ptr<VulkanPhysicalDevice>> GetPhysicalDevices() const { return physicalDevices; }
    std::vector<std::shared_ptr<VulkanPhysicalDevice>> GetChoosenPhysicalDevices() const { return chosenPhysicalDevices; }

    void CreateLogicalDevice();
    void CreateSurface(GLFWwindow* window);

private:
    VkInstance vkInstance;
    std::unique_ptr<VulkanDebugUtils> debugUtils;
    std::vector<std::shared_ptr<VulkanPhysicalDevice>> physicalDevices;
    std::vector<std::shared_ptr<VulkanPhysicalDevice>> chosenPhysicalDevices;
    std::unique_ptr<VulkanSurface> surface;
};

#endif // VULKANINSTANCE_H
