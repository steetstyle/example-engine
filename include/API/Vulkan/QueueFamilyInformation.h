#ifndef QUEUEFAMILYINFORMATION_H
#define QUEUEFAMILYINFORMATION_H

#include <cstdint>

struct QueueFamilyInformation {
    uint32_t graphicsBitIndex = UINT32_MAX;
    uint32_t computeBitIndex = UINT32_MAX;
    uint32_t transferBitIndex = UINT32_MAX;
    uint32_t sparseBindingBitIndex = UINT32_MAX;
    uint32_t protectedBitIndex = UINT32_MAX;
    uint32_t computeIndex = UINT32_MAX;
    uint32_t opticalFlowBitNvIndex = UINT32_MAX;

    uint32_t presentIndex = UINT32_MAX;

    bool isComplete() {
        return graphicsBitIndex != UINT32_MAX && presentIndex != UINT32_MAX;
    }
};

#endif // QUEUEFAMILYINFORMATION_H