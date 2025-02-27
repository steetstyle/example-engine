#ifndef VULKANAPI_H
#define VULKANAPI_H

#include <API/IGraphicsAPI.h>
#include "VulkanInstance.h"
#include <memory>

class VulkanAPI : public IGraphicsAPI {
public:
    VulkanAPI();

    virtual void Init() override;
    virtual void Update() override;
    virtual void Cleanup() override;

    ~VulkanAPI();

private:
    std::unique_ptr<VulkanInstance> vulkanInstance;
};

#endif // VULKANAPI_H
