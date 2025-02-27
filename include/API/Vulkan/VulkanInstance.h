#ifndef VULKANINSTANCE_H
#define VULKANINSTANCE_H

#include <vulkan/vulkan.h>
#include <memory>

class VulkanInstance {
public:
    VulkanInstance();

    static std::unique_ptr<VulkanInstance> Create();

    VkInstance GetVkInstance() const { return vkInstance; }

    ~VulkanInstance();

private:
    VkInstance vkInstance;
};

#endif // VULKANINSTANCE_H
