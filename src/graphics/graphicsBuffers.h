#pragma once
#include "glm/vec3.hpp"
#include <vulkan/vulkan.h>
// std
#include <array>
#include <vector>
#include <stdexcept>

struct Vertex {
	// members
	glm::vec3 pos;
	glm::vec3 color;

	// methods

	///<summary>
	/// Returns the format in which the vertex shader input is.
	///</summary>
	static VkVertexInputBindingDescription getBindingDescription();

	///<summary>
	/// Returns how to interpret the members of vertexdata
	///</summary>
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

///<summary>
/// Vertex buffer wrapper that deals with allocation and deallocation.
///</summary>
class VertexBuffer 
{

	VertexBuffer(VkDevice device, size_t size, uint32_t memType, std::vector<Vertex> vertices);
	~VertexBuffer();

	void bindForRender(VkCommandBuffer cmdBuffer);

private:
	VkDevice allocatorDevice;

	VkBuffer vertexBuffer;
	VkDeviceMemory gpuMemory;

	std::vector<Vertex> vertexData;
};

///<summary>
/// Returns a Vertex buffer object for given device with the specified size.
/// <remark> 
/// DOES NOT ALLOCATE/ASSIGN MEMORY FOR THE BUFFER. 
/// However the size specified can be used in the allocation phase in order to make sure the memory size match
/// </remark>
///</summary>
VkBuffer createVertexBuffer(VkDevice device, size_t size);

// void createIndexBuffer(); Should be created when creating a buffer? maybe we should combine the two as they are used together anyway.

//Dummy data tetrahedron
const std::vector<Vertex> tetrahedronVertices = {
	{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
	{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
	{{1.0f, 1.0f, 0.0f}, {0.8f, 0.7f, 0.0f}}
};