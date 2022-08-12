#include "graphicsBuffers.h"


VkVertexInputBindingDescription Vertex::getBindingDescription() {
	VkVertexInputBindingDescription desc;

	desc.binding = 0;
	desc.stride = sizeof(Vertex);
	desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return desc;
}

std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
	attributeDescriptions[0].binding = 0; // binding is an index into the pBuffers array bound by vkCmdBindVertexBuffers
	attributeDescriptions[0].location = 0; // matches shader input?
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // 3 float(rgb) vec
	attributeDescriptions[0].offset = offsetof(Vertex, pos);

	attributeDescriptions[1].binding = 0;// binding value for an attribute is equivalent to the binding value you provide to glVertexAttribBinding
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; 
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	return attributeDescriptions;
}

#pragma region classBuffers

VertexBuffer::VertexBuffer(VkDevice device, size_t memSize, uint32_t memType, std::vector<Vertex> vertices):allocatorDevice(device), vertexData(vertices)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = memSize;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer object");
	}

	VkMemoryRequirements memRegs;
	vkGetBufferMemoryRequirements(allocatorDevice, vertexBuffer, &memRegs);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRegs.size;
	allocInfo.memoryTypeIndex = memType;

	if (vkAllocateMemory(allocatorDevice, &allocInfo, nullptr, &gpuMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate gpu memory");
	}

	vkBindBufferMemory(allocatorDevice, vertexBuffer, gpuMemory, 0);

	// map -> copy -> unmap
	void* data;
	vkMapMemory(allocatorDevice, gpuMemory, 0, memSize, 0, &data);
	memcpy(data, vertices.data(), memSize);
	vkUnmapMemory(allocatorDevice, gpuMemory);


}

VertexBuffer::~VertexBuffer()
{
	vkDestroyBuffer(allocatorDevice, vertexBuffer, nullptr);
	vkFreeMemory(allocatorDevice, gpuMemory, nullptr);
}

void VertexBuffer::bindForRender(VkCommandBuffer cmdBuffer)
{
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmdBuffer, 0, 1, buffers, offsets);
}

#pragma endregion classBuffers

#pragma region functionalBuffers

VkBuffer GXBuffer::createBuffer(VkDevice device, size_t size, VkBufferUsageFlags usageFlags) 
{
	VkBuffer buffer;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usageFlags;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer");
	}

	return buffer;
}

void GXBuffer::createAllocateBindBuffer(VkDevice device, VkPhysicalDevice gpu, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProps, VkBuffer& buffer, VkDeviceMemory& mem, size_t size)
{ 
	buffer = createBuffer(device, size, usage);
	// TODO hoida minttii. Staging ja t‰‰
	mem = allocateBuffer(device, gpu, buffer, memProps);

	vkBindBufferMemory(device, buffer, mem, 0);
}

VkDeviceMemory GXBuffer::allocateBuffer(VkDevice allocator, VkPhysicalDevice gpu, VkBuffer buffer, VkMemoryPropertyFlags memoryFlags)
{
	VkMemoryRequirements memRegs;
	vkGetBufferMemoryRequirements(allocator, buffer, &memRegs);

	uint32_t memType = findMemoryType(gpu, memRegs.memoryTypeBits, memoryFlags);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRegs.size;
	allocInfo.memoryTypeIndex = memType;

	VkDeviceMemory gpuMem;

	if (vkAllocateMemory(allocator, &allocInfo, nullptr, &gpuMem) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate gpu memory");
	}

	return gpuMem;
}


void GXBuffer::copyMemoryToGpu(VkDevice allocator, VkDeviceMemory gpuMem, size_t memSize, const void* copySrc)
{
	void* data;
	vkMapMemory(allocator, gpuMem, 0, memSize, 0, &data);
	memcpy(data, copySrc, memSize);
	vkUnmapMemory(allocator, gpuMem);
}

void GXBuffer::copyBuffer(VkDevice device, VkBuffer src, VkBuffer dst, VkCommandPool cmdPool, VkQueue queue, const VkBufferCopy &copyRegion)
{
	// allocate
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = cmdPool;
	allocInfo.commandBufferCount = 1;
	
	VkCommandBuffer cmdBuffer;

	if (vkAllocateCommandBuffers(device, &allocInfo, &cmdBuffer) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers for copying");

	// record
	VkCommandBufferBeginInfo begInfo{};
	begInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(cmdBuffer, &begInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin cmd buffer for copying");

	vkCmdCopyBuffer(cmdBuffer, src, dst, 1, &copyRegion);

	vkEndCommandBuffer(cmdBuffer);

	// submit
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffer;
	// you could theoretically set a semaphore or fence for this, but we currently have no need for such.

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);
	// Fence would allow for multiple submits but that will be handled later.

	vkFreeCommandBuffers(device, cmdPool, 1, &cmdBuffer);

}

void GXBuffer::freeAndDestroyBuffer(VkDevice allocator, VkBuffer buffer, VkDeviceMemory bufferMemory)
{
	vkDestroyBuffer(allocator, buffer, nullptr);
	vkFreeMemory(allocator, bufferMemory, nullptr);
}

uint32_t GXBuffer::findMemoryType(VkPhysicalDevice gpu, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties gpuMemProp;
	vkGetPhysicalDeviceMemoryProperties(gpu, &gpuMemProp); // evaluates the props

	for (uint32_t i = 0u; i < gpuMemProp.memoryTypeCount; i++) {
		if (typeFilter & (i << i)
			&& (gpuMemProp.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("No memory type found suitable");

	return uint32_t();
}

#pragma endregion functionalBuffers
