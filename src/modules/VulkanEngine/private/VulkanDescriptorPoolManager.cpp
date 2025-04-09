#include "VulkanDescriptorPoolManager.h"

#include "VulkanRenderEngine.h"

VulkanDescriptorPoolManager::VulkanDescriptorPoolManager(VulkanRenderEngine *renderEngine)
    : renderEngine(renderEngine)
{
}

VulkanDescriptorPoolManager::~VulkanDescriptorPoolManager()
{
    for (const auto& pool : descriptorPools)
    {
        VkResult result = vkResetDescriptorPool(renderEngine->GetDevice(), pool, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        if (result != VK_SUCCESS) 
        {
            std::cerr << "Failed to free descriptor set!" << std::endl;
        }

        vkDestroyDescriptorPool(renderEngine->GetDevice(), pool, nullptr);
    }

    descriptorPools.clear();
}

VkDescriptorPool VulkanDescriptorPoolManager::CreateNewDescriptorPool()
{

    std::vector<VkDescriptorPoolSize> poolSizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10}, // 10 uniform buffers
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10}, // 10 uniform buffers
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10}, // 10 uniform buffers
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 10}, // 10 uniform buffers
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 10},  // 10 sampled images
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10}  // 10 combined image samplers
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1000; // Nombre maximal de descriptor sets dans ce pool

    VkDescriptorPool pool;
    vkCreateDescriptorPool(renderEngine->GetDevice(), &poolInfo, nullptr, &pool);
    descriptorPools.push_back(pool);
    return pool;
}

bool VulkanDescriptorPoolManager::AllocateDescriptorSets(const VkDescriptorSetLayout* setLayouts, uint32_t descriptorSetCount, VkDescriptorSet *descriptorSets)
{
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = descriptorSetCount;
    allocInfo.pSetLayouts = setLayouts;
    allocInfo.pNext = nullptr;


    VkResult result;
    bool found{false};
    for (const auto& pool : descriptorPools)
    {
        allocInfo.descriptorPool = pool;
        result = vkAllocateDescriptorSets(renderEngine->GetDevice(), &allocInfo, descriptorSets);
    
        if (result == VK_SUCCESS)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        allocInfo.descriptorPool = CreateNewDescriptorPool();
        result = vkAllocateDescriptorSets(renderEngine->GetDevice(), &allocInfo, descriptorSets);
        return result == VK_SUCCESS;
    }

    return true;

}
