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

VkBuffer createVertexBuffer(VkDevice device, size_t size)
{ // VkBuffer is a handle, so copying it __should__ not cause problems
	VkBuffer vertexBuffer;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer");
	}

	return vertexBuffer;
}


