#include "VulkanCamera.h"
#include "SwapchainRenderer.h"
#include "IRenderable.h"
#include "ICamera.h"
#include "SwapchainRenderer.h"
#include "VulkanRenderer.h"
#include <array>

#include <glm/gtc/matrix_transform.hpp>

bool VulkanCamera::Init(IRenderEngine* renderEngine, class IRenderTarget* renderTarget)
{
    this->renderEngine = dynamic_cast<VulkanRenderer*>(renderEngine);
    if (!this->renderEngine)
    {
        return false;
    }

    // Init RenderPass
    CreateRenderPass();
    CreateDescriptorPool();
    CreateDescriptors();

    if (renderTarget)
        SetRenderTarget(renderTarget);

    viewBuffer.Create( this->renderEngine->GetDevice(), this->renderEngine->GetPhysicalDevice(), sizeof(TransformMVP));
    objectBuffer.Create( this->renderEngine->GetDevice(), this->renderEngine->GetPhysicalDevice(), sizeof(ObjectData));

    UpdateViewMatrix();
    UpdateProjectionMatrix();

    return true;
}

bool VulkanCamera::Render(IRenderable* renderable)
{

    SwapchainRenderer* sr = dynamic_cast<SwapchainRenderer*>(renderTarget);
    if (sr)
    {
        
        if(!sr->BeginRenderCommands())
            return false;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(GetRenderTarget()->GetWidth());
        viewport.height = static_cast<float>(GetRenderTarget()->GetHeight());
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
    
        VkRect2D scissor{}; 
        scissor.offset = {0, 0};
        scissor.extent = {GetRenderTarget()->GetWidth(), GetRenderTarget()->GetHeight()};
    
        vkCmdSetViewport(GetRenderTarget()->GetCurrentCommandBuffer(), 0, 1, &viewport);
        vkCmdSetScissor(GetRenderTarget()->GetCurrentCommandBuffer(), 0, 1, &scissor);
    
        SetAspectRatio(static_cast<float>(GetRenderTarget()->GetWidth()) / static_cast<float>(GetRenderTarget()->GetHeight()));

        objectData.model = glm::mat4(1.0f);
        objectData.model = glm::translate(objectData.model, {0.0f, 0.0f, 0.0f});

        objectBuffer.Update(&objectData);



        renderEngine->material.Bind(this);

    
        if (renderable)
            renderable->draw(this);

        sr->EndRenderCommandsAndPresent(renderEngine->GetPresentQueue(),renderEngine->GetGraphicsQueue());
    }

    return true;
}

void VulkanCamera::Cleanup()
{
    viewBuffer.Cleanup();
    objectBuffer.Cleanup();

    // Détruire le render pass
    if (renderPass != VK_NULL_HANDLE) 
    {
        vkDestroyRenderPass(renderEngine->GetDevice(), renderPass, nullptr);
        renderPass = VK_NULL_HANDLE;
    }

    // Détruire le descriptor pool
    if (cameraDescriptorPool != VK_NULL_HANDLE) 
    {
        vkDestroyDescriptorPool(renderEngine->GetDevice(), cameraDescriptorPool, nullptr);
        cameraDescriptorPool = VK_NULL_HANDLE;
    }
}

void VulkanCamera::SetRenderTarget(IRenderTarget *renderTarget)
{
    this->renderTarget = renderTarget;
    SwapchainRenderer* sr = dynamic_cast<SwapchainRenderer*>(renderTarget);
    if (sr)
    {
        sr->SetRenderPass(GetRenderPass());
    }
}

void VulkanCamera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void VulkanCamera::SetFOV(float fov)
{
    this->fov = fov;
    UpdateProjectionMatrix();
}

void VulkanCamera::SetNearPlan(float nearPlan)
{
    this->nearPlan = nearPlan;
    UpdateProjectionMatrix();
}

void VulkanCamera::SetFarPlan(float farPlan)
{
    this->farPlan = farPlan;
    UpdateProjectionMatrix();
}

IRenderTarget *VulkanCamera::GetRenderTarget() const { return renderTarget; }

void VulkanCamera::CreateRenderPass()
{
    // Attachement de couleur
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = renderEngine->GetSwapChainImageFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Attachement de profondeur
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = VK_FORMAT_D24_UNORM_S8_UINT; // ou VK_FORMAT_D32_SFLOAT_S8_UINT
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

    // Sous-passe
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    // Dépendance de sous-passe
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    // Création du Render Pass
    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(renderEngine->GetDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render pass!");
    }
}

void VulkanCamera::CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 10; // Gérer plusieurs materials

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 10;

    if (vkCreateDescriptorPool(renderEngine->GetDevice(), &poolInfo, nullptr, &cameraDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Échec de la création du descriptor pool !");
    }
}

void VulkanCamera::CreateDescriptors()
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = renderEngine->GetDescriptorPool();
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = renderEngine->GetCameraDescriptorLayout();

    if (vkAllocateDescriptorSets(renderEngine->GetDevice(), &allocInfo, &cameraDescriptorSet) != VK_SUCCESS)
    {
        throw std::runtime_error("Échec de l'allocation du descriptor set !");
    }
}

void VulkanCamera::UpdateViewMatrix()
{
    viewData.view = glm::lookAt(
        glm::vec3(5.0f, 3.0f, 4.0f), // Position caméra
        glm::vec3(0.0f, 0.0f, 0.0f),  // Point cible
        glm::vec3(0.0f, 1.0f, 0.0f)   // Vecteur "up"
    );
    UpdateViewDataBuffer();
}

void VulkanCamera::UpdateProjectionMatrix()
{
    viewData.proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlan, farPlan);
    UpdateViewDataBuffer();
}

void VulkanCamera::UpdateViewDataBuffer()
{
    viewBuffer.Update(&viewData);
            
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = GetViewDataBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(TransformMVP);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = *GetDescriptorSet();
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(renderEngine->GetDevice(), 1, &descriptorWrite, 0, nullptr);
}
