#include "VulkanRenderPassManager.h"

#include <stdexcept>
#include <vector>
#include <algorithm>

VulkanRenderPassManager* VulkanRenderPassManager::instance{nullptr};

void VulkanRenderPassManager::InitGeometryPass(VkDevice device)
{
    
    if (GetGeometryPass() != VK_NULL_HANDLE) 
    {
        vkDestroyRenderPass(device, geometryPass, nullptr);
        geometryPass = VK_NULL_HANDLE;
    }


    // Attachement de couleur
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = GetColorFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Attachement de profondeur
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = GetDepthStencilFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription normalAttachment{};
    normalAttachment.format = GetColorFormat();
    normalAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    normalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    normalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    normalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    normalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    normalAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    normalAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkAttachmentDescription posAttachment{};
    posAttachment.format = GetHDRFormat();
    posAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    posAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    posAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    posAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    posAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    posAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    posAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    /* BIND ATTACHEMENT AND SUBPASS*/
    std::vector<VkAttachmentReference> colorReferences = {
        { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, // Albedo
        { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, // Normales
        { 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }  // Positions
    };

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = colorReferences.size();
    subpass.pColorAttachments = colorReferences.data();
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::vector<VkAttachmentDescription> attachments = {
        colorAttachment, 
        depthAttachment, 
        normalAttachment,
        posAttachment
    };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &geometryPass) != VK_SUCCESS)
        throw std::runtime_error("Failed to create geometry pass!");
}

void VulkanRenderPassManager::InitShadowPass(VkDevice device)
{
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = GetShadowMapFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 0;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 0;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependencies[2] = {};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &depthAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 2;
    renderPassInfo.pDependencies = dependencies;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &shadowPass) != VK_SUCCESS) 
        throw std::runtime_error("Failed to create shadow map render pass!");
}

void VulkanRenderPassManager::InitLightingPass(VkDevice device)
{
    if (GetLightingPass() != VK_NULL_HANDLE) 
    {
        vkDestroyRenderPass(device, lightingPass, nullptr);
        lightingPass = VK_NULL_HANDLE;
    }

    // Attachement pour la sortie couleur (éclairage final)
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = GetHDRFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;  // On efface le framebuffer
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // Ou SHADER_READ_ONLY si post-processing

    // Référence d'attachement pour la couleur
    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Subpass description
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    
    // Pas besoin de depth/stencil car on utilise le G-Buffer existant
    // et on fait du full-screen quad avec depth test désactivé

    // Dépendance avec la passe précédente (remplissage du G-Buffer)
    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };

    // Création de la render pass
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &lightingPass) != VK_SUCCESS)
        throw std::runtime_error("Failed to create lighting pass!");
}

bool VulkanRenderPassManager::FormatContainStencil(VkFormat format)
{
    const std::vector<VkFormat> stencilFormats = 
    {
        VK_FORMAT_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT
    };

    return std::find(stencilFormats.begin(), stencilFormats.end(), format) != stencilFormats.end();
}

VulkanRenderPassManager *VulkanRenderPassManager::GetInstance()
{
    if (!instance)
        instance = new VulkanRenderPassManager();

    return instance;
}

void VulkanRenderPassManager::Cleanup()
{
    if (GetGeometryPass() != VK_NULL_HANDLE) 
    {
        vkDestroyRenderPass(device, geometryPass, nullptr);
        geometryPass = VK_NULL_HANDLE;
    }


}


void VulkanRenderPassManager::InitRenderPasses(VkDevice device, PassesInfo infos)
{
    this->device = device;
    passesInfo = infos;

    if (!FormatContainStencil(infos.depthStencilFormat))
        throw std::invalid_argument("Failed to initialize render passes : Depth stencil format does not support stencil");

    InitGeometryPass(device);
    InitShadowPass(device);
    InitLightingPass(device);
}
