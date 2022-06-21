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

void createIndexBuffer()
{
}

